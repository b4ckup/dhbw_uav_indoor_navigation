#include "cmedianfilter.h"

cMedianFilter::cMedianFilter(QString MAC, uint windowSize) : cFilter(MAC, eFiltertypes::median)
{
    this->windowSize = windowSize;
}

cMedianFilter::~cMedianFilter()
{

}

void cMedianFilter::enableLogfile(QString name)
{
    cFilter::enableLogfile(name);
    *logStream<<"#windowsize="<<windowSize<<endl;
}

cFilteredSignalParameter *cMedianFilter::getValue()
{
    int size;
    cFilteredSignalParameter *val = NULL;

    //copy values
    QVector <cBeaconSignalParameter*> *valuesCpy = new QVector <cBeaconSignalParameter*> (values);
    size = valuesCpy->size();

    //sort
    qSort(valuesCpy->begin(), valuesCpy->end(), dereferencedLessThan<cBeaconSignalParameter>);

    //print
    //printValues(valuesCpy);

    //filter
    if(size == 0)
    {
        std::cout<<"-> WARNING <- cMedianFilter::getValue(): Filter contains no values!"<<std::endl;
    }
    else if(size%2 == 0)
    {
        //this is a equal number of elements
        double val1, val2;
        val1 = (*valuesCpy)[(int)size/2-1]->getRange();
        val2 = (*valuesCpy)[((int)size/2)]->getRange();
        val = new cFilteredSignalParameter(MAC, eFiltertypes::median, filterNumber, (val1+val2)/2, calculateSigma());
        //print to Log
        if(verbose == true)
        {
            *logStream<<FILT_ACT_CALC<<val->toString()<<endl;
        }

        //clear filter if windowsize == 0
        if(windowSize == 0)
        {
            clearValues();
        }
    }
    else
    {
        //this is a non equal number of elements
        val = new cFilteredSignalParameter(MAC, eFiltertypes::median, filterNumber, (*valuesCpy)[(int)size/2]->getRange(), calculateSigma());
    }


    delete valuesCpy;
    return val;
}

void cMedianFilter::addValue(cBeaconSignalParameter *par)
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


