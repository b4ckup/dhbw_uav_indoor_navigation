#ifndef CMEANFILTER_H
#define CMEANFILTER_H

#include "cfilter.h"

class cMeanFilter : public cFilter
{
private:
    uint windowSize;            //windowSize 0 means no window is used, the filterbuffer is cleared everytime getValue() is called
public:
    cMeanFilter(QString MAC, uint windowSize);
    ~cMeanFilter();

    void   enableLogfile(QString name = QString());

    cFilteredSignalParameter *getValue();
    void   addValue(cBeaconSignalParameter *par);
};

#endif // CMEANFILTER_H
