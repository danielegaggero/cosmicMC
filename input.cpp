#include "input.h"
#include "constants.h"
#include "tinyxml.h"
#include "errorcode.h"
#include "math.h"

#include <string>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std;

int Input::QueryIntAttribute(string obj, TiXmlElement* el) {
  TiXmlElement* el1 = el->FirstChildElement(obj.c_str());
  int result = -1;
  if (el1) el1->QueryIntAttribute("value", &result);
  return result;
}

double Input::QueryDoubleAttribute(string obj, TiXmlElement* el) {
  TiXmlElement* el1 = el->FirstChildElement(obj.c_str());
  double result = -1.0;
  if (el1) el1->QueryDoubleAttribute("value", &result);
  return result;
}

double Input::QueryDoubleAttributeWithDefault(string obj, TiXmlElement* el, double def) {
  TiXmlElement* el1 = el->FirstChildElement(obj.c_str());
  double result = def;
  if (el1) el1->QueryDoubleAttribute("value", &result);
  return result;
}


int Input::LoadFile(const string inputfilename) {
   
  TiXmlDocument* doc = new TiXmlDocument(inputfilename.c_str());
   
  TiXmlElement* el = NULL;
  //TiXmlElement* el1 = NULL;
  //TiXmlElement* el2 = NULL;
   
  if (doc->LoadFile(inputfilename.c_str(), TIXML_DEFAULT_ENCODING)) {
     
    string gridstr = "Grid";     
    el = doc->FirstChildElement(gridstr.c_str());
 
   if (el){
	string Eminstr = "Emin";	
        Emin_GeV = QueryDoubleAttribute(Eminstr, el);
	string Emaxstr = "Emax";	
        Emax_GeV = QueryDoubleAttribute(Emaxstr, el);
	string nEstr = "nE";	
        nE = QueryDoubleAttribute(nEstr, el);
	deltalogE =  log10(Emax_GeV/Emin_GeV)/(nE-1);
	cout << "energy vector: " << endl;
	for (int ie=0; ie<(nE); ie++) {
           E_vec.push_back( Emin_GeV*pow(10.,ie*deltalogE) );
	   cout << E_vec.back() << " ";
	}
	cout << endl;

    }

    string SNRstr = "SNR";     
    el = doc->FirstChildElement(SNRstr.c_str());
    if (el){
	string deltatstr = "deltat";	
        deltat = QueryDoubleAttribute(deltatstr, el);
        string SNR_energy_outputstr = "SNRenergy";	
        SNR_energy_output = QueryDoubleAttribute(SNR_energy_outputstr, el);
	string SNR_slope_str = "SNRslope";	
        SNR_slope = QueryDoubleAttribute(SNR_slope_str, el);
	string Nsamples_str = "Nsamples";	
        Nsamples = QueryIntAttribute(Nsamples_str, el);
  
    }

    string propstr = "Propagation";     
    el = doc->FirstChildElement(propstr.c_str());
    if (el){
	string deltastr = "delta";	
        delta = QueryDoubleAttribute(deltastr, el);
        string D0str = "D0";	
        D0 = QueryDoubleAttribute(D0str, el);
	string E0str = "E0";	
        E0 = QueryDoubleAttribute(E0str, el);
	string Lstr = "L";	
        L = QueryDoubleAttribute(Lstr, el);
		
    }

  }

  delete doc;
   
  cout << "Input file read successfully!" << endl;
   
  return 0;
}


void Input::Print() {

  cout << endl;
  cout << "************************************** " << endl;
  cout << "You selected the following parameters: " << endl;
  cout << "Emin [GeV] : "   << Emin_GeV << endl;
  cout << "Emax [GeV] : "   << Emax_GeV << endl;
  cout << "number of energy points : " << nE << endl;
  cout << "deltat : " << deltat << endl;
  cout << "SNR energy output [10^48 GeV] : " << SNR_energy_output << endl;
  cout << "inj slope : " << SNR_slope << endl;
  cout << "************************************** " << endl;
  cout << endl;

}
