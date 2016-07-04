#include "cpositionvisualizationgui.h"
#include "ui_cpositionvisualizationgui.h"
#include "qcustomplot.h"

cPositionVisualizationGui::cPositionVisualizationGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cPositionVisualizationGui)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    xmin = -1;
    xmax = 1;
    ymin = -1;
    ymax = 1;
    zmin = -1;
    zmax = 1;

    ui->ip_lineEdit->setInputMask("000.000.000.000");
    ui->ip_lineEdit->setText("127.0.0.1");
    ui->port_lineEdit->setValidator(new QIntValidator(0, 100000));
    ui->port_lineEdit->setText(QString::number(VISUAL_PROT_PORT));
    ui->disconnect_button->setHidden(true);


    plotxy = new QCustomPlot(this);
    dynamic_cast<QVBoxLayout*>(ui->centralWidget->layout())->insertWidget(1, plotxy);
    plotxz = new QCustomPlot(this);
    dynamic_cast<QVBoxLayout*>(ui->centralWidget->layout())->insertWidget(2, plotxz);
    setupPlots();

    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(on_socket_readyRead()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(on_socket_connected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(on_socket_disconnected()));

}

cPositionVisualizationGui::~cPositionVisualizationGui()
{
    delete ui;
    if(socket != NULL)
    {
        delete socket;
    }
    delete plotxy;
    delete plotxz;
}

void cPositionVisualizationGui::receiveMessage(QString msg)
{
    QString msgDiv;
    QString val;
    QString tmp;


    while(msg.contains(";"))
    {
        msgDiv = msg.section(";",0,0);
        ui->log->append(msgDiv);
        tmp = msg.section("(",0,0);


        if(tmp == "beacon")
        {
           double x,y,z;

           tmp = msgDiv.section("(",-1,-1);
           tmp = tmp.section(")",0,0);

           x = tmp.section(",",0,0).toDouble();
           y = tmp.section(",",1,1).toDouble();
           z = tmp.section(",",2,2).toDouble();

           if(beacon_x.isEmpty()) //first message
           {
               xmin = x-RANGE_MARGIN;
               xmax = x+RANGE_MARGIN;
               ymin = y-RANGE_MARGIN;
               ymax = y+RANGE_MARGIN;
               zmin = z-RANGE_MARGIN;
               zmax = z+RANGE_MARGIN;
           }

           beacon_x.push_back(x);
           beacon_y.push_back(y);
           beacon_z.push_back(z);

           if(x < xmin) xmin = x-RANGE_MARGIN;
           if(x > xmax) xmax = x+RANGE_MARGIN;
           if(y < ymin) ymin = y-RANGE_MARGIN;
           if(y > ymax) ymax = y+RANGE_MARGIN;
           if(z < zmin) zmin = z-RANGE_MARGIN;
           if(z > zmax) zmax = z+RANGE_MARGIN;

           plotxy->xAxis->setRange(xmin, xmax);
           plotxy->yAxis->setRange(ymin, ymax);

           plotxz->xAxis->setRange(xmin, xmax);
           plotxz->yAxis->setRange(zmin, zmax);

           plotxy->graph(2)->setData(beacon_x, beacon_y);
           plotxz->graph(2)->setData(beacon_x, beacon_z);

           plotxy->replot();
           plotxz->replot();

        }
        else if(tmp == "solution")
        {
            uint64_t time;
            double x,y,z;
            bool valid;

            tmp = msgDiv.section("(",-1,-1);
            tmp = tmp.section(")",0,0);

            val = tmp.section(",",0,0);
            if(val == "valid")
            {
                valid = true;
            }
            if(val == "invalid")
            {
                valid = false;
            }

            tmp = tmp.section(",",1,-1);

            time = tmp.section(",",0,0).toUInt();
            x = tmp.section(",",1,1).toDouble();
            z = tmp.section(",",2,2).toDouble();
            y = tmp.section(",",3,3).toDouble();

            if(x < xmin) xmin = x-RANGE_MARGIN;
            if(x > xmax) xmax = x+RANGE_MARGIN;
            if(y < ymin) ymin = y-RANGE_MARGIN;
            if(y > ymax) ymax = y+RANGE_MARGIN;
            if(z < zmin) zmin = z-RANGE_MARGIN;
            if(z > zmax) zmax = z+RANGE_MARGIN;

            plotxy->xAxis->setRange(xmin, xmax);
            plotxy->yAxis->setRange(ymin, ymax);

            plotxz->xAxis->setRange(xmin, xmax);
            plotxz->yAxis->setRange(zmin, zmax);

            if(valid == true)
            {
                pos_x.push_back(x);
                pos_y.push_back(y);
                pos_z.push_back(z);
                plotxy->graph(0)->setData(pos_x, pos_y);
                plotxz->graph(0)->setData(pos_x, pos_z);
            }
            else
            {
                pos_inv_x.push_back(x);
                pos_inv_y.push_back(y);
                pos_inv_z.push_back(z);
                plotxy->graph(1)->setData(pos_inv_x, pos_inv_y);
                plotxz->graph(1)->setData(pos_inv_x, pos_inv_z);
            }

            plotxy->replot();
            plotxz->replot();

        }
        else
        {
            //invalid message
        }

        msg = msg.section(";",1,-1);

    }//end of while
}

void cPositionVisualizationGui::setupPlots()
{
    //setup xy plot
    plotxy->xAxis->setLabel("X");
    plotxy->yAxis->setLabel("Y");
    plotxy->addGraph();
    plotxy->graph(0)->setPen(QPen(QColor(0,153,0)));
    plotxy->graph(0)->setLineStyle(QCPGraph::lsNone);
    plotxy->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 10));
    plotxy->graph(0)->setData(pos_x, pos_y);
    plotxy->addGraph();
    plotxy->graph(1)->setPen(QPen(Qt::red));
    plotxy->graph(1)->setLineStyle(QCPGraph::lsNone);
    plotxy->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 10));
    plotxy->graph(1)->setData(pos_inv_x, pos_inv_y);
    plotxy->addGraph();
    plotxy->graph(2)->setPen(QPen(Qt::blue));
    plotxy->graph(2)->setLineStyle(QCPGraph::lsNone);
    plotxy->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 10));
    plotxy->graph(2)->setData(beacon_x, beacon_y);
    plotxy->xAxis->setRange(xmin, xmax);
    plotxy->yAxis->setRange(ymin, ymax);
    plotxy->replot();

    //setup xz plot
    plotxz->xAxis->setLabel("X");
    plotxz->yAxis->setLabel("Z");
    plotxz->addGraph();
    plotxz->graph(0)->setPen(QPen(QColor(0,153,0)));
    plotxz->graph(0)->setLineStyle(QCPGraph::lsNone);
    plotxz->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 10));
    plotxz->graph(0)->setData(pos_x, pos_z);
    plotxz->addGraph();
    plotxz->graph(1)->setPen(QPen(Qt::red));
    plotxz->graph(1)->setLineStyle(QCPGraph::lsNone);
    plotxz->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 10));
    plotxz->graph(1)->setData(pos_inv_x, pos_inv_z);
    plotxz->addGraph();
    plotxz->graph(2)->setPen(QPen(Qt::blue));
    plotxz->graph(2)->setLineStyle(QCPGraph::lsNone);
    plotxz->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 10));
    plotxz->graph(2)->setData(beacon_x, beacon_z);
    plotxz->xAxis->setRange(xmin, xmax);
    plotxz->yAxis->setRange(zmin, zmax);
}

void cPositionVisualizationGui::on_connect_button_clicked()
{
    socket->connectToHost(QHostAddress(ui->ip_lineEdit->text()), ui->port_lineEdit->text().toInt());
}

void cPositionVisualizationGui::on_disconnect_button_clicked()
{
    socket->disconnectFromHost();
}

void cPositionVisualizationGui::on_socket_connected()
{
       ui->connect_button->setHidden(true);
       ui->disconnect_button->setHidden(false);
}

void cPositionVisualizationGui::on_socket_disconnected()
{
    ui->connect_button->setHidden(false);
    ui->disconnect_button->setHidden(true);
}

void cPositionVisualizationGui::on_socket_readyRead()
{
    qint64 bytes = socket->bytesAvailable();
    char buffer[bytes];
    memset((char*)buffer, 0, bytes);

    socket->read((char*)buffer, bytes);

    receiveMessage(QString(buffer));
}

void cPositionVisualizationGui::on_clearButton_clicked()
{
    ui->log->clear();
    pos_x.clear();
    pos_y.clear();
    pos_z.clear();
    pos_inv_x.clear();
    pos_inv_y.clear();
    pos_inv_z.clear();
    plotxy->clearGraphs();
    plotxz->clearGraphs();

    setupPlots();

    plotxy->graph(2)->setData(beacon_x, beacon_y);
    plotxz->graph(2)->setData(beacon_x, beacon_z);

    plotxy->replot();
    plotxz->replot();
}
