#ifndef __CONSTANTS_H
#define __CONSTANTS_H

const double m_p = .939; 
const double m_e = 0.000510998918; // electron mass in GeV/c^2
const double b = 1.4e-16;         // energy loss rate in GeV^-1*s^-1
const double c2kB = 6.50966198;   // divided by 10^36
const double GeV_erg = 0.00160217646;
const double erg_GeV = 624.15; 
const double c0 = 2.99792458e8;               // speed of light in vacuum m*s^-1
const double yr_sec = 31556925.2;             // tropical year in seconds
const double sec_yr = 1./yr_sec;          // to pass from seconds to year
const double pc_mt = 3.0856775807e16 ;        // parsec in meters
const double kpc_cm = 3.0856775807e21;        // kpc in cm
const double c_kpc_Myr = c0*1.e6*yr_sec/pc_mt;  // kpc/Myr
const double c_cm_s = c0*100.;
const double robs = 8.3;
const double zobs = 0.;

const double rate = 0.3*0.01;

const int randgen_numsteps = 10;

const double precision_images_method = 0.001;
const int N_images_MAX = 100; // maximum value; two image charges x time


#endif
