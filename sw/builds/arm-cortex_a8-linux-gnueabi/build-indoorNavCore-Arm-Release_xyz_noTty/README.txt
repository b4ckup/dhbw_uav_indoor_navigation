BUILT WITH THESE CONFIGS
global.h:
//Switch MACROS
#define USE_LEASTSQUARE             false                       //enable least square with gauß-newton (atm only either lsq or lem supported)
#define USE_LEVENBERG_MARQUARDT     true                        //enable levenberg marquardt triangulator (atm only either lsq or lem supported)
#define BEAGLE_BONE_BUILD           false                       //enable this if you build a release for the beagle bone (this will enable output throug tty)
#define FILTER_LOGS_ENABLED         false                       //set true to enable logging for the filterLayer
#define TRIANGULATOR_LOG_ENABLED    true                        //set true to enable logging of calculated positions
#define TRIANGULATOR_STDOUT_POS     true                        //set if Triangulator should cout position
#define USE_XYZ_COORDINATES         true                        //set this false to use actual lla coordinate input and ouput, the xyz mode is for testing and debuggin

initialConfig.cpp:
tBeaconResources beaconResources[] =
{
    /*MAC                       LATITUDE/X      LONGITUDE/Y         HEIGHT/Z    FilterType                  PAR1(windowSize)    referenceDistance       referencePower*/

    //for wifi card (laptop)
    {WIFI_BEACON1_MAC,        4.1,              1.6,                1,          { eFiltertypes::median,     5},                 1,                      -25},
    {WIFI_BEACON2_MAC,        10,               4.2,                0,          { eFiltertypes::median,     5},                 1,                      -25},
    {WIFI_BEACON3_MAC,        4.6,              15,                 -1,          { eFiltertypes::median,    5},                 1,                      -25},
    {WIFI_UAVHOST_MAC,        3.1,              1.1,                0,          { eFiltertypes::median,     5},                 1,                      -28},

    //for ASUS antenna
//    {WIFI_BEACON1_MAC,        1.4,                4.61,           0,          { eFiltertypes::median,     3},                 1,                      98},
//    {WIFI_BEACON2_MAC,        5,                  0.27,           0,          { eFiltertypes::median,     3},                 1,                      98},
//    {WIFI_BEACON3_MAC,        7.06,               4.86,           0,          { eFiltertypes::median,     3},                 1,                      98},
//    {WIFI_UAVHOST_MAC,        4.56,               5.77,           0,          { eFiltertypes::median,     3},                 1,                      74},

    //for test driver
//    {TESTMAC1,                  -1,                 1,              1,          { eFiltertypes::mean,       5},                 1,                      -20},
//    {TESTMAC2,                  -3,                 3,              -3,         { eFiltertypes::mean,       5},                 1,                      -20},
//    {TESTMAC3,                  8,                  -8,             -5,         { eFiltertypes::mean,       5},                 1,                      -20},
//    {TESTMAC4,                  8,                  4,              0,          { eFiltertypes::mean,       5},                 1,                      -20},