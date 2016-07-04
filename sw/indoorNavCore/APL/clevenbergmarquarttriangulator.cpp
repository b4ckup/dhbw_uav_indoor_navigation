#include "clevenbergmarquarttriangulator.h"

cLevenbergMarquartTriangulator::cLevenbergMarquartTriangulator(double updateInterval_msec, cFilterControl *ctrl, cProtocolDriver *protocol) : cTriangulator(updateInterval_msec, ctrl, protocol)
{

}

apl_error_code cLevenbergMarquartTriangulator::calculatePosition(tPositionSolution &solution)
{
    Eigen::MatrixXd H(beaconNumber, 3);
    Eigen::VectorXd position(3);
    H.setZero(beaconNumber,3);
    position.setZero(3,1);

    if(beaconNumber < 3)
    {
        cout<<"-> WARNING <- cTriangulator::cTriangulator(): beaconNumber < 3 --> no position calculation possible"<<endl;
    }
    else
    {
        bool parameters_valid   = true;


        //Check if all parameters are not NULL (NULL means the filter didn't have any values and returned NULL)
        for(uint i=0; i<beaconNumber; i++)
        {
            if(parameters[i] == NULL)
            {
                parameters_valid = false;
            }
        }

        //##############################################//
        // Iterative solution of non-linear equations   //
        //##############################################//
        if(parameters_valid == true)
        {
            double rads[beaconNumber];
            for(uint i1=0; i1<beaconNumber; i1++)
            {
                for(uint i2=0; i2<beaconNumber; i2++)
                {
                    if(parameters[i2]->getMac() == beaconResources[i1].MAC)
                    {
                        rads[i1] = parameters[i2]->getRange();
                    }
                }
            }

            MyFunctor functor(rads);
            Eigen::LevenbergMarquardt<MyFunctor, double> lm(functor);

            position(0) = startPosition(0);
            position(1) = startPosition(1);
            position(2) = startPosition(2);

            lm.iter = 150;
            Eigen::LevenbergMarquardtSpace::Status ret = lm.lmder1(position, TRIANGULATOR_CONV_TOL);
            functor.df(position, H);                                                                                //Jakobi Matrix for DOPS

#if(USE_XYZ_COORDINATES == false)
            solution.position = mathtools::convertECFtoLLA(position);
#endif
            //This sets all values of solution
            solution = createPositionSolution(position, true, beaconNumber, parameters[0]->getTime(), H);        //quality is currently not used

            //TODO check if lmder1 converged (state codes of >> ret <<)
//            if(converged == false)
//            {
//                    cout << "-> WARNING <- cTriangulator::calculatePosition(): Position from ranges did not converge."<<endl;
//                    solution.position.setZero(3, 1);
//                    solution.validity = false;
//                    return _DID_NOT_CONVERGE;
//            }
//            else
//            {
                //Write to log
                if(logging == true)
                {
                    *log<<solution<<"\n";
                    log->flush();
                }
//            }

        }
        else
        {
            solution = createPositionSolution(position, false, beaconNumber, 0, H);
            cout<<"-> INFO <- cTriangulator::calculatePosition(): Parameters contained one or more NULL pointers, which means one or more cFilter classes didn't have values"<<endl;
        }

    }

    return _OK;
}

cLevenbergMarquartTriangulator::MyFunctor::MyFunctor(double ranges[])
{
    m_inputs = 3;
    m_values = 4;
    for(uint i=0; i<beaconNumber; i++)
    {
        this->ranges[i] = ranges[i];
    }
}

int cLevenbergMarquartTriangulator::MyFunctor::operator()(const Eigen::VectorXd &x, Eigen::VectorXd &dvec) const
{
    Eigen::VectorXd vec_to_beacon(3);
    for(uint i=0; i<beaconNumber; i++)
    {
        vec_to_beacon(0) = beaconResources[i].latitude;
        vec_to_beacon(1) = beaconResources[i].longitude;
        vec_to_beacon(2) = beaconResources[i].height;
#if(USE_XYZ_COORDINATES == false)
                    vec_to_beacon = mathtools::convertLLAtoECF(vec_to_beacon);
#endif
        vec_to_beacon = vec_to_beacon - x;

        dvec(i) = ranges[i] - vec_to_beacon.norm();
    }
    return 0;
}

int cLevenbergMarquartTriangulator::MyFunctor::df(const Eigen::VectorXd &x, Eigen::MatrixXd &djac) const
{
    // Implement dy/dx = 2*x
    Eigen::VectorXd unit_vec_to_beacon(3);
    Eigen::VectorXd vec_to_beacon(3);

    for(uint i=0; i<beaconNumber; i++)
    {
        vec_to_beacon(0) = beaconResources[i].latitude;
        vec_to_beacon(1) = beaconResources[i].longitude;
        vec_to_beacon(2) = beaconResources[i].height;
        vec_to_beacon = vec_to_beacon - x;
        unit_vec_to_beacon = vec_to_beacon / vec_to_beacon.norm();

        for(int j=0; j<3; j++)
        {
            djac(i, j) = unit_vec_to_beacon(j);
        }
    }
    return 0;
}
