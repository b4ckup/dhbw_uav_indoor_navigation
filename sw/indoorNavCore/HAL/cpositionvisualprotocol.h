#ifndef CPOSITIONVISUALPROTOCOL_H
#define CPOSITIONVISUALPROTOCOL_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "cprotocoldriver.h"
#include "../global.h"

class cPositionVisualProtocol : public QObject, public cProtocolDriver
{
    Q_OBJECT
private:
    QTcpServer *server;
    QTcpSocket *socket;
    int32_t port;
    bool open;
    void writeInitialInformation();
public:
    cPositionVisualProtocol(int32_t port, QObject *parent = NULL);
    ~cPositionVisualProtocol();

    bool writePositionToFlightCtrl(tPositionSolution);
private slots:
    void on_server_newConnection();
    void on_socket_readyRead();
    void on_socket_connected();
    void on_socket_disconnected();
};

#endif // CPOSITIONVISUALPROTOCOL_H
