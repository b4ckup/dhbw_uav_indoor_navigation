#ifndef GLOBAL_H
#define GLOBAL_H

//Suppress these warnings caused by the Eigen Matrix library...
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wignored-attributes"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"

#include <QString>
#include <QDebug>
#include <iostream>
#include <QDateTime>
#include <math.h>
#include "Eigen/Dense"
#include "initialconfig.h"

//Switch MACROS
#define USE_LEASTSQUARE             false                       //enable least square with gauß-newton (atm only either lsq or lem supported)
#define USE_LEVENBERG_MARQUARDT     true                        //enable levenberg marquardt triangulator (atm only either lsq or lem supported)
#define BEAGLE_BONE_BUILD           true                        //enable this if you build a release for the beagle bone (this will enable output throug tty)
#define FILTER_LOGS_ENABLED         false                       //set true to enable logging for the filterLayer
#define TRIANGULATOR_LOG_ENABLED    true                        //set true to enable logging of calculated positions
#define TRIANGULATOR_STDOUT_POS     true                        //set if Triangulator should cout position
#define USE_XYZ_COORDINATES         false                       //set this false to use actual lla coordinate input and ouput, the xyz mode is for testing and debuggin
//Config Macros
#define PROTOCOL_TTY                "/dev/tty01"                //device to output protocol messages
#define LOGDIR                      "./logs/"                   //directory where logs are stored
#define TRIANGULATOR_MAX_ITERATIONS 100                         //max iterations for cTriangulators
#define MAX_BEACON_NUM              10                          //number of max beacons, mustn't be lower than beaconNumber is at runtime
#define BT_TX_POWER                 100                         //TX Power of bluetooth devices, needed to calculate range (this probably doesn't work too well!!!)
#define PIPE_INPUT_BUFFERSIZE       100                         //buffersize for one line that is read by cListener in CL
#define PROPAGATION_CONSTANT        2.2                         //propagation constant to get ranges from dbm
//General Macros
#define MACSIZE                     18                          //size of a MAC addr in ascii format
#define TRIANGULATOR_CONV_TOL       1e-3                        //tolerance that determines whether cTriangulator converged or not
#define STDOUT                      STDOUT_FILENO               //fileDescriptor number of stdout
#define STDIN                       STDIN_FILENO                //fileDescriptor number of stdin

//Some string macros used for logging...
#define FILT_ACT_ADD    "add\t\t"
#define FILT_ACT_CALC   "calc\t\t"
#define LOG_DELIMITER   ';'

//############################################################################################################################################
//used to dereference pointers and use less than operator (needed to perform cBeaconSignalParameter *par1 < cBeaconSignalParameter *par2)
template<class T>
bool dereferencedLessThan(T * o1, T * o2) {
    return *o1 < *o2;
}

//############################################################################################################################################
//Application Errorcodes
enum apl_error_code { _INVALID_SIG_PAR, _DID_NOT_CONVERGE, _ERROR = -1 , _OK = 0 };

//############################################################################################################################################
//Interface class to parse sensor measurements from CL to FPL
class cBeaconSignalParameter
{
public:
    cBeaconSignalParameter(eSignalParameterTypes typ = rssi);
    virtual ~cBeaconSignalParameter();

    double          getValue()      const;
    eSignalParameterTypes            getType()       const;
    QString         getMac()        const;
    uint64_t        getTime()       const;
    virtual double  getRange() const = 0;

    void            setValue(double val);
    void            setMac(QString MAC);
    void            setTime(uint64_t time);

    bool            operator<(const cBeaconSignalParameter &other) const;
    bool            operator>(cBeaconSignalParameter &other) const;

    QString         toString(char delimiter = '\t') const;
    static QString  getStringFromType(eSignalParameterTypes typ);
protected:
    QString MAC;
    double  value;
    eSignalParameterTypes    typ;
    uint64_t time;
};

//############################################################################################################################################
//Class to parse Rssi values from CL to FPL
class cBeaconSignalRssi : public cBeaconSignalParameter
{
public:
    cBeaconSignalRssi() : cBeaconSignalParameter(eSignalParameterTypes::rssi) {}
    ~cBeaconSignalRssi() {}

    double getRange() const
    {
        return pow(10,(value -(double)BT_TX_POWER)/(10*PROPAGATION_CONSTANT));
    }
private:
};

//############################################################################################################################################
//Class to parse rsii  values from CL to FPL
class cBeaconSignalDbm : public cBeaconSignalParameter
{
public:
    cBeaconSignalDbm() : cBeaconSignalParameter(eSignalParameterTypes::dbm) {}
    ~cBeaconSignalDbm() {}

    double getRange() const
    {
        int num = -1;
        for(uint i=0; i<beaconNumber; i++)
        {
            if(MAC == beaconResources[i].MAC)
            {
                num = i;
            }
        }
        return (double)beaconResources[num].referenceDistance * pow(10, ((double)beaconResources[num].referencePower - value)/(10*(double)PROPAGATION_CONSTANT));
    }
private:
};

//############################################################################################################################################
//Class to parse filtered values from FPL to APL
class cFilteredSignalParameter
{
public:

    static QString getStringFromType(eFiltertypes);
private:
    QString             MAC;
    eFiltertypes        typ;
    int                 filterNumber;
    double              range;
    double              sigma;
    uint64_t            time;
public:
    cFilteredSignalParameter(QString MAC, eFiltertypes typ, int filterNumber, double range, double sigma = -1); //sigma < 0 means not used
    virtual ~cFilteredSignalParameter();

    eFiltertypes    getFilterType() const;
    QString         toString(char delimiter = '\t') const;
    int             getFilterNumber() const;
    double          getSigma() const;
    double          getRange() const;
    QString         getMac() const;
    uint64_t        getTime() const;
};

//############################################################################################################################################
//Struct containing Dilution of Positions (like GPS DOPs)
struct tPositionQuality
{
    double                      VDOP;           //vertical DOP
    double                      HDOP;           //horizontal DOP
    double                      PDOP;           //position DOP
    double                      GDOP;           //geometric DOP
};

//############################################################################################################################################
//Struct to parse calculated postion solutions to the Protocol layer (e.g. NMEA Protocol driver)
struct tPositionSolution
{
    Eigen::Matrix<double, 3, 1> position;       //LLH!
    bool                        validity;
    tPositionQuality            quality;
    uint64_t                    gpstime;
    double                      trackAngle;     //Angle in which the device is moving
    double                      speedOverGround;//In m/s
    int                         beaconNumber;   //number of beacons that were used to calculate this position
};


#endif // GLOBAL_H
