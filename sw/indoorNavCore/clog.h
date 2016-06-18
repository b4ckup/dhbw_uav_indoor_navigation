#ifndef CLOG_H
#define CLOG_H

#include <QFile>
#include <QString>
#include <stdint.h>
#include <iostream>
#include <QDateTime>
#include <QTextStream>

#include "global.h"
#include "Eigen/Dense"

using namespace std;

class cLog
{
protected:
    QFile           *logFile;
    QTextStream     *logStream;


public:
    cLog(QString pathToLog);
    ~cLog();

    cLog &operator<<(char c);
    cLog &operator<<(QString c);
    cLog &operator<<(int c);
    cLog &operator<<(uint32_t c);
    cLog &operator<<(double c);
    cLog &operator<<(tPositionSolution solution);

    bool openLog(QString pathToLog);
    void closeLog();
    void flush();
};


#endif // CLOG_H
