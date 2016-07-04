#include "cpositionvisualizationgui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cPositionVisualizationGui w;
    w.show();

    return a.exec();
}
