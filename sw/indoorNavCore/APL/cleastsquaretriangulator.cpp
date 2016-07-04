#include "cleastsquaretriangulator.h"

cLeastSquareTriangulator::cLeastSquareTriangulator(double updateInterval_msec, cFilterControl *ctrl, cProtocolDriver *protocol) : cTriangulator(updateInterval_msec, ctrl, protocol)
{

}

apl_error_code cLeastSquareTriangulator::calculatePosition(tPositionSolution &solution) //The initialization of solution.position is important!!!
{
    if(beaconNumber < 3)
    {
        cout<<"-> WARNING <- cTriangulator::cTriangulator(): beaconNumber < 3 --> no position calculation possible"<<endl;
    }
    else
    {
        cFilteredSignalParameter *currentSignalParameter = NULL;

        //Anmerkung: "Eigen" linear algebra Bilbiothek fängt bei Eigen::Matrix Index 0 an zu zählen...
        Eigen::Matrix<double, 3, 1> beacon_llh;
        Eigen::Matrix<double, 3, 1> beacon_xyz;
        Eigen::Matrix<double, 3, 1> vec_to_beacon;
        Eigen::Matrix<double, 3, 1> unit_vec_to_beacon;
        Eigen::Matrix<double, 3, 1> dx;
        Eigen::Matrix<double, MAX_BEACON_NUM, 1> dy;
        Eigen::Matrix<double, 3, 3> V;                                  //Matrix to determine DOPS

        Eigen::Matrix<double, MAX_BEACON_NUM, 3> H;                     //Jakobi Matrix
        Eigen::Matrix<double, MAX_BEACON_NUM,MAX_BEACON_NUM> invR;      //inverse Variance matrix
        Eigen::Matrix<double, 3, 3> HTinvRH;

        double sigma                    = 0;
        double distance_to_beacon       = 0.0;
        int iterations, i               = 0;
        bool converged                  = false;
        bool parameters_valid           = true;

        beacon_llh.setZero(3, 1);
        beacon_xyz.setZero(3, 1);
        vec_to_beacon.setZero(3, 1);
        unit_vec_to_beacon.setZero(3, 1);
        dx.setZero(3, 1);
        dy.setZero(MAX_BEACON_NUM, 1);
        V.setZero(3, 3);
        H.setZero(MAX_BEACON_NUM, 3);
        invR.setZero(MAX_BEACON_NUM, MAX_BEACON_NUM);
        HTinvRH.setZero(3, 3);

        iterations = 0;
        converged = 0;

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
            while((iterations < TRIANGULATOR_MAX_ITERATIONS) && (converged == false))
            {
                for(uint num=0; num<beaconNumber; num++)
                {
                    //search the buffer for a signal parameter with the matching MAC
                    currentSignalParameter = getParameter(beaconResources[num].MAC);

                    if(currentSignalParameter == NULL)
                    {
                        cout<<"-> ERROR <- cTriangulator::calculatePosition(): No signal parameter found for MAC:"<<beaconResources[num].MAC.toStdString()<<" this must not happen!"<<endl;
                        solution.position.setZero(3, 1);
                        solution.validity = false;
                        return _INVALID_SIG_PAR;
                    }
                    //##############################################//
                    // Taylor series, determine Jacobi-Matrix H     //
                    //##############################################//

                    //Coordinates are currently parsed in ECEF !NOT! in LLA
                    beacon_llh(0) = beaconResources[num].latitude;
                    beacon_llh(1) = beaconResources[num].longitude;
                    beacon_llh(2) = beaconResources[num].height;

#if(USE_XYZ_COORDINATES == false)
                    beacon_xyz = mathtools::convertLLAtoECF(beacon_llh);
#else
                    beacon_xyz = beacon_llh;
#endif
                    vec_to_beacon = beacon_xyz - solution.position;
                    distance_to_beacon = vec_to_beacon.norm();

                    unit_vec_to_beacon = vec_to_beacon / distance_to_beacon;

                    dy(num) = currentSignalParameter->getRange() - distance_to_beacon;

//                    cout<<"range["<<num<<"]="<<currentSignalParameter->getRange()<<endl;  //Debug output
                    //cout<<"distance="<<distance_to_sat<<endl<<"dy="<<endl<<dy<<endl;      //Debug output

                    for(i = 0; i < 3; i++)
                    {
                            H(num, i) = -unit_vec_to_beacon(i);
                    }

//                    cout<<"unit_vec_to_beacon="<<endl<<unit_vec_to_beacon<<endl           //Debug output
//                       <<"range="<<beaconDistance[num]<<endl
//                      <<"vec_to_beacon="<<endl<<vec_to_beacon<<endl
//                     <<"beacon_xyz="<<endl<<beacon_xyz<<endl
//                    <<"distance_to_beacon="<<endl<<distance_to_beacon<<endl;

                    sigma = currentSignalParameter->getSigma();
                    if(sigma > 0)
                    {
                        invR(num, num) = 1.0 / (currentSignalParameter->getSigma() * currentSignalParameter->getSigma());     //use Variances for weighted least-square
                    }
                    else
                    {
                        invR(num, num) = 1.0;
                    }
                }

                //##############################################//
                // Calc weighted least-square-solution          //
                //##############################################//
                HTinvRH = H.transpose() * invR * H;

                dx = HTinvRH.inverse() * H.transpose() * invR * dy;


                //##############################################//
                // Improve estimation                           //
                //##############################################//

                solution.position = solution.position + dx;

//                cout<<"H="<<endl<<H<<endl<<"dx="<<endl<<dx<<endl<<"dy="<<endl                 //Debug output
//                    <<dy<<endl<<"solution.position="<<endl<<solution.position<<endl;

                if(dx.norm() < TRIANGULATOR_CONV_TOL)
                {
                        converged = true;
                }
                iterations++;

            }


#if(USE_XYZ_COORDINATES == false)
            solution.position = mathtools::convertECFtoLLA(solution.position);
#endif

            solution = createPositionSolution(solution.position, true, beaconNumber, parameters[0]->getTime(), H);       //quality is currently not used!

            if(converged == false)
            {
                    cout << "-> WARNING <- cTriangulator::calculatePosition(): Position from ranges did not converge."<<endl;
                    solution.position.setZero(3, 1);
                    solution.validity = false;
                    return _DID_NOT_CONVERGE;
            }
            else
            {
                //Write to log
                if(logging == true)
                {
                    *log<<solution<<"\n";
                    log->flush();
                }
            }



        }
        else
        {
            solution.validity = false;
            solution.position.setZero(3, 1);
            cout<<"-> INFO <- cTriangulator::calculatePosition(): Parameters contained one or more NULL pointers, which means one or more cFilter classes didn't have values"<<endl;
        }

    }

    return _OK;
}
