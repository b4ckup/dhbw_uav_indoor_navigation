#include "global.h"

cBeaconSignalParameter::cBeaconSignalParameter(eSignalParameterTypes typ)
{
    this->typ = typ;
    this->value = 0;
    this->MAC = "00:00:00:00:00:00";
    this->time = QDateTime::currentMSecsSinceEpoch();
}

cBeaconSignalParameter::~cBeaconSignalParameter()
{

}

double cBeaconSignalParameter::getValue() const
{
    return value;
}

eSignalParameterTypes cBeaconSignalParameter::getType() const
{
    return typ;
}

QString cBeaconSignalParameter::getMac() const
{
    return MAC;
}

uint64_t cBeaconSignalParameter::getTime() const
{
    return time;
}

void cBeaconSignalParameter::setValue(double val)
{
    this->value = val;
}

void cBeaconSignalParameter::setMac(QString MAC)
{
    this->MAC = MAC;
}

void cBeaconSignalParameter::setTime(uint64_t time)
{
    this->time = time;
}

bool cBeaconSignalParameter::operator<(const cBeaconSignalParameter &other) const
{
    return (getRange() < other.getRange());
}

bool cBeaconSignalParameter::operator>(cBeaconSignalParameter &other) const
{
    return (getRange() > other.getRange());
}

QString cBeaconSignalParameter::toString(char delimiter) const
{
    QString str;
    str = MAC + delimiter + QString::number(value) + delimiter + cBeaconSignalParameter::getStringFromType(typ) + delimiter + QString::number(time);
    return str;
}

QString cBeaconSignalParameter::getStringFromType(eSignalParameterTypes typ)
{
    QString str;
    switch(typ)
    {
    case eSignalParameterTypes::rssi:
    {
        str = "rssi";
        break;
    }
    case eSignalParameterTypes::dbm:
    {
        str = "dbm";
        break;
    }
    default:
    {
        str = "unknown";
    }
    }
    return str;
}




QString cFilteredSignalParameter::getStringFromType(eFiltertypes typ)
{
    QString str;
    switch(typ)
    {
    case median:
    {
        str = "median";
        break;
    }
    case mean:
    {
        str = "mean";
        break;
    }
    default:
    {
        str = "unknown";
    }
    }
    return str;
}

cFilteredSignalParameter::cFilteredSignalParameter(QString MAC, eFiltertypes typ, int filterNumber, double range, double sigma)
{
    this->typ               = typ;
    this->filterNumber      = filterNumber;
    this->range             = range;
    this->sigma             = sigma;
    this->time              = QDateTime::currentMSecsSinceEpoch();
    this->MAC               = MAC;
}

cFilteredSignalParameter::~cFilteredSignalParameter()
{

}

eFiltertypes cFilteredSignalParameter::getFilterType() const
{
    return typ;
}

QString cFilteredSignalParameter::toString(char delimiter) const
{
    QString str;
    str = MAC + delimiter + QString::number(range) + delimiter + cFilteredSignalParameter::getStringFromType(typ) + delimiter + QString::number(time);
    return str;
}

int cFilteredSignalParameter::getFilterNumber() const
{
    return filterNumber;
}

double cFilteredSignalParameter::getSigma() const
{
    return sigma;
}

double cFilteredSignalParameter::getRange() const
{
    return range;
}

QString cFilteredSignalParameter::getMac() const
{
    return MAC;
}

uint64_t cFilteredSignalParameter::getTime() const
{
    return time;
}
