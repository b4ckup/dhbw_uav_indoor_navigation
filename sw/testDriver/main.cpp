#include <QCoreApplication>
#include "ctestdriver.h"

//Trajectory file must look like this: "x_coord,y_coord,z_coord"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cTestDriver testDriver(500, "linearTrajectory.csv", 3);
    testDriver.start();

    return a.exec();
}
