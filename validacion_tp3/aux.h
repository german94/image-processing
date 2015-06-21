#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/time.h>
#include <math.h>
#include <algorithm>

timeval start, endTime;
//////////////////////////////////////////////////////////////
void init_time()
{
      gettimeofday(&start,NULL);
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
double get_time()
{
      gettimeofday(&endTime,NULL);
      return
(1000000*(endTime.tv_sec-start.tv_sec)+(endTime.tv_usec-start.tv_usec))/1000000.0;
}
//////////////////////////////////////////////////////////////

enum Metodo { VECINO = 0, BILINEAL = 1, BILINEALBIS = 2, SPLINE = 3, SPLINEVARIABLE = 4};
enum SubMetodo { ORGINALES = 0, CALCULADOSF = 1, PROM = 2};
enum SubMetodoDeBilineal { ORGINALEs = 0, EXPANDIDO = 1, PORDIAGONALES = 2, PORBLOQUES = 3};

void usage() { cout << "./tp <input_filename> <K > <metodo>" << endl; }

void usageSpline() { cout << "./tp <input_filename> <K > <metodo> <bloques>" << endl; }

void usageVecino() { cout << "./tp <input_filename> <K > <metodo> <submetodo>" << endl; }

void usageBilinealBis() { cout << "./tp <input_filename> <K > <metodo> <submetodoBilineal>" << endl; }

