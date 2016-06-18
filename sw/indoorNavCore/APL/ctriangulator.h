#ifndef CTRIANGULATOR_H
#define CTRIANGULATOR_H


#include <QFile>
#include <QTimer>
#include <QObject>
#include <QVector>
#include <QTextStream>

#include "../clog.h"
#include "initialconfig.h"
#include "FPL/cfiltercontrol.h"

#include "../Eigen/Dense"
#include "../MATH_TOOLS/mathtools.h"

#include "../HAL/cprotocoldriver.h"

extern tBeaconResources beaconResources[];
extern const uint32_t beaconNumber;

class cTriangulator : public QObject
{
    Q_OBJECT
protected:
    cLog *log;
    QTimer timer;
    bool logging;
    cFilterControl *ctrl;
    cProtocolDriver *protocol;
    tPositionSolution lastSolution;
    Eigen::Matrix<double, 3, 1> startPosition;
    cFilteredSignalParameter* parameters[MAX_BEACON_NUM];


    virtual apl_error_code      calculatePosition(tPositionSolution &solution) = 0;
    cFilteredSignalParameter    *getParameter(QString MAC);

    void            calculateStartPosition();
    bool            getParameters();
    void            clearParameters();
    void            enableLogging();
    double          getSpeedOverGround(Eigen::MatrixXd pos, uint64_t current_param_time);       //output is in m/s!!!
    double          getTrackAngle(Eigen::MatrixXd pos);
    void calculateDOPs(Eigen::MatrixXd H, tPositionSolution &solution);

    tPositionSolution createPositionSolution(Eigen::Matrix<double,3 ,1> position, bool valid, uint beaconNumber, uint64_t gpstime, Eigen::MatrixXd H);
public:
    cTriangulator(double updateInterval_msec, cFilterControl *ctrl, cProtocolDriver *protocol);
    virtual ~cTriangulator();

    void start();
public slots:
    void on_timer_timeout();
};


#endif // CTRIANGULATOR_H
