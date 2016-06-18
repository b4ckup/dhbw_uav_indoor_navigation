#ifndef MATHTOOLS_H
#define MATHTOOLS_H

#include "constants.h"
#include "../Eigen/Dense"

namespace mathtools
{
    Eigen::Matrix<double, 3, 1>convertECFtoLLA(const Eigen::Matrix<double, 3, 1> &inPosECF);
    Eigen::Matrix<double, 3, 1>convertECFtoLLAbow(const Eigen::Matrix<double, 3, 1> &inPosECF);
    Eigen::Matrix<double, 3, 1>convertLLAtoECF(const Eigen::Matrix<double, 3, 1> &inPosLLA);
}

#endif // MATHTOOLS_H
