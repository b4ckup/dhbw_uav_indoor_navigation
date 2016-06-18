#include "cprotocoldriver.h"

cProtocolDriver::cProtocolDriver(QString device)
{
    ttyFileDescriptor = open(device.toStdString().c_str(), O_WRONLY);
    if(ttyFileDescriptor < 0)
    {
        std::cout<<"-> ERROR <- cProtocolDriver::cProtocolDriver(): Error opening tty device!"<<std::endl;
        opened = false;
    }
    else
    {
        opened = true;
    }
}

cProtocolDriver::~cProtocolDriver()
{
    if(opened == true)
    {
        close(ttyFileDescriptor);
    }
}
