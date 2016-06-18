#ifndef CTESTDRIVER_H
#define CTESTDRIVER_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <math.h>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "Eigen/Dense"
using namespace std;

#define PROPAGATION_CONSTANT 2.2
#define HANDYMAC "24:00:BA:53:10:EF"
#define WIFI_BEACON1_MAC "4A:D9:E7:6D:AB:34"
#define WIFI_BEACON2_MAC "4A:D9:E7:6D:B1:0D"
#define WIFI_BEACON3_MAC "4A:D9:E7:6D:A8:FE"
#define WIFI_UAVHOST_MAC "AC:9E:17:95:F1:C0"    //this is the logitech router (has reference Power of -33dBm at 1m)
//#define WIFI_UAVHOST_MAC "00:12:BF:7B:F3:5D"  //this is our own router (hast reference Power of -22dBm at 1m)

#define TESTMAC1 "00:00:00:00:00:00"
#define TESTMAC2 "00:00:00:00:00:01"
#define TESTMAC3 "00:00:00:00:00:02"
#define TESTMAC4 "00:00:00:00:00:03"

#define LOGDIR "logs/"
#define GAUSSIAN_NOISE_VARIANCE 0.97
#define GAUSSIAN_NOISE_FILE "gaussianNoise.csv"
#define ENABLE_TRAJECTORY_LOG true
#define TRAJECTORY_LOG_FILENAME "testDriver_trajectoryLog.txt"

struct tBeaconResources
{
    QString             MAC;
    double              latitude;           //WGS82!
    double              longitude;          //WGS82!
    double              height;             //WGS82!
    double              referenceDistance;  //[m]
    double              referencePower;     //Power [dBm] measured at referenceDistance
};

extern tBeaconResources beaconResources[];
extern uint beaconResourceNum;

class cTestDriver : public QObject
{
    Q_OBJECT
private:
    QTimer timer;
    double variance;

    QFile *trajFile;
    QFile *noiseFile;
    QTextStream *trajStream;
    QTextStream *noiseStream;
    QFile *trajectoryLogFile;
    QTextStream *trajectoryLogStream;
    Eigen::Matrix<double,3,1> position;
public:
    cTestDriver(int interval_msecs, QString trajectory_file, double sigma);
    cTestDriver(int interval_msecs,double pos_x, double pos_y, double pos_z, double sigma);
    ~cTestDriver();
    void start();
public slots:
    void on_timer_timeout();
};

#endif // CTESTDRIVER_H
