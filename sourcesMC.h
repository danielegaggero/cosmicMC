#ifndef __sourcesMC_H
#define __sourcesMC_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <vector>

class Input;
class randgen;

using namespace std;

// **********************************************************************************************************************

class propagation_model {

 public:

   // methods

   propagation_model(Input*); //constructor
   ~propagation_model();
   double GetD(double, Input*);
   inline double GetD_cm2s(int ie) { return D_vec[ie]*1.e28;  }

   // attributes

   vector<double> energy_vec;
   double E0;
   double delta;
   double D0;
   vector<double> D_vec;
   double L;
   double xsun;
   double ysun;
   double zsun;

};

// **********************************************************************************************************************

class event {

  public:

    // methods

    event(Input*, randgen*, vector<double>);
    ~event();
    double GetDistance(propagation_model*);

    // attributes

    double age;
    double age_sec;
    double radius;
    double theta;
    double x;
    double y;
    double z;
    double energy_output;
    double alpha;
    double cut;

};

// **********************************************************************************************************************

class sourcesMC {

 public:

   sourcesMC();
   sourcesMC(Input*);
   ~sourcesMC();
   
   vector<double> compute_SNR_spectrum(vector<double>, Input*, event*);

   void evolve(Input*);
   long int linearized_index(Input*, int, int);


 protected:

   vector<double> E_vec;
   double dimE;
   double Emin;
   double Emax;
   //vector<double> current_spectrum;
   vector<double> total_spectrum;
   propagation_model* propmodel;
   randgen* myrandom;
   long int Nevents;

};

// **********************************************************************************************************************


#endif
