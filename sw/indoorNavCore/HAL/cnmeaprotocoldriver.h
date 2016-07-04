#ifndef CNMEAPROTOCOLDRIVER_H
#define CNMEAPROTOCOLDRIVER_H

#include <GeographicLib/Geoid.hpp>
#include <GeographicLib/MagneticModel.hpp>
#include "cprotocoldriver.h"
using namespace std;

#define FIX_GPS 1
#define FIX_SIMULATION 8


class cNmeaProtocolDriver : public cProtocolDriver
{
public:
    cNmeaProtocolDriver(QString device);
    ~cNmeaProtocolDriver();
    QString buildGGAsentence(tPositionSolution solution);
    QString buildVTGsentence(tPositionSolution solution);
    QString buildRMCsentence(tPositionSolution solution);
    bool writePositionToFlightCtrl(tPositionSolution solution);
};

#endif // CNMEAPROTOCOLDRIVER_H
