#ifndef CLEVENBERGMARQUARTTRIANGULATOR_H
#define CLEVENBERGMARQUARTTRIANGULATOR_H

#include <QVector>
#include "ctriangulator.h"
#include "Eigen/NonLinearOptimization"

class cLevenbergMarquartTriangulator : public cTriangulator
{
    Q_OBJECT
private:
    template<typename _Scalar, int NX = Eigen::Dynamic, int NY = Eigen::Dynamic>
    struct Functor
    {
    typedef _Scalar Scalar;
    enum {
        InputsAtCompileTime = NX,
        ValuesAtCompileTime = NY
    };
    typedef Eigen::Matrix<Scalar,InputsAtCompileTime,1> InputType;
    typedef Eigen::Matrix<Scalar,ValuesAtCompileTime,1> ValueType;
    typedef Eigen::Matrix<Scalar,ValuesAtCompileTime,InputsAtCompileTime> JacobianType;

    int m_inputs, m_values;

    Functor() : m_inputs(InputsAtCompileTime), m_values(ValuesAtCompileTime) {}
    Functor(int inputs, int values) : m_inputs(inputs), m_values(values) {}

    int inputs() const { return m_inputs; }
    int values() const { return m_values; }

    };

    struct MyFunctor : Functor<double>
    {
        double ranges[MAX_BEACON_NUM];

        MyFunctor(double ranges[]);
        int operator()(const Eigen::VectorXd &x, Eigen::VectorXd &fvec) const;

        int df(const Eigen::VectorXd &x, Eigen::MatrixXd &fjac) const;
    };

public:
    cLevenbergMarquartTriangulator(double updateInterval_msec, cFilterControl *ctrl, cProtocolDriver *protocol);

    apl_error_code calculatePosition(tPositionSolution &solution);
};

#endif // CLEVENBERGMARQUARTTRIANGULATOR_H
