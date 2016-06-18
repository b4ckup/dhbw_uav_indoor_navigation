#include "clistenerthread.h"

cListenerThread::cListenerThread(QObject *connectObj, QObject *parent) : QThread(parent)
{
    listener = NULL;
    this->connectObj = connectObj;
}

cListenerThread::~cListenerThread()
{
    quit();
    if(wait(1000) == false)
    {
        cout<<"-> ERROR <- cSyncThread::~cSyncThread(): Thread won't exit, terminate will be forced..."<<endl;
    }
}

cListener *cListenerThread::getListener()
{
    return listener;
}

void cListenerThread::run()
{
    listener = new cListener;

    QObject::connect(listener, SIGNAL(readDeviceInfo(cBeaconSignalParameter*)), connectObj, SLOT(on_beaconParameter_rcvd(cBeaconSignalParameter*)));
    cout<<"-> INFO <- cListenerThread::run(): Started listenerThread..."<<endl;
    listener->start();
    exec();

    delete listener;
}
