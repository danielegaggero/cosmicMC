#include <iostream>
#include <stdio.h>
#include <cstdlib> 
#include <ctime>

#include "constants.h"
#include "errorcode.h"
#include "sourcesMC.h"
#include "input.h"
#include "randgen.h"

using namespace std;

int main(int argc, char** argv) {

  if (argc != 2) {
        cerr << "Usage: ./sourcesMC <xml-file>" << endl;
        exit(-1);
  }

  time_t time_s;
  time_t time_e;

  time(&time_s); // fix initial time

  Input* inp = new Input();
  inp->LoadFile( argv[1] );

  cout << "Parameter file read successfully" << endl;
  inp->Print();

  cout << "Calling the construcor... " << endl;
 
  sourcesMC* supernovaMC = new sourcesMC(inp);

  cout << endl << "Evolving... " << endl;

  supernovaMC->evolve(inp);

  cout << "End of evolution " << endl << endl;

  time(&time_e); // fix final time
  cout << "Solution found in " << (double)(time_e-time_s) << " s." << endl;

  delete supernovaMC;
  delete inp;

  return 0;

}
