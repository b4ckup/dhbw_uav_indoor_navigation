#include "cfilter.h"

int cFilter::filterCnt = 0;

cFilter::cFilter(QString MAC, eFiltertypes typ)
{
    this->filterNumber = filterCnt;
    filterCnt++;
    this->MAC       = MAC;
    this->typ       = typ;
    this->verbose   = false;
    this->logFile   = NULL;
    this->logStream = NULL;

}

cFilter::~cFilter()
{
    for(int i=0; i<values.size(); i++)
    {
        delete values[i];
    }
    if(logFile != NULL)
    {
        logFile->close();
        delete logFile;
    }
    if(logStream != NULL)
    {
        delete logStream;
    }

}

QString cFilter::getMac() const
{
    return MAC;
}

void cFilter::addValue(cBeaconSignalParameter *par)
{
    values.push_back(copyBeaconParameter(par));

    if(verbose == true)
    {
        *logStream<<FILT_ACT_ADD<<par->toString()<<endl;
    }
}

void cFilter::enableLogfile(QString name)
{
    int cnt = 0;
    QString logName = QString(LOGDIR) + "log_" + cFilteredSignalParameter::getStringFromType(typ) + "filter_" + QString::number(filterNumber) + "_" + QDateTime::currentDateTime().toString("dd.MM.yy") + "_" + name;
    QString tmp     = logName;

    QFileInfo *fileInfo = new QFileInfo(logName);
    while(fileInfo->exists())
    {
        tmp = logName + "_" + QString::number(cnt);
        delete fileInfo;
        fileInfo = new QFileInfo(tmp);
        cnt++;
    }
    logName = tmp;

    logFile = new QFile(logName);
    if(!logFile->open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        std::cout<<"-> ERROR <- cFilter::enableVerbosity(): Could not open logFile (filterNumber = "<<filterNumber<<"), errorString:"<<logFile->errorString().toStdString()<<std::endl;
        delete logFile;
        logFile = NULL;
    }
    else
    {
        verbose = true;
        logStream = new QTextStream(logFile);
        //Write header
        *logStream<<"#Filter log, created on "<<QDateTime::currentDateTime().toString("dd.MM.yy hh:mm")
                 <<" by "<<cFilteredSignalParameter::getStringFromType(typ)<<" filter, filterNumber="<<filterNumber<<", MAC="<<MAC<<"\n";
        *logStream<<"#Operation\tValue\tType\tTimestamp"<<endl;
    }
}

void cFilter::clearValues()
{
    values.clear();
}

void cFilter::printValues(const QVector <cBeaconSignalParameter*> *val)
{
    if(val == NULL)
    {
        val = &values;
    }

    std::cout<<"Filter values:\n";
    for(int i=0; i<val->size(); i++)
    {
        std::cout<<(*val)[i]->getValue()<<" | ";
    }
    std::cout<<std::endl;
}

double cFilter::calculateSigma()
{
    double sigma    = 0;
    double mean     = 0;
    //Calculate mean
    for(int i=0; i<values.size(); i++)
    {
        mean += values[i]->getValue();
    }
    mean = mean / values.size();
    //Calculate sigma
    for(int i=0; i<values.size(); i++)
    {
        sigma += pow(values[i]->getValue()-mean, 2.0);
    }
    sigma = sqrt(sigma / values.size());
    return sigma;
}

cBeaconSignalParameter *cFilter::copyBeaconParameter(cBeaconSignalParameter *par)
{
    cBeaconSignalParameter *cpy = NULL;
    switch(par->getType())
    {
    case eSignalParameterTypes::rssi:
    {
        cpy = new cBeaconSignalRssi;
        cpy->setMac(par->getMac());
        cpy->setValue(par->getValue());
        break;
    }
    case eSignalParameterTypes::dbm:
    {
        cpy = new cBeaconSignalDbm;
        cpy->setMac(par->getMac());
        cpy->setValue(par->getValue());
        break;
    }
    default:
    {
        std::cout<<"-> WARNING <- cFilter::copyBeaconParameter(): Invalid eSignalParamaterType value, did you add a new one but didn't put a case for the type here?"<<std::endl;
    }
    }
    return cpy;
}
