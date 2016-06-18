#ifndef CLISTENER_H
#define CLISTENER_H

#include <QThread>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include "../global.h"

using namespace std;

class cListener : public QObject
{
    Q_OBJECT
private:
    bool running;
public:
    cListener(QObject *parent = NULL);
    ~cListener();

    void start();

signals:
    void readDeviceInfo(cBeaconSignalParameter *par);

public slots:
    void on_terminate_rcvd();
};

#endif // CLISTENER_H
