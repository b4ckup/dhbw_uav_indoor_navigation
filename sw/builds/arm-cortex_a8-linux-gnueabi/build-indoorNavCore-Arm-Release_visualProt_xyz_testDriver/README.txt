BUILT WITH THIS CONFIG
global.h:
//Switch MACROS
#define USE_LEASTSQUARE             false                       //enable least square with gauß-newton (atm only either lsq or lem supported)
#define USE_LEVENBERG_MARQUARDT     true                        //enable levenberg marquardt triangulator (atm only either lsq or lem supported)
#define USE_VISUALPROTOCOL          true                        //enables cVisualProtocolDriver to get data over TCP/IP to positionVisualizationGui (this will disable TTY output atm though)
#define USE_NMEA_TTY_OUTPUT         false                       //this will enable output through the dev specified in PROTOCOL_TTY (see below)
#define FILTER_LOGS_ENABLED         false                       //set true to enable logging for the filterLayer
#define TRIANGULATOR_LOG_ENABLED    true                        //set true to enable logging of calculated positions
#define TRIANGULATOR_STDOUT_POS     true                        //set if Triangulator should cout position
#define USE_XYZ_COORDINATES         true                        //set this false to use actual lla coordinate input and ouput, the xyz mode is for testing and debuggin

initialConfig.cpp
tBeaconResources beaconResources[] =
{
    /*MAC                       LATITUDE/X      LONGITUDE/Y         HEIGHT/Z    FilterType                  PAR1(windowSize)    referenceDistance       referencePower*/
    //for test driver
    {TESTMAC1,                  -1,                 1,              1,          { eFiltertypes::mean,       5},                 1,                      -20},
    {TESTMAC2,                  -3,                 3,              -3,         { eFiltertypes::mean,       5},                 1,                      -20},
    {TESTMAC3,                  8,                  -8,             -5,         { eFiltertypes::mean,       5},                 1,                      -20},
    {TESTMAC4,                  8,                  4,              0,          { eFiltertypes::mean,       5},                 1,                      -20},
};