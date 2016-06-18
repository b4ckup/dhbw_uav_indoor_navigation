#include "cfiltercontrol.h"

cFilterControl::cFilterControl()
{

}

cFilterControl::~cFilterControl()
{
    for(int i=0; i<filters.size(); i++)
    {
        delete filters[i];
    }
}

QList<cFilteredSignalParameter*> cFilterControl::getValue(QString MAC)
{
    cFilteredSignalParameter *buf;
    QList <cFilter*> filters = getFilters(MAC);
    QList <cFilteredSignalParameter*> values;
    for(int i=0; i<filters.size(); i++)
    {
        buf = filters[i]->getValue();
        if(buf != NULL)
        {
            values.push_back(buf);
        }
    }
    return values;
}

void cFilterControl::addFilter(QString MAC, eFiltertypes typ, uint par1)
{
    cFilter *filter;
    //TODO check if dublicate?
    switch(typ)
    {
    case eFiltertypes::median:
    {
        filter = new cMedianFilter(MAC, par1);
        break;
    }
    case eFiltertypes::mean:
    {
        filter = new cMeanFilter(MAC, par1);
        break;
    }
    default:
    {
        cout<<"-> WARNING <- cFilterControl::addFilter(): Invalid eSignalParamaterType value, did you add a new one but didn't put a case for the type here?"<<endl;
    }
    }
    filters.push_back(filter);

    cout<<"-> INFO <- cFilterControl::addFilter(): Added filter for MAC"<<MAC.toStdString()<<endl;
}

QList<cFilter *> cFilterControl::getFilters(QString MAC)
{
    QList<cFilter*> fts;
    for(int i=0; i<filters.size(); i++)
    {
        if(MAC == filters[i]->getMac())
        {
            fts.push_back(filters[i]);
        }
    }
    return fts;
}

void cFilterControl::initFilters()
{
    for(uint i=0; i<beaconNumber; i++)
    {
        addFilter(beaconResources[i].MAC, beaconResources[i].filterConfig.typ, beaconResources[i].filterConfig.par1);
    }

    if(FILTER_LOGS_ENABLED == true)
    {
        std::cout<<"-> INFO <- cFilterControl::initFilters(): Enabling logFiles for all filters (FILTER_LOGS_ENABLED is defined true!)"<<std::endl;
        for(int i=0; i<filters.size(); i++)
        {
            filters[i]->enableLogfile();
        }
    }

}

void cFilterControl::on_beaconParameter_rcvd(cBeaconSignalParameter *par)
{
    //Add value to the filters with the right MAC
    QList <cFilter*> filters = getFilters(par->getMac());
    if(filters.size() != 0)
    {
        for(int i=0; i<filters.size(); i++)
        {
            filters[i]->addValue(par);
        }
    }
    else
    {
        //std::cout<<"-> INFO <- cFilterControl::on_beaconParameter_rcvd(): Receiving signal from MAC"<<par->getMac().toStdString()<<" with no associated Filter"<<std::endl;
    }
    delete par;
}
