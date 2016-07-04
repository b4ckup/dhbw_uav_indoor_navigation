#include "clog.h"


cLog::cLog(QString pathToLog)
{
    logFile         = NULL;
    logStream       = NULL;
    QString path    = LOGDIR;
    path += pathToLog + QDateTime::currentDateTime().toString("dd.MM.log");
    openLog(path);
}

cLog::~cLog()
{
    closeLog();
}


cLog & cLog::operator<<(char c)
{
    if(logStream != NULL)
    {
        *logStream<<c;
    }
    return *this;
}

cLog & cLog::operator<<(QString c)
{
    if(logStream != NULL)
    {
        *logStream<<c;
    }
    return *this;
}

cLog & cLog::operator<<(int c)
{
    if(logStream != NULL)
    {
        *logStream<<c;
    }
    return *this;
}

cLog & cLog::operator<<(uint32_t c)
{
    if(logStream != NULL)
    {
        *logStream<<c;
    }
    return *this;
}

cLog & cLog::operator<<(double c)
{
    if(logStream != NULL)
    {
        *logStream<<c;
    }
    return *this;
}

cLog &cLog::operator<<(tPositionSolution solution)
{
    if(logStream != NULL)
    {
        *logStream<<solution.gpstime<<LOG_DELIMITER<<solution.position(0)<<LOG_DELIMITER
                 <<solution.position(1)<<LOG_DELIMITER<<solution.position(2)<<LOG_DELIMITER
                <<solution.validity<<LOG_DELIMITER<<solution.quality.GDOP<<LOG_DELIMITER
               <<solution.quality.HDOP<<LOG_DELIMITER<<solution.quality.PDOP<<LOG_DELIMITER
              <<solution.quality.VDOP;

    }
   return *this;
}

bool cLog::openLog(QString pathToLog)
{
    logFile = new QFile(pathToLog);
    if(!logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        cout<<"-> ERROR <- cLog::openLog() reported an Error opening the log file\n ErrorString: "<<logFile->errorString().toStdString()<<"\n Note that no log file is open right now!"<<endl;
        logFile = NULL;
        logStream = NULL;
        return false;
    }
    else
    {
        logStream = new QTextStream(logFile);
        logStream->seek(0);
        *logStream<<"\n ################################################### \n LOG from "<<QDateTime::currentDateTime().toString("dd.MM.yy HH:mm")<<"\n";
        return true;
    }
}

void cLog::closeLog()
{
    if(logStream != NULL)
    {
        delete logStream;
    }
    if(logFile != NULL)
    {
        logFile->close();
        delete logFile;
    }
    logFile = NULL;
    logStream = NULL;
}

void cLog::flush()
{
    if(logStream != NULL)
    {
        logStream->flush();
    }
}
