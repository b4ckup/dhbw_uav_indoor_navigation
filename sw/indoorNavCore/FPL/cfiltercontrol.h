#ifndef CFILTERCONTROL_H
#define CFILTERCONTROL_H

#include <QObject>
#include <QVector>
#include <QList>
#include "cfilter.h"
#include "cmeanfilter.h"
#include "cmedianfilter.h"
#include "../initialconfig.h"

extern tBeaconResources beaconResources[];
extern const uint32_t beaconNumber;

class cFilterControl : public QObject
{
    Q_OBJECT
private:
    QVector <cFilter*> filters;

public:
    cFilterControl();
    ~cFilterControl();
    QList<cFilteredSignalParameter*>getValue(QString MAC);
    void                            addFilter(QString MAC, eFiltertypes typ, uint windowSize);
    QList<cFilter*>                 getFilters(QString MAC);
    void                            initFilters();
public slots:
    void on_beaconParameter_rcvd(cBeaconSignalParameter *par);
};

#endif // CFILTERCONTROL_H
