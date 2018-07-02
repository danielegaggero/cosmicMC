#include "constants.h"
#include "randgen.h"
#include "input.h"
#include <string>

using namespace std;

randgen::randgen() { 

}

randgen::randgen(Input* inp) {

}

randgen::~randgen() {

}

// *****************************************

double randgen::SNR_ferriere_r(double radius){

	double z = 0.;
	if (radius > 3.7)
		return (50.0*(0.79*exp(-pow(z/0.212,2.))+0.21*exp(-pow(z/0.636,2.)))*exp(-((radius)*(radius)-robs*robs)/(6.8*6.8)) + 7.3*exp(-(radius-robs)/4.5-fabs(z)/0.325));
    	else 
		return (177.5*(0.79*exp(-pow(z/0.212,2.))+0.21*exp(-pow(z/0.636,2.)))*exp(-pow((radius-3.7)/2.1, 2.)) + 7.3*exp(-(radius-robs)/4.5-fabs(z)/0.325));
}

// *****************************************

double randgen::SNR_ferriere_z(double z){

	double radius = robs;
	if (radius > 3.7) 
		return (50.0*(0.79*exp(-pow(z/0.212,2.))+0.21*exp(-pow(z/0.636,2.)))*exp(-((radius)*(radius)-robs*robs)/(6.8*6.8)) + 7.3*exp(-(radius-robs)/4.5-fabs(z)/0.325));
    	else
		return (177.5*(0.79*exp(-pow(z/0.212,2.))+0.21*exp(-pow(z/0.636,2.)))*exp(-pow((radius-3.7)/2.1, 2.)) + 7.3*exp(-(radius-robs)/4.5-fabs(z)/0.325));
}

// *****************************************


double randgen::SNR_ferriere_r_norm(double radius){
	return SNR_ferriere_r(radius)/SNR_ferriere_r(robs);
}

double randgen::SNR_ferriere_z_norm(double z){
	return SNR_ferriere_z(z)/SNR_ferriere_z(zobs);
}

// *****************************************

double randgen::random_arbitrary_function(string func_name, double xmin, double xmax) {

	// randgen_numsteps is in constants.h

	double ymin = 0.;
	double ymax;
	if (func_name == "SNR_ferriere_r_norm")
		ymin = SNR_ferriere_r_norm(xmin);
	if (func_name == "SNR_ferriere_z_norm")
		ymin = SNR_ferriere_z_norm(xmin);
	ymax = ymin;

	for (int i=0; i<randgen_numsteps; i++) {
    		double x = xmin + (xmax - xmin) * i / randgen_numsteps;    
		double y = 0.	;	
		if (func_name == "SNR_ferriere_r_norm")
			y = SNR_ferriere_r_norm(x);
		if (func_name == "SNR_ferriere_z_norm")
			y = SNR_ferriere_z_norm(x);
    		if (y < ymin) 
			ymin = y;
    		if (y > ymax) 
			ymax = y;
	}

	while (true) {
		double xr = ((double) rand() / (RAND_MAX));
        double yr = ((double) rand() / (RAND_MAX));
        double xrand = xmin + (xmax - xmin) * xr;
        double yrand = ymin + (ymax - ymin) * yr;
		double f_of_x = 0.;
		if (func_name == "SNR_ferriere_r_norm")
			f_of_x = SNR_ferriere_r_norm(xrand);
		if (func_name == "SNR_ferriere_z_norm")
			f_of_x = SNR_ferriere_z_norm(xrand);
        	if (yrand <= f_of_x)
            		return xrand;            		
	}
}
