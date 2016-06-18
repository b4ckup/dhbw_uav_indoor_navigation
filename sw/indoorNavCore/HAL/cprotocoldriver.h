#ifndef CPROTOCOLDRIVER_H
#define CPROTOCOLDRIVER_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "global.h"

class cProtocolDriver
{
protected:
    int ttyFileDescriptor;
    bool opened;
public:
    cProtocolDriver(QString device);
    virtual ~cProtocolDriver();
    virtual bool writePositionToFlightCtrl(tPositionSolution) = 0;
};

#endif // CPROTOCOLDRIVER_H
