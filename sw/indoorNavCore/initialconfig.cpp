#include "initialconfig.h"


#define HANDYMAC "24:00:BA:53:10:EF"
#define WIFI_BEACON1_MAC "4A:D9:E7:6D:AB:34"
#define WIFI_BEACON2_MAC "4A:D9:E7:6D:B1:0D"
#define WIFI_BEACON3_MAC "4A:D9:E7:6D:A8:FE"
#define WIFI_UAVHOST_MAC "AC:9E:17:95:F1:C0"    //this is the asus router (has reference Power of -33dBm at 1m)
//#define WIFI_UAVHOST_MAC "00:12:BF:7B:F3:5D"  //this is our own router (hast reference Power of -28dBm at 1m with one antenna)

#define TESTMAC1 "00:00:00:00:00:00"
#define TESTMAC2 "00:00:00:00:00:01"
#define TESTMAC3 "00:00:00:00:00:02"
#define TESTMAC4 "00:00:00:00:00:03"



//For Wifi card
tBeaconResources beaconResources[] =
{
    /*MAC                       LATITUDE/X      LONGITUDE/Y         HEIGHT/Z    FilterType                  PAR1(windowSize)    referenceDistance       referencePower*/
    //for wifi card (laptop)
    {WIFI_BEACON1_MAC,        4.1,              1.6,                0,          { eFiltertypes::median,     10},                 1,                      -30.7},
    {WIFI_BEACON2_MAC,        10,               4.2,                0,          { eFiltertypes::median,     10},                 1,                      -30},
    {WIFI_BEACON3_MAC,        4.6,              15,                 0.5,         { eFiltertypes::median,     10},                 1,                      -30},
    {WIFI_UAVHOST_MAC,        3.1,              1.1,                0,          { eFiltertypes::median,     10},                 1,                      -28},

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
};


const uint32_t beaconNumber = (sizeof(beaconResources)/sizeof(tBeaconResources));

