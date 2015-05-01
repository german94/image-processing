#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/time.h>
#include "util.h"
#include "Matriz.h"

// #define DEBUG

using namespace std;


    /* //DESCOMENTAR SI ES QUE SE DEBE ARREGLAR LO DE LOS k VECINOS
    bool haymayor(vector<pair<int, int> >  &normas2AlCuadrado,  unsigned int distanciaAlCuadrado)
    {
        bool res = false;
        for(int i = 0;  i < normas2AlCuadrado.size(); i++)
        {
            if(distanciaAlCuadrado <= normas2AlCuadrado[i].second) { res = true; break;}
        }
        return res;
    }
    
    int dondemayor(vector<pair<int, int> >  &normas2AlCuadrado)
    {
        int res = 0;
        for(int i = 0; i < normas2AlCuadrado.size(); i++)
        {
            if (normas2AlCuadrado[i].second >= normas2AlCuadrado[res].second) {res = i;}
        }
        return res;
    }
*/

void cargarMatrizTrain(Matriz<unsigned int>& train);
vector<double> kNN(unsigned int K, unsigned int k, Matriz<bool>& Klineas, Matriz<unsigned int>& train);
unsigned int norma2AlCuadrado(vector<unsigned int> v1, vector<unsigned int> v2);


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


    Matriz<bool> Klineas(K,42000); // matriz con las particiones a realizar
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

    Matriz<unsigned int> train(42000, 785);     //42k de imagenes, cada imagen son 784 valores y un valor mas que representa el digito al que corresponde
    cargarMatrizTrain(train);

    switch(metodo){

        case KNN:{

            vector<double> res = kNN(K, k, Klineas, train);

            for(int i=0; i<res.size();i++){
            cout<<res[i]<<endl;

            }

            ofstream tasaReconocimiento("tasaReconocimiento.txt",std::ofstream::out);// genero este nuevo archivo de salda con el formato pero el vector tiene valores cero
            tasaReconocimiento<<"ImageId,Label\n";

            for (int i = 0; i < res.size(); ++i){

               tasaReconocimiento<<i+1<<","<<res[i]<<endl;

           }

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


void cargarMatrizTrain(Matriz<unsigned int>& train)
{
    ifstream baseTrain("../data/train.csv", std::ifstream::in);
    string lineaNombrePixel;
    getline(baseTrain, lineaNombrePixel); //la linea de la entrada avanzo

   // cout<<"primera linea\n";
   // cout<<lineaNombrePixel<<endl;

    int filaM = 0;
    for (string linea; getline(baseTrain, linea);)
    {
        vector<string> valores = separarComa(linea);

        if (valores.size() != 785) {
            throw runtime_error("Se esperaban 785 valores en línea ");
        }

        for(int colM = 0; colM < 785; colM++){

            train[filaM][colM] = string_to_type<unsigned int>(valores[colM]);
        }

        filaM++;
    }
}

vector<double> kNN(unsigned int K, unsigned int k, Matriz<bool>& Klineas, Matriz<unsigned int>& train)
{
    vector<double> tasaDeReconocimiento(K);

    for(int i = 0; i < K; i++)
    {
        vector<pair<unsigned int, vector<unsigned int> > > imagenesDeTrainParaReconocer;
        vector<pair<unsigned int, vector<unsigned int> > > imagenesDeTrainParaEntrenar;

        for(int j = 0; j < Klineas.columnas(); j++)//42000
        {
            vector<unsigned int> imagen;
            for(int l = 0; l < 784; l++)
                imagen.push_back(train[j][l + 1]);

            pair<unsigned int, vector<unsigned int> > p(train[j][0], imagen);

            if(Klineas[i][j] == 0)
                imagenesDeTrainParaReconocer.push_back(p);
            else
                imagenesDeTrainParaEntrenar.push_back(p);
        }


        unsigned int reconocidos = 0;
        for(int z = 0; z < imagenesDeTrainParaReconocer.size(); z++)
        {
            vector<pair<int, int> > normas2AlCuadrado(k, pair<unsigned int, int>(-1, -1));

            for(int m = 0; m < imagenesDeTrainParaEntrenar.size(); m++)
            {
                unsigned int distanciaAlCuadrado = norma2AlCuadrado(imagenesDeTrainParaReconocer[z].second, imagenesDeTrainParaEntrenar[m].second);
                for(int r = 0; r < k; r++)
                {
                    if(normas2AlCuadrado[r].first == -1 || (normas2AlCuadrado[r].first != -1 && normas2AlCuadrado[r].second > distanciaAlCuadrado))
                    {
                        normas2AlCuadrado[r].first = imagenesDeTrainParaEntrenar[m].first;
                        normas2AlCuadrado[r].second = distanciaAlCuadrado;
                    }
                }

                unsigned int digitos[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
                for(int t = 0; t < k; t++)
                    digitos[normas2AlCuadrado[t].first]++;

                int ganador = -1;
                for(int x = 0; x < 10; x++)
                {
                    if(digitos[x] > ganador)
                        ganador = x;
                }

                if(ganador == imagenesDeTrainParaReconocer[z].first)
                    reconocidos++;
            }
        }

        double tasa = (double)reconocidos/(double)imagenesDeTrainParaReconocer.size();
        tasaDeReconocimiento.push_back(tasa);
    }

    return tasaDeReconocimiento;
}


unsigned int norma2AlCuadrado(vector<unsigned int> v1, vector<unsigned int> v2)
{
    unsigned int n = 0;
    for(int i = 0; i < v1.size(); i++)
    {
        n += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }

    return n;
}

/*
//HABRIA QUE REEMPLAZAR LO DE ADENTRO DEL FOR DE LA LINEA 218 POR TODO LO SGTE
    vector<pair<int, int> > normas2AlCuadrado;

            for(int m = 0; m < imagenesDeTrainParaEntrenar.size(); m++)
            {
                unsigned int distanciaAlCuadrado = norma2AlCuadrado(imagenesDeTrainParaReconocer[z].second, imagenesDeTrainParaEntrenar[m].second);
                
                if(normas2AlCuadrado.size() < k) //coloco las primeras k normas
                {
                    pair<unsigned int, int> a;
                    a.first =  imagenesDeTrainParaEntrenar[m].first;
                    a.second = distanciaAlCuadrado;
                    normas2AlCuadrado.push_back(a);
                }
                else
                {
                    if(haymayor(normas2AlCuadrado, distanciaAlCuadrado)); //si ya tengo k voy sacando las mayores
                    {
                        int pos_mayor = dondemayor(normas2AlCuadrado);
                        normas2AlCuadrado[pos_mayor].first = imagenesDeTrainParaEntrenar[m].first;
                        normas2AlCuadrado[pos_mayor].second = distanciaAlCuadrado;
                    }
                }
                
                unsigned int digitos[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
                for(int t = 0; t < k; t++)
                    digitos[normas2AlCuadrado[t].first]++;

                int ganador = -1;
                for(int x = 0; x < 10; x++)
                {
                    if(digitos[x] > ganador)
                        ganador = x;
                }

                if(ganador == imagenesDeTrainParaReconocer[z].first)
                    reconocidos++;
            }

*/