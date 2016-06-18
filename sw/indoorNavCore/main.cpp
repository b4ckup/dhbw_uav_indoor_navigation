#include <QCoreApplication>
#include <QDebug>
#include "initialconfig.h"


#include "CL/clistenerthread.h"
#include "capplicationthread.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::locale::global(std::locale::classic());

    if(MAX_BEACON_NUM < beaconNumber)
    {
        cout<<"-> ERROR <- main(): MAX_BEACON_NUM < beaconNumber --> too many beacons defined, this will cause serious undefined behaviour!!"<<endl;
    }
    cApplicationThread *aplThread = new cApplicationThread;
    aplThread->start();

    sleep(1);

    cListenerThread *listenerThread = new cListenerThread(aplThread->getFilterControl());
    listenerThread->start();

    int k = a.exec();

    delete listenerThread;
    delete aplThread;
    return k;
}
