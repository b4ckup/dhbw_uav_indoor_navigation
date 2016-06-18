#ifndef CMEDIANFILTER_H
#define CMEDIANFILTER_H

#include "cfilter.h"
using namespace std;

class cMedianFilter : public cFilter
{
private:
    uint   windowSize;      //windowSize 0 means no window is used, the filterbuffer is cleared everytime getValue() is called
public:
    cMedianFilter(QString MAC, uint windowSize);
    ~cMedianFilter();

    void   enableLogfile(QString name = QString());
    cFilteredSignalParameter *getValue();
    void   addValue(cBeaconSignalParameter *par);
};

#endif // CMEDIANFILTER_H
