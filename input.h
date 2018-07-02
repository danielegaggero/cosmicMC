#ifndef _INPUT_H
#define _INPUT_H

#include <iostream>
#include <string>
#include <vector>

#include "constants.h"

class TiXmlElement;

using namespace std;

class Input {
   
private:
   int QueryIntAttribute(string, TiXmlElement*);
   double QueryDoubleAttribute(string, TiXmlElement*);
   double QueryDoubleAttributeWithDefault(string, TiXmlElement*, double);
   
public:
   Input() { }  /**< The default constructor. */

   int LoadFile(const string);
   void Print();
   
   ~Input() { } /**< Destructor. Does nothing. */
   
   // Grid

   double Emin_GeV; //GeV
   double Emax_GeV;
   double nE;
   double deltalogE;
   vector<double> E_vec;

   double SNR_energy_output;
   double deltat;
   double SNR_slope;
   int Nsamples;

   double delta;
   double D0;
   double E0; //reference rigidity for D0
   double L;

};

#endif
