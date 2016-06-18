#include "capplicationthread.h"

cApplicationThread::cApplicationThread(QObject *parent) : QThread(parent)
{
    filterControl = NULL;
}

cApplicationThread::~cApplicationThread()
{
    quit();
    if(wait(1000) == false)
    {
        cout<<"-> ERROR <- cSyncThread::~cSyncThread(): Thread won't exit, terminate will be forced..."<<endl;
    }
}

cFilterControl *cApplicationThread::getFilterControl()
{
    return filterControl;
}

void cApplicationThread::run()
{
    //Set up filter layer
    filterControl = new cFilterControl;
    filterControl->initFilters();

    //Set up protocolDriver in HAL
    protocolDriver = new cNmeaProtocolDriver(PROTOCOL_TTY);

    //Set up Application Layer

#if(USE_LEVENBERG_MARQUARDT==true)
    cTriangulator *lemTriang = new cLevenbergMarquartTriangulator(1000, filterControl, protocolDriver);
    lemTriang->start();
#elif(USE_LEASTSQUARE == true)
    cTriangulator *lsqTriang = new cLeastSquareTriangulator(1000, filterControl, protocolDriver);
    lsqTriang->start();
#endif
    cout<<"-> INFO <- cApplicationThread::run(): Started applicationThread..."<<endl;
    exec();

    delete filterControl;
#if(USE_LEVENBERG_MARQUARDT==true)
    delete lemTriang;
#elif(USE_LEASTSQUARE == true)
    delete lsqTriang;
#endif
}
