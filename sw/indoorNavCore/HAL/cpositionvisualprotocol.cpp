#include "cpositionvisualprotocol.h"


cPositionVisualProtocol::cPositionVisualProtocol(int32_t port, QObject *parent) :  QObject(parent), cProtocolDriver()
{
    open = false;
    socket = NULL;
    this->port = port;

    server = new QTcpServer(this);
    server->listen(QHostAddress::Any, port);

    QObject::connect(server, SIGNAL(newConnection()), this, SLOT(on_server_newConnection()));
}

cPositionVisualProtocol::~cPositionVisualProtocol()
{
    if(socket != NULL)
    {
        delete socket;
    }
    delete server;
}

void cPositionVisualProtocol::writeInitialInformation()
{
    QString tmp;
    for(uint i=0; i<beaconNumber; i++)
    {
        tmp = "beacon(";
        tmp += QString::number(beaconResources[i].latitude) + ",";
        tmp += QString::number(beaconResources[i].longitude) + ",";
        tmp += QString::number(beaconResources[i].height) + ");";
        socket->write(tmp.toStdString().c_str(), tmp.size());
        socket->flush();
    }
}

bool cPositionVisualProtocol::writePositionToFlightCtrl(tPositionSolution solution)
{
    bool k = true;
    QString tmp;
    qint64 writebytes;
    if(open == true)
    {
        tmp = "solution(";
        if(solution.validity)
        {
            tmp += "valid,";
        }
        else
        {
            tmp += "invalid,";
        }
        tmp += QString::number(solution.gpstime) + ",";
        tmp += QString::number(solution.position(0)) + ",";
        tmp += QString::number(solution.position(1)) + ",";
        tmp += QString::number(solution.position(2)) + ");";

        writebytes = socket->write(tmp.toStdString().c_str(), tmp.size());
        if(writebytes != tmp.size())
        {
            k = false;
        }
    }
    else
    {
        std::cout<<"-> WARNING <- cPositionVisualProtocol::writePositionToFlightControl(): Socket is not open!"<<std::endl;
    }
    return k;
}

void cPositionVisualProtocol::on_server_newConnection()
{
    socket = server->nextPendingConnection();
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(on_socket_connected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(on_socket_disconnected()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(on_socket_readyRead()));
    writeInitialInformation();
    server->pauseAccepting();
    on_socket_connected();
}

void cPositionVisualProtocol::on_socket_readyRead()
{
    //This is a one way protocol atm...
}

void cPositionVisualProtocol::on_socket_connected()
{
    open = true;
    std::cout<<"-> INFO <- cPositionVisualProtocol::on_socket_connected(): Connected to Socket on Port "<<port<<std::endl;
}

void cPositionVisualProtocol::on_socket_disconnected()
{
    open = false;
    socket->close();
    server->resumeAccepting();
    if(server->hasPendingConnections())
    {
        on_server_newConnection();
    }
}
