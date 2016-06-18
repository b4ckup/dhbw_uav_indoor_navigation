#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>

// general
    const double PI 			= M_PI; 			// Ratio of the circumference to the diameter of a circle

// WGS84
    const double WGS84_EQUATOR_RADIUS 			= 6378137.0;			// Earth Ellipsoid Semi Major Axis (m)
    const double WGS84_EQUATOR_RADIUS_SQUARE	= WGS84_EQUATOR_RADIUS * WGS84_EQUATOR_RADIUS;
    const double WGS84_POLAR_RADIUS 			= 6356752.31424518;  	// Earth Ellipsoid Semi Minor Axis (m)
    const double WGS84_POLAR_RADIUS_SQUARE		= WGS84_POLAR_RADIUS * WGS84_POLAR_RADIUS;

    const double WGS84_ELLIPSOID_FLATTENING 	= 1.0 - (WGS84_POLAR_RADIUS/WGS84_EQUATOR_RADIUS);
    const double WGS84_FIRST_ECCENTRICITY 		= sqrt ((pow(WGS84_EQUATOR_RADIUS,2.0) - pow(WGS84_POLAR_RADIUS,2)) / pow(WGS84_EQUATOR_RADIUS,2.0));// Ellipsoid First Eccentricity
    const double WGS84_SECOND_ECCENTRICITY 		= sqrt ((pow(WGS84_EQUATOR_RADIUS,2.0) - pow(WGS84_POLAR_RADIUS,2)) / pow(WGS84_POLAR_RADIUS,2.0));// Ellipsoid Second Eccentricity

#endif // CONSTANTS_H
