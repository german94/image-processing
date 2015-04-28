#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/time.h>
#include "util.h"
#include "Matriz.h"

// #define DEBUG

using namespace std;


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

enum Metodo { KNN = 0,
			  PCA_KNN = 1};

#define MAX_H 50

void usage() {
	cout << "./tp <input_filename> <output_filename> <metodo>" << endl;
}





int main(int argc, char *argv[]) {
	if(argc != 4) {
		usage();
		return 0;
	}
	ifstream entrada(argv[1],std::ifstream::in);
	ofstream salida(argv[2],std::ofstream::out);

	Metodo metodo = (Metodo) string_to_type<int>(argv[3]);

    unsigned int num_linea = 1;
    int estado = 0;


    string path;
    unsigned int k = 0;
    unsigned int alpha = 0;
    unsigned int K=0;

    string lineaInicial;
    getline(entrada, lineaInicial); //la linea de la entrada avanzo
    num_linea++;
    vector<string> valoresIniciales = separar(lineaInicial);

     if (valoresIniciales.size() != 4) {
        throw runtime_error("Se esperaban 4 valores en línea ");
    }

    path = valoresIniciales[0];
    k = string_to_type<unsigned int>(valoresIniciales[1]);
    alpha = string_to_type<unsigned int>(valoresIniciales[2]);
    K = string_to_type<unsigned int>(valoresIniciales[3]);


    Matriz<bool> Klineas(K,42000); // K vectores de 42000 valores en memoria
    int filaM=0;  // fila de la matriz


   //se empieza a analizar desde la segunda linea
    for (string linea; getline(entrada, linea); num_linea++) {

        vector<string> valores = separar(linea);

                if (valores.size() != 42000) {
                    throw runtime_error("Se esperaban 42000 valores en línea ");
                }

                for(int colM=0;colM<42000;colM++){

                    Klineas[filaM][colM] = string_to_type<bool>(valores[colM]);
                }

                filaM++;

    }

/*
cout<< "el path es "<<path<< endl;
cout<< "el k es "<<k<<endl;
cout<< "el alpha es "<<alpha<<endl;
cout<< "el K es "<<K<<endl;

for(int i=0;i<Klineas.filas();i++){
cout<<"[ ";
    for(int j=0;j<Klineas.columnas();j++){
    cout<<Klineas[i][j]<<" , ";


    }

cout<<" ]\n";

}

*/

    switch(metodo){

        case KNN:{


  //ifstream train("../data/train.csv");
  //ifstream test("../data/test.csv");



            //for(int i=0;i<longitud;i++){
            //     salida << std::fixed << std::setprecision(5) << i/cantidadColumnasP<<"\t"<<i%cantidadColumnasP<<"\t"<<resGaussCompacto[i][0] <<endl;
            // }
            break;
        }

        case PCA_KNN: {


            //metodo




            break;
        }


        default:

            throw runtime_error("No existe ese metodo");

    }

    return 0;

}
