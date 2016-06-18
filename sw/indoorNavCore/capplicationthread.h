#ifndef CAPPLICATIONTHREAD_H
#define CAPPLICATIONTHREAD_H

#include <QThread>
#include "HAL/cprotocoldriver.h"
#include "HAL/cnmeaprotocoldriver.h"
#include "FPL/cfiltercontrol.h"
#include "APL/cleastsquaretriangulator.h"
#include "APL/clevenbergmarquarttriangulator.h"

class cApplicationThread : public QThread
{
    Q_OBJECT
private:
    cFilterControl  *filterControl;
    cProtocolDriver *protocolDriver;
public:
    cApplicationThread(QObject *parent = NULL);
    ~cApplicationThread();

    cFilterControl *getFilterControl();

    void run();
};

#endif // CAPPLICATIONTHREAD_H
