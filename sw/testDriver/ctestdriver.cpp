#include "ctestdriver.h"


tBeaconResources beaconResources[] =
{
    /*MAC                       LATITUDE            LONGITUDE           HEIGHT                  referenceDistance       referencePower*/
    //for wifi card (laptop)
//    {WIFI_BEACON1_MAC,          4.1,                1.6,                1,                          1,                      -25},
//    {WIFI_BEACON2_MAC,          10,                 4.2,                0,                          1,                      -25},
//    {WIFI_BEACON3_MAC,          4.6,                15,                 -1,                         1,                      -25},
//    {WIFI_UAVHOST_MAC,          3.1,                1.1,                0,                          1,                      -22},
    //
    {TESTMAC1,                  -1,                 1,                  1,                          1,                      -20},
    {TESTMAC2,                  -3,                 3,                  -3,                         1,                      -20},
    {TESTMAC3,                  8,                  -8,                 -5,                         1,                      -20},
    {TESTMAC4,                  8,                  4,                  0,                          1,                      -20},
};


uint beaconResourceNum = sizeof(beaconResources)/sizeof(tBeaconResources);


cTestDriver::cTestDriver(int interval_msecs, QString trajectory_file, double sigma)
{
    trajectoryLogFile   = NULL;
    trajectoryLogStream = NULL;
    timer.setInterval(interval_msecs);
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
    this->variance = sigma * sigma;

    //open trajectory file
    trajFile = new QFile(trajectory_file);
    if(!trajFile->open(QIODevice::ReadOnly))
    {
        cout<<"-> ERROR <- cTestDriver(): Cannot open trajectory file : "<<trajFile->errorString().toStdString()<<endl;
        trajFile = NULL;
        trajStream = NULL;
    }
    else
    {
        trajStream = new QTextStream(trajFile);
    }

    //open noise file
    noiseFile = new QFile(GAUSSIAN_NOISE_FILE);
    if(!noiseFile->open(QIODevice::ReadOnly))
    {
        cout<<"-> ERROR <- cTestDriver(): Cannot open gaussian noise file : "<<noiseFile->errorString().toStdString()<<endl;
        noiseFile = NULL;
        noiseStream = NULL;
    }
    else
    {
        noiseStream = new QTextStream(noiseFile);
    }

    //open trajectoryLogFile
    if(ENABLE_TRAJECTORY_LOG == true)
    {
        trajectoryLogFile = new QFile(QString(LOGDIR) + QString(TRAJECTORY_LOG_FILENAME));
        if(!trajectoryLogFile->open(QIODevice::WriteOnly | QIODevice::Append))
        {
            cout<<"-> ERROR <- cTestDriver(): Cannot open trajectory log file noise file : "<<trajectoryLogFile->errorString().toStdString()<<endl;
            trajectoryLogFile   = NULL;
            trajectoryLogStream = NULL;
        }
        else
        {
            trajectoryLogStream = new QTextStream(trajectoryLogFile);
            *trajectoryLogStream<<"\n ################################################### \n LOG from "<<QDateTime::currentDateTime().toString("dd.MM.yy HH:mm")<<"\n";
        }
    }
}

cTestDriver::cTestDriver(int interval_msecs, double pos_x, double pos_y, double pos_z, double sigma)
{
    trajectoryLogFile   = NULL;
    trajectoryLogStream = NULL;

    position(0) = pos_x;
    position(1) = pos_y;
    position(2) = pos_z;

    trajFile    = NULL;
    trajStream  = NULL;


    timer.setInterval(interval_msecs);
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
    this->variance = sigma * sigma;

    //open noise file
    noiseFile = new QFile(GAUSSIAN_NOISE_FILE);
    if(!noiseFile->open(QIODevice::ReadOnly))
    {
        cout<<"-> ERROR <- cTestDriver(): Cannot open gaussian noise file : "<<noiseFile->errorString().toStdString()<<endl;
    }
    else
    {
        noiseStream = new QTextStream(noiseFile);
    }

    //open trajectoryLogFile
    if(ENABLE_TRAJECTORY_LOG == true)
    {
        trajectoryLogFile = new QFile(QString(LOGDIR) + QString(TRAJECTORY_LOG_FILENAME));
        if(!trajectoryLogFile->open(QIODevice::WriteOnly | QIODevice::Append))
        {
            cout<<"-> ERROR <- cTestDriver(): Cannot open trajectory log file noise file : "<<trajectoryLogFile->errorString().toStdString()<<endl;
            trajectoryLogFile   = NULL;
            trajectoryLogStream = NULL;
        }
        else
        {
            trajectoryLogStream = new QTextStream(trajectoryLogFile);
        }
    }

}

cTestDriver::~cTestDriver()
{
    if(noiseFile != NULL)
    {
        noiseFile->close();
        delete noiseFile;
    }
    if(noiseStream != NULL)
    {
        delete noiseStream;
    }
    if(trajFile != NULL)
    {
        trajFile->close();
        delete trajFile;
    }
    if(trajStream != NULL)
    {
        delete trajStream;
    }
}

void cTestDriver::start()
{
    timer.start();
}

void cTestDriver::on_timer_timeout()
{
    QString tmp;
    double factor;
    double wgn;
    double power;
    Eigen::Matrix<double, 3,1> beaconPos;
    Eigen::Matrix<double, 3, 1> vecToBeacon;

    if(trajStream != NULL)
    {
        if(trajStream->atEnd())
        {
            trajStream->seek(0); //rewind if at End
        }
        tmp = trajStream->readLine();
        position(0) = tmp.section(",",0,0).toDouble();
        position(1) = tmp.section(",",1,1).toDouble();
        position(2) = tmp.section(",",2,2).toDouble();
        if(trajectoryLogStream != NULL)
        {
            *trajectoryLogStream<<QDateTime::currentMSecsSinceEpoch()<<";"<<position(0)<<";"<<position(1)<<";"<<position(2)<<"\n";
            trajectoryLogStream->flush();
        }
    }

    for(uint i=0; i<beaconResourceNum; i++)
    {
        beaconPos(0) = beaconResources[i].latitude;
        beaconPos(1) = beaconResources[i].longitude;
        beaconPos(2) = beaconResources[i].height;
        vecToBeacon = beaconPos - position;

        if(noiseStream->atEnd())
        {
            noiseStream->seek(0);
        }
        tmp = noiseStream->readLine();

        wgn = tmp.toDouble();
        factor = sqrt(variance / GAUSSIAN_NOISE_VARIANCE);
        wgn = wgn * factor;

        power = beaconResources[i].referencePower - 10* PROPAGATION_CONSTANT * log10(vecToBeacon.norm() / beaconResources[i].referenceDistance);
        int p = rint(power+wgn);
        tmp = QString::number(p);
        cout<<"dbm "<<beaconResources[i].MAC.toStdString()<<" "<<tmp.toStdString()<<endl;
    }
}
