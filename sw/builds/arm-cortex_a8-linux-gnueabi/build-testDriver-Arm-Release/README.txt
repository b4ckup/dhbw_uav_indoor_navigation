BUILT WITH THIS CONFIG
cTestDriver.h
#define TESTMAC1 "00:00:00:00:00:00"
#define TESTMAC2 "00:00:00:00:00:01"
#define TESTMAC3 "00:00:00:00:00:02"
#define TESTMAC4 "00:00:00:00:00:03"
#define PROPAGATION_CONSTANT 2.2

cTestDriver.cpp:
tBeaconResources beaconResources[] =
{
    /*MAC                       LATITUDE            LONGITUDE           HEIGHT                  referenceDistance       referencePower*/
    {TESTMAC1,                  -1,                 1,                  1,                          1,                      -20},
    {TESTMAC2,                  -3,                 3,                  -3,                         1,                      -20},
    {TESTMAC3,                  8,                  -8,                 -5,                         1,                      -20},
    {TESTMAC4,                  8,                  4,                  0,                          1,                      -20},
};