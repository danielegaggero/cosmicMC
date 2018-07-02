#ifndef __RANDGEN_H
#define __RANDGEN_H

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <vector>

class Input;

using namespace std;

class randgen {

 public:
  randgen();
  randgen(Input*);
  ~randgen();
  double SNR_ferriere_r(double);
  double SNR_ferriere_z(double);
  double SNR_ferriere_r_norm(double);
  double SNR_ferriere_z_norm(double);
  double random_arbitrary_function(string, double, double) ;

 protected:

//  


};


#endif
