#ifndef CFILTER_H
#define CFILTER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QDateTime>
#include <QFileInfo>
#include "global.h"

class cFilter
{
protected:
    static int filterCnt;

    bool        verbose;        //this is to print added values and calculated filtered values to a logFile
    QFile       *logFile;       //this is the logFile
    QTextStream *logStream;     //this is the logStream

    int             filterNumber;
    QString         MAC;
    QVector <cBeaconSignalParameter*> values;
    eFiltertypes    typ;

public:
    cFilter(QString MAC, eFiltertypes typ);
    virtual ~cFilter();

    QString getMac() const;
    virtual cFilteredSignalParameter* getValue() = 0;
    virtual void   addValue(cBeaconSignalParameter *par);

    virtual void    enableLogfile(QString name = QString());
    void    clearValues();
    void    printValues(const QVector <cBeaconSignalParameter*> *val = NULL);
    double calculateSigma();

    cBeaconSignalParameter *copyBeaconParameter(cBeaconSignalParameter *par);
};

#endif // CFILTER_H
