#include "ctriangulator.h"


cTriangulator::cTriangulator(double updateInterval_msec, cFilterControl *ctrl, cProtocolDriver *protocol)
{
    log             = NULL;
    logging         = false;
    this->ctrl      = ctrl;
    this->protocol  = protocol;
    timer.setInterval(updateInterval_msec);


    //init lastSolution
    lastSolution.position(0)        = NAN;
    lastSolution.position(1)        = NAN;
    lastSolution.position(2)        = NAN;
    lastSolution.gpstime            = 0;
    lastSolution.trackAngle         = 0;
    lastSolution.beaconNumber       = 0;
    lastSolution.speedOverGround    = 0;
    lastSolution.validity           = false;
    memset((char*)&lastSolution.quality, 0, sizeof(lastSolution.quality));

    //Determine startPosition by calculating centroid of polygon
    calculateStartPosition();
    cout<<"-> INFO <- cTriangulator::cTriangulator(): startPosition for leastSquare ="<<startPosition(0)<<" "<<startPosition(1)<<" "<<startPosition(2)<<endl;

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));

    if(MAX_BEACON_NUM < beaconNumber)
    {
        cout<<"-> ERROR <- cTriangulator::cTriangulator(): MAX_BEACON_NUM < beaconNumber --> too many beacons defined, this will cause serious undefined behaviour!!"<<endl;
    }
    if(beaconNumber < 3)
    {
        cout<<"-> WARNING <- cTriangulator::cTriangulator(): beaconNumber < 3 --> no position calculation possible"<<endl;
    }

    for(int i=0; i<MAX_BEACON_NUM; i++)
    {
        parameters[i] = NULL;
    }

    if(TRIANGULATOR_LOG_ENABLED == true)
    {
        enableLogging();
    }
}

cTriangulator::~cTriangulator()
{
    if(log != NULL)
    {
        delete log;
    }
    clearParameters();
}

void cTriangulator::start()
{
    timer.start();
}

void cTriangulator::on_timer_timeout()
{
    tPositionSolution solution;
    if(!getParameters())
    {
        cout<<"-> ERROR <- cTriangulator::on_timer_timeout(): getParamteres() returned false!"<<endl;
    }
    else
    {
        solution.position = startPosition;
        if(calculatePosition(solution) == _DID_NOT_CONVERGE)
        {
            solution = lastSolution; //send last solution if it's not possible to calculate one at the moment
            cout<<"-> ERROR <- cTriangulator::on_timer_timeout(): calculatePosition returned _DID_NOT_CONVERGE, cannot use this solution!!"<<endl;

            //if leastsquare didn't converge reset start position
            calculateStartPosition();
        }
        else
        {
            //new start position is the current position
            startPosition = solution.position;
        }
        clearParameters();
    }

    if(solution.validity)
    {
        lastSolution = solution;
        protocol->writePositionToFlightCtrl(solution);
    }
    else
    {
        cout<<"-> WARNING <- cTriangulator::on_timer_timeout(): Calculated solution is invalid"<<endl;
    }
#if(TRIANGULATOR_STDOUT_POS == true)
    cout<<"The estimated position is:"<<endl<<solution.position<<endl;
#endif
}

bool cTriangulator::getParameters()
{
    bool k = true;
    for(uint i=0; i<beaconNumber; i++)
    {
        //TODO atm element 0 of the QList is used, the QList is unnecessary for the application but is good for testing different filters for the same MAC
        QList<cFilteredSignalParameter*> pars = ctrl->getValue(beaconResources[i].MAC);
        if(!pars.isEmpty())
        {
            parameters[i] = pars[0];
            if(parameters[i] == NULL)
            {
                k = false;
            }
        }
    }
    return k;
}

void cTriangulator::clearParameters()
{
    for(int i=0; i<MAX_BEACON_NUM; i++)
    {
        if(parameters[i] != NULL)
        {
            delete parameters[i];
            parameters[i] = NULL;
        }
    }
}

void cTriangulator::enableLogging()
{
    logging = true;
    log = new cLog("leastSquare");
}

void cTriangulator::calculateStartPosition()
{
    Eigen::Matrix<double, 3, 1> beaconPos;
    startPosition.setZero(3,1);
    for(uint i=0; i<beaconNumber; i++)
    {
        beaconPos(0) = beaconResources[i].latitude;
        beaconPos(1) = beaconResources[i].longitude;
        beaconPos(2) = beaconResources[i].height;
        startPosition = startPosition + beaconPos;
    }
    startPosition = startPosition / beaconNumber;
}

double cTriangulator::getSpeedOverGround(Eigen::MatrixXd pos, uint64_t current_param_time)
{
    double velocity;
    Eigen::Matrix<double, 3, 1> v;
    if(lastSolution.validity == true)
    {
        v = pos - lastSolution.position;
        v = v / ((current_param_time - lastSolution.gpstime)*1000);
        velocity = v.norm();
    }
    else
    {
        velocity = 0;
    }
    return velocity;
}

double cTriangulator::getTrackAngle(Eigen::MatrixXd pos)
{
    //TODO implement this
    return NAN;
}

void cTriangulator::calculateDOPs(Eigen::MatrixXd H, tPositionSolution &solution)
{
    Eigen::MatrixXd V(3,3);
    V = H.transpose() * H;
    V = V.inverse().eval();                                                             //call eval() to prevent aliasing

    solution.quality.VDOP = sqrt(V(2,2));
    solution.quality.HDOP = sqrt(V(0,0) + V(1,1));
    solution.quality.PDOP = sqrt(V(0,0) + V(1,1) + V(2,2));
    solution.quality.GDOP = solution.quality.PDOP;
}

tPositionSolution cTriangulator::createPositionSolution(Eigen::Matrix<double,3 ,1> position, bool valid, uint beaconNumber, uint64_t gpstime, Eigen::MatrixXd H)
{
    tPositionSolution solution;
    solution.position(0)    = position(0);
    solution.position(1)    = position(1);
    solution.position(2)    = position(2);
    calculateDOPs(H, solution);
    solution.gpstime        = parameters[0]->getTime();
    solution.validity       = valid;
    solution.beaconNumber   = beaconNumber;
    solution.trackAngle     = getTrackAngle(position);
    solution.speedOverGround= getSpeedOverGround(position, gpstime);

    return solution;
}

cFilteredSignalParameter *cTriangulator::getParameter(QString MAC)
{
    cFilteredSignalParameter *par = NULL;
    for(uint i=0; i<beaconNumber; i++)
    {
        if(parameters[i] != NULL)
        {
            if(parameters[i]->getMac() == MAC)
            {
                par = parameters[i];
            }
        }
    }
    return par;
}


