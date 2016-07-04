#include "cnmeaprotocoldriver.h"


cNmeaProtocolDriver::cNmeaProtocolDriver(QString device) : cProtocolDriver()
{
    ttyFileDescriptor = open(device.toStdString().c_str(), O_WRONLY);
    if(ttyFileDescriptor < 0)
    {
        std::cout<<"-> ERROR <- cProtocolDriver::cProtocolDriver(): Error opening tty device!"<<std::endl;
        opened = false;
    }
    else
    {
        opened = true;
    }
}

cNmeaProtocolDriver::~cNmeaProtocolDriver()
{
    if(opened == true)
    {
        close(ttyFileDescriptor);
    }
}

QString cNmeaProtocolDriver::buildGGAsentence(tPositionSolution solution)
{
    GeographicLib::Geoid geoid("egm96-5");
    QString sentence = "$GPGGA,";
    //Add UTC time
    QDateTime dateTime;
    dateTime.fromMSecsSinceEpoch(solution.gpstime);
    sentence += dateTime.toString("HHmmss") + ",";
    //Add latitude
    sentence += QString::number(solution.position(1)*100, 'f', 3) + ",N,";
    //Add longitude
    sentence += QString::number(solution.position(2)*100, 'f', 3) + ",E,";
    //Fix quality
    sentence += QString::number(FIX_GPS) + ",";
    //Number of satellites being tracked
    sentence += QString::number(solution.beaconNumber, 'f', 2) + ",";
    //HDOP
    sentence += QString::number(solution.quality.HDOP, 'f', 2) + ",";
    //Height above Geoid
    double heightAboveGeoid = geoid.ConvertHeight(solution.position(0), solution.position(1), solution.position(2), GeographicLib::Geoid::ELLIPSOIDTOGEOID);
    sentence += QString::number(heightAboveGeoid, 'f', 1) + ",M,";
    //Height of geoid above WGS84
    double geoidHeightAboveWGS84 = heightAboveGeoid - solution.position(2);
    sentence += QString::number(geoidHeightAboveWGS84, 'f', 1) + ",M,,";
    //Checksum
    sentence += "*47";
    return sentence;
}

QString cNmeaProtocolDriver::buildVTGsentence(tPositionSolution solution)
{
    //TODO implement track made good

    QString sentence = "$GPVTG,";
    //true track made good
    sentence += QString("000.0") + ",T,";                                                   //TODO implement this
    //magnetic track made good (degress)
    sentence += QString("000.0") + ",M,";                                                   //TODO implement this
    //speed over ground in !!knots!!
    sentence += QString::number(solution.speedOverGround*3.6*1.94384, 'f', 1) + ",N,";
    //speed over ground in km/h
    sentence += QString::number(solution.speedOverGround * 3.6, 'f', 1) + ",K";
    //checksum
    sentence += "*48";
    return sentence;
}

QString cNmeaProtocolDriver::buildRMCsentence(tPositionSolution solution)
{
    GeographicLib::MagneticModel mag("wmm2010");
    double Bx, By, Bz, H, F, I;
    QString sentence = "$GPRMC,";
    //Add UTC time
    QDateTime dateTime;
    dateTime.fromMSecsSinceEpoch(solution.gpstime);
    sentence += dateTime.toString("HHmmss") + ",";
    //Add active
    sentence += "A,";
    //Add latitude
    sentence += QString::number(solution.position(1)*100, 'f', 3) + ",N,";
    //Add longitude
    sentence += QString::number(solution.position(2)*100, 'f', 3) + ",E,";
    //Add speed over ground in !!knots!!
    sentence += QString::number(solution.speedOverGround*1.94384, 'f', 1) + ",";
    //Add track angle
    sentence += QString::number(solution.trackAngle, 'f', 1) + ",";
    //Add date
    sentence += dateTime.toString("DDMMyy") + ",";
    //Magnetic declination
    double declination;
    int t = dateTime.toString("yyyy").toInt();
    mag(t, solution.position(0), solution.position(1), solution.position(2), Bx, By, Bz);
    GeographicLib::MagneticModel::FieldComponents(Bx, By, Bz, H, F, declination, I);
    sentence += QString::number(declination) + ",W";
    //Checksum
    sentence += "*6A";
    return sentence;
}

bool cNmeaProtocolDriver::writePositionToFlightCtrl(tPositionSolution solution)
{
    int ret;
    bool k = true;
    QString rmcSentence, ggaSentence, vtgSentence;
    rmcSentence = buildRMCsentence(solution);
    vtgSentence = buildVTGsentence(solution);
    ggaSentence = buildGGAsentence(solution);

#if(BEAGLE_USE_TTY == true)                                                                          //TODO test this
    if(opened == true)
    {
        ret = write(ttyFileDescriptor, ggaSentence.toStdString().c_str(), ggaSentence.size());
        if(ret < 0)
        {
            k = false;
            cout<<"-> ERROR <- cNmeaProtocolDriver::writePositionToFlightCtrl(): Error writing to tty!"<<endl;
        }
        ret = write(ttyFileDescriptor, vtgSentence.toStdString().c_str(), ggaSentence.size());
        if(ret < 0)
        {
            k = false;
            cout<<"-> ERROR <- cNmeaProtocolDriver::writePositionToFlightCtrl(): Error writing to tty!"<<endl;
        }
        ret = write(ttyFileDescriptor, ggaSentence.toStdString().c_str(), ggaSentence.size());
        if(ret < 0)
        {
            k = false;
            cout<<"-> ERROR <- cNmeaProtocolDriver::writePositionToFlightCtrl(): Error writing to tty!"<<endl;
        }
    }
#else                                                                                                   //write to stdout if beaglebone is not used
    cout<<ggaSentence.toStdString()<<endl<<vtgSentence.toStdString()<<endl<<rmcSentence.toStdString()<<endl;
#endif
    return k;
}
