#ifndef CPOSITIONVISUALIZATIONGUI_H
#define CPOSITIONVISUALIZATIONGUI_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QValidator>
#include <QHostAddress>
#include <iostream>
#include "qcustomplot.h"
using namespace std;

#define VISUAL_PROT_PORT            34567
#define RANGE_MARGIN                1
namespace Ui {
class cPositionVisualizationGui;
}

class cPositionVisualizationGui : public QMainWindow
{
    Q_OBJECT
private:
    QCustomPlot *plotxy;
    QCustomPlot *plotxz;
    Ui::cPositionVisualizationGui *ui;
    QTcpSocket *socket;

    double xmin, xmax, ymin, ymax, zmin, zmax;
    QVector<double> beacon_x, beacon_y, beacon_z, pos_x, pos_y, pos_z, pos_inv_x, pos_inv_y, pos_inv_z;


    void receiveMessage(QString msg);
    void setupPlots();
public:
    explicit cPositionVisualizationGui(QWidget *parent = 0);
    ~cPositionVisualizationGui();


private slots:
    void on_connect_button_clicked();
    void on_disconnect_button_clicked();
    //socket
    void on_socket_connected();
    void on_socket_disconnected();
    void on_socket_readyRead();
    void on_clearButton_clicked();
};

#endif // CPOSITIONVISUALIZATIONGUI_H
