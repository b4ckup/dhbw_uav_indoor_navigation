#include "cmeanfilter.h"

cMeanFilter::cMeanFilter(QString MAC, uint windowSize) : cFilter(MAC, eFiltertypes::mean)
{
    this->windowSize = windowSize;
}

cMeanFilter::~cMeanFilter()
{

}

void cMeanFilter::enableLogfile(QString name)
{
    cFilter::enableLogfile(name);
    *logStream<<"#windowsize="<<windowSize<<endl;
}

cFilteredSignalParameter *cMeanFilter::getValue()
{
    cFilteredSignalParameter *val = NULL;

    if(values.size() != 0)
    {
        double meanRange = 0;
        for(int i=0; i<values.size(); i++)
        {
            meanRange += values[i]->getRange();
        }
        meanRange /= values.size();
        val = new cFilteredSignalParameter(MAC, eFiltertypes::mean, filterNumber, meanRange, calculateSigma());

        //print to Log
        if(verbose == true)
        {
            *logStream<<FILT_ACT_CALC<<val->toString()<<endl;
        }

        //clear buffer is windowSize == 0
        if(windowSize == 0)
        {
            clearValues();
        }
    }
    else
    {
        std::cout<<"-> WARNING <- cMeanFilter::getValue(): Filter contains no values! Please check if you configured the correct MAC (this Filter is for:"
                <<MAC.toStdString()<<", also check if the Beacon is powered and up!"<<std::endl;
    }

    return val;
}

void cMeanFilter::addValue(cBeaconSignalParameter *par)
{
    cFilter::addValue(par);

    if(windowSize > 0)
    {
        if((uint)values.size() > windowSize)
        {
            cBeaconSignalParameter *buf;    //Remove from vector first, then delete!!!
            buf=values[0];
            values.remove(0,1);
            delete buf;
        }
    }
}
