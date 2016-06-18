#include "clistener.h"

cListener::cListener(QObject *parent) : QObject(parent)
{
    running = false;
}

cListener::~cListener()
{

}

void cListener::start()
{
    fd_set readSet;
    fd_set tmpSet;

    FD_ZERO(&readSet);
    FD_SET(STDIN, &readSet);

    running = true;

    while(running == true)
    {
        memcpy((char*)&tmpSet, (char*)&readSet, sizeof(readSet));
        select(STDIN+1, &tmpSet, NULL, NULL, NULL);
        if(FD_ISSET(STDIN, &tmpSet))
        {
            char line[PIPE_INPUT_BUFFERSIZE];
            while(fgets(line, PIPE_INPUT_BUFFERSIZE, stdin) != NULL)
            {
                cBeaconSignalParameter *par = NULL;
                QString type(strtok(line, " "));
                if((type != "dbm") && ((type != "rssi")))
                {
                    continue;
                }
                QString MAC(strtok(NULL, " "));
                char *stren = strtok(NULL, " ");
                double sigStrength = stod(stren);

                if(type == "rssi")
                {
                    par = new cBeaconSignalRssi;
                    par->setMac(MAC);
                    par->setValue(sigStrength);
                    emit readDeviceInfo(par);
                }
                else if(type == "dbm")
                {
                    par = new cBeaconSignalDbm;
                    par->setMac(MAC);
                    par->setValue(sigStrength);
                    emit readDeviceInfo(par);
                }
                else
                {
                    std::cout<<"-> WARNING <- cListener::start(): Unknown parameter type, this shoud not happen! Maybe you specified a new format but didn't add a case for it here?"<<std::endl;
                }
            }
        }
        else
        {

        }
    }
}

void cListener::on_terminate_rcvd()
{
    running = false;
}
