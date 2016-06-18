#ifndef INITIALCONFIG_H
#define INITIALCONFIG_H

#include <QString>

enum eFiltertypes { none = -1, mean, median};
enum eSignalParameterTypes {rssi, dbm, range};

struct tFilterResources
{
    eFiltertypes   typ;
    uint            par1;
};

struct tBeaconResources
{
    QString             MAC;
    double              latitude;           //WGS82!
    double              longitude;          //WGS82!
    double              height;             //WGS82!
    tFilterResources    filterConfig;
    double              referenceDistance;  //[m]
    double              referencePower;     //Power [dBm] measured at referenceDistance
};

extern const uint32_t beaconNumber;
extern tBeaconResources beaconResources[];

#endif // INITIALCONFIG_H
