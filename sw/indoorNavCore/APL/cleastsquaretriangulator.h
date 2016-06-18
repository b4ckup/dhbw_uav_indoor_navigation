#ifndef CLEASTSQUARETRIANGULATOR_H
#define CLEASTSQUARETRIANGULATOR_H

#include "ctriangulator.h"

extern tBeaconResources beaconResources[];
extern const uint32_t beaconNumber;

class cLeastSquareTriangulator : public cTriangulator
{
    Q_OBJECT
private:
public:
    cLeastSquareTriangulator(double updateInterval_msec, cFilterControl *ctrl, cProtocolDriver *protocol);

    apl_error_code calculatePosition(tPositionSolution &solution);
};

#endif // CLEASTSQUARETRIANGULATOR_H
