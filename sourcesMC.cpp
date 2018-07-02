#include "constants.h"
#include "sourcesMC.h"
#include "input.h"
#include "randgen.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ctime>

#define DEBUG 0
#define TESTMODE 0
#define NDEBUG 10
#define NUMTHREADS 4

using namespace std;

// **********************************************************************************************************************
// **********************************************************************************************************************

propagation_model::propagation_model(Input* inp) {
        
        energy_vec = inp->E_vec;
	delta = inp->delta; //0.5
	D0 = inp->D0; // 2.24
	E0 = inp->E0; //4.
	/*if (DEBUG==2)
	  cout << D0 << " " << E0 << " " << delta << endl;*/
	for (unsigned int ie=0; ie<energy_vec.size(); ie++)
	  D_vec.push_back( pow( (D0*energy_vec[ie]/E0), delta) );
	L = inp->L; //4. 
	xsun = robs; //this is in constants.h
	ysun = 0.;
	zsun = zobs;

}

propagation_model::~propagation_model() {

}

double propagation_model::GetD(double energy, Input* inp){

	unsigned int index = (unsigned int) log10(energy/energy_vec.front())/(inp->deltalogE-1);

	return D_vec[index];
}

// **********************************************************************************************************************
// **********************************************************************************************************************

event::event(Input* inp, randgen* rand_generator, vector<double> E_vec) {

	//cout << "generating age in " << inp->deltat << endl;
	age = ((double) rand() / (RAND_MAX))*inp->deltat;
	//cout << "age = " << age << endl;
	age_sec = age * 31556925.2;
	radius = rand_generator->random_arbitrary_function("SNR_ferriere_r_norm", 0., 15.);
	theta = ((double) rand() / (RAND_MAX))*2.*M_PI;
	x = radius*cos(theta);
	y = radius*sin(theta);
	z = rand_generator->random_arbitrary_function("SNR_ferriere_z_norm",-2.,2.);
	energy_output = inp->SNR_energy_output;	
	alpha = inp->SNR_slope;
        cut = 1.e5;

}

event::~event() {

}

double event::GetDistance(propagation_model* propmodel) {

	double distance = sqrt( pow(x - propmodel->xsun, 2.) + pow(y - propmodel->ysun, 2.) + pow(z - propmodel->zsun, 2.) );
        return distance;

}

// **********************************************************************************************************************
// **********************************************************************************************************************

sourcesMC::sourcesMC() { 
  cout << "I am the default costructor. I do nothing" << endl;
}

// constructor --------------------------------------------------------------------------------------------------------------------------------

sourcesMC::sourcesMC(Input* inp)
{

  cout << "This is the sourcesMC constructor" << endl;
  
  E_vec = inp->E_vec;
  dimE = E_vec.size();
  Emin = E_vec.front();
  Emax = E_vec.back();

  for (unsigned int ie=0; ie<dimE; ie++)
    total_spectrum.push_back(0.);

  propmodel = new propagation_model(inp);
  myrandom  = new randgen();

  Nevents = (long int) (inp->deltat * rate); //rate is in constants.h
  cout << inp->deltat << endl;
  cout << rate << endl;
  cout << Nevents << endl;
 

}

// --------------------------------------------------------------------------------------------------------------------------------------------
// destructor ---------------------------------------------------------------------------------------------------------------------------------

sourcesMC::~sourcesMC()
{

  delete propmodel;
  delete myrandom;


}

// --------------------------------------------------------------------------------------------------------------------------------------------

vector<double> sourcesMC::compute_SNR_spectrum(vector<double> E_vec, Input* inp, event* explosion) {

  vector<double> result(E_vec.size(), 0.0);

  double Q_0 = 1.e48*explosion->energy_output*erg_GeV * 1./(2. - explosion->alpha) * ( pow(Emax,(2.-explosion->alpha)) - pow(Emin,(2.-explosion->alpha))  );  // GeV^-1

  unsigned int ie = 0;
  unsigned int i_images = 0;

#ifdef _OPENMP
#pragma omp parallel for default(shared) private(ie,i_images) schedule(dynamic) num_threads(NUMTHREADS)
#endif		
  for (ie = 0; ie < E_vec.size(); ie++) {

    double D_cm2_s = propmodel->GetD_cm2s(ie); // cm^2/s (GetD is in units of 10^28 cm^2/s)
    double D_kpc2_s = D_cm2_s / (kpc_cm*kpc_cm); //kpc^2/s
			 
    double L = propmodel->L; // kpc

    //versione con cutoff --> Q = Q_0 * energy_vec[ie]**(-alpha[i]) * np.exp(-energy_vec[ie]/cut[i]) #GeV^-1
    double Q = Q_0 * pow(E_vec[ie],(-explosion->alpha));

    double norm = Q / ( pow(4.*M_PI*D_cm2_s*explosion->age_sec,1.5) ); // 1/(GeV cm^3)
   
    /*if (DEBUG==1){
	cout << "E = " << E_vec[ie] << "; D0 " << D_cm2_s << endl;
	cout << "Q0 = " << Q_0 << endl;
	cout << "norm = " << norm << endl;
    }*/			

    double e1 = exp( -( pow((explosion->x - propmodel->xsun),2.) + pow((explosion->y - propmodel->ysun),2.) ) / (4.*D_kpc2_s*explosion->age_sec) );
		
    // N_images is in constants.h

    double e2 = 0.;
    double previous_e2 = 0.;
    i_images = 0;

    double accuracy = 100.;
    do {
	previous_e2 = e2;

	if(i_images == 0)
           e2 += exp( -(pow((propmodel->zsun - explosion->z), 2.)) / (4.*D_kpc2_s*explosion->age_sec) );
        else{
   	   double zprimeplus  = pow(-1.,i_images) * explosion->z + 2.*i_images*L;
           double zprimeminus = pow(-1.,-i_images) * explosion->z - 2.*i_images*L;
	   double temp1 = pow(-1.,i_images) * exp( -( pow((propmodel->zsun - zprimeplus), 2.) ) / (4.*D_kpc2_s*explosion->age_sec) );
	   double temp2 = pow(-1.,-i_images) * exp( -( pow((propmodel->zsun - zprimeminus), 2.) ) / (4.*D_kpc2_s*explosion->age_sec) );
	   e2 = e2 + temp1 + temp2;
	}
	accuracy = 100.;
	if (i_images > 0)
	  accuracy = fabs((e2-previous_e2)/e2);
	  //cout << i_images << " -> " << previous_e2 << " " << e2 << " " << accuracy << " " << precision_images_method << endl;
 	i_images++;

    } while ( (accuracy > precision_images_method) && (i_images < N_images_MAX) );

    result[ie] =  norm*e1*e2 * 10000. * c_cm_s/4./M_PI ; // 1/( GeV m^2 s sr )
    if (result[ie] < 0.) result[ie] = 0.;
  }

  return result;

}

// --------------------------------------------------------------------------------------------------------------------------------------------

long int sourcesMC::linearized_index(Input* inp, int i, int ie)  {
	
	return (i*inp->nE +ie);
}

// evolutor ----------------------------------------------------------------------------------------------------------------------------------

void sourcesMC::evolve(Input* inp) 
{

  cout << "This is the sourcesMC evolutor" << endl;
  cout << "***" << endl;

  for (int iSample=0; iSample<inp->Nsamples; iSample++) {
   
    cout << "This is chain n. " << iSample << endl;

    ofstream outfile;
    stringstream sst;
    sst << "final_spectrum_" << iSample << ".dat";
    outfile.open (sst.str().c_str());

    ofstream outfile_complete;
    stringstream sst2;
    sst2 << "all_spectra_" << iSample << ".dat";
    outfile_complete.open (sst2.str().c_str());

    cout << "Generating " << Nevents << " events in a period of " << inp->deltat << " years " << endl;

    long int N_debug = (long int) Nevents/NDEBUG;

    for (unsigned int ie=0; ie<E_vec.size(); ie++) 
	total_spectrum[ie] = 0.;

    srand(time(NULL));

    for (long long i=0; i<Nevents; i++) {
		
      event* explosion = new event(inp, myrandom, E_vec);

      vector<double> current_spectrum = compute_SNR_spectrum(E_vec, inp, explosion); 

      if (i%N_debug==0){
		  cout<< " *** "<< i << " x = "<< explosion->x << " y = " << explosion->y << " z = " << explosion->z << " radius = " << explosion->radius << endl;
 		  cout<< " *** "<< i << " age = " <<  explosion->age << " y; distance = " << explosion->GetDistance(propmodel) << " pc; energy = " << explosion->energy_output << " * 10^48 erg " << endl;
      }

      for (unsigned int ie=0; ie<E_vec.size(); ie++) {
	total_spectrum[ie] += current_spectrum[ie];
      }
   
      delete explosion;

    }

    for (unsigned int ie=0; ie<E_vec.size(); ie++) 
	outfile << E_vec[ie] << "\t" << total_spectrum[ie] << endl;
      
    outfile.close();
    outfile_complete.close();

  }

}
// --------------------------------------------------------------------------------------------------------------------------------------------

