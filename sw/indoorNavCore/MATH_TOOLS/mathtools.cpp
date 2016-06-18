#include "mathtools.h"

//Coordinate system calculations from "Wendel - Integrierte Navigationssysteme" p. 32
Eigen::Matrix<double, 3, 1>mathtools::convertECFtoLLA(const Eigen::Matrix<double, 3, 1> &inPosECF)
{
   double ep;
   double r;
   double e2;
   double f;
   double g;
   double c;
   double s;
   double p;
   double q;
   double ro;
   double tmp;
   double u;
   double v;
   double zo;
   double x = inPosECF(0);
   double y = inPosECF(1);
   double z = inPosECF(2);

   Eigen::Matrix<double, 3, 1> outPosLLA;

   ep = WGS84_FIRST_ECCENTRICITY * (WGS84_EQUATOR_RADIUS / WGS84_POLAR_RADIUS);
   r = sqrt(x * x + y * y);
   e2 = WGS84_EQUATOR_RADIUS * WGS84_EQUATOR_RADIUS - WGS84_POLAR_RADIUS * WGS84_POLAR_RADIUS;
   f = 54.0 * WGS84_POLAR_RADIUS * WGS84_POLAR_RADIUS * z * z;
   g = r * r + (1.0 - WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY) * z * z - WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY * e2;
   c = (WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY * f * r * r) / (g * g * g);
   s = exp(log(fabs(1.0 + c + sqrt(c * c + 2.0 * c))) * 1.0 / 3.0);
   p = f / (3.0 * (s + 1.0 / s + 1.0) * (s + 1.0 / s + 1.0) * g * g);
   q = sqrt(1.0 + 2.0 * WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY * p);
   ro = -(p * WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY * r) / (1.0 + q) + sqrt((WGS84_EQUATOR_RADIUS * WGS84_EQUATOR_RADIUS / 2.0) * (1.0 + 1.0 / q)
         - (p * (1.0 - WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY) * z * z) / (q * (1.0 + q)) - p * r * r / 2.0);
   tmp = (r - WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY * ro) * (r - WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY * ro);
   u = sqrt(tmp + z * z);
   v = sqrt(tmp + (1.0 - WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY) * z * z );
   zo = (WGS84_POLAR_RADIUS * WGS84_POLAR_RADIUS * z) / (WGS84_EQUATOR_RADIUS * v);

   //outPosLLA(1) = atan2((z + ep * ep * zo), r);
   outPosLLA(0) = atan2((z +   zo * (WGS84_EQUATOR_RADIUS*WGS84_EQUATOR_RADIUS - WGS84_POLAR_RADIUS*WGS84_POLAR_RADIUS)/(WGS84_POLAR_RADIUS*WGS84_POLAR_RADIUS)), r);
   outPosLLA(1) = atan2(y, x);
   outPosLLA(2) = u * ( 1.0 - WGS84_POLAR_RADIUS * WGS84_POLAR_RADIUS / (WGS84_EQUATOR_RADIUS * v));

   return outPosLLA;
}

Eigen::Matrix<double, 3, 1>mathtools::convertECFtoLLAbow(const Eigen::Matrix<double, 3, 1> &inPosECF)
{
   double a 	= WGS84_EQUATOR_RADIUS;
   double b 	= WGS84_POLAR_RADIUS;
   double e 	= WGS84_FIRST_ECCENTRICITY;

   double x 	= inPosECF(0);
   double y 	= inPosECF(1);
   double z 	= inPosECF(2);

   double r;
   double alph;
   double n;

   Eigen::Matrix<double, 3, 1> outPosLLA;

   r	= sqrt(  x*x  +  y*y  );
   alph = atan( (a*z) / (b*r) );


   double sinAlph = sin(alph);
   double cosAlph = cos(alph);

   outPosLLA(0) = atan(  (z + (a*a * e*e * sinAlph*sinAlph*sinAlph)/b )
                        /(r - (  a * e*e * cosAlph*cosAlph*cosAlph)   ) );

   outPosLLA(1) = atan( y/x );

   double sinLat = sin(outPosLLA(1));
   double cosLat = cos(outPosLLA(1));

   n = WGS84_EQUATOR_RADIUS / sqrt (1.0 - (WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY) * sinLat * sinLat);

   outPosLLA(2) = r / cosLat - n;

   return outPosLLA;
}

Eigen::Matrix<double, 3, 1>mathtools::convertLLAtoECF(const Eigen::Matrix<double, 3, 1> &inPosLLA)
{
   double lat 	=  inPosLLA(0);//LLD_r_BE(1);
   double lon 	=  inPosLLA(1);//LLD_r_BE(2);
   double h 	=  inPosLLA(2);//-LLD_r_BE(3);
   double sinLat = sin(lat);
   double cosLat = cos(lat);
   double sinLon = sin(lon);
   double cosLon = cos(lon);
   double n;

   Eigen::Matrix<double, 3, 1> outPosECF;

   n = WGS84_EQUATOR_RADIUS / sqrt (1.0 - (WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY) * sinLat * sinLat);

   outPosECF(0) = (n + h) * cosLat * cosLon;
   outPosECF(1) = (n + h) * cosLat * sinLon;
   //outPosECF(3) = (n * (1.0 - WGS84_FIRST_ECCENTRICITY * WGS84_FIRST_ECCENTRICITY) + h) * sinLat;
   outPosECF(2) = (WGS84_POLAR_RADIUS_SQUARE/WGS84_EQUATOR_RADIUS_SQUARE * n + h) * sinLat;
   return outPosECF;
}
