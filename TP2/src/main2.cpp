#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/time.h>
#include "util.h"
#include "Matriz.h"

// #define DEBUG

using namespace std;

void init_time();
double get_time();
void usage();

void cargarMatrizTrain(Matriz<unsigned int>& train);
void cargarMatrizEntrada(Matriz<bool>& Klineas, ifstream entrada);
void leerParametros(unsigned int& k, unsigned int& alpha, unsigned int& K);
void kNN(unsigned int K, unsigned int k, const Matriz<bool>& Klineas, const Matriz<unsigned int>& train, ofstream salida);
unsigned int norma2AlCuadrado(vector<unsigned int> v1, vector<unsigned int> v2);

enum Metodo { KNN = 0,
              PCA_KNN = 1};
timeval start, endTime;

int main(int argc, char *argv[]) {

	if(argc != 4) {
		usage();
		return 0;
	}

	ifstream entrada(argv[1],std::ifstream::in);
	ofstream salida(argv[2],std::ofstream::out);

	Metodo metodo = (Metodo) string_to_type<int>(argv[3]);

    int estado = 0;

    string path;
    unsigned int k = 0;
    unsigned int alpha = 0;
    unsigned int K = 0;
    leerParametros(k, alpha, K);


    Matriz<bool> Klineas(K, 42000); // K vectores de 42000 valores en memoria
    cargarMatrizEntrada(Klineas, entrada);

    Matriz<unsigned int> train(42000, 785);     //42k de imagenes, cada imagen son 784 valores y un valor mas que representa el digito al que corresponde
    cargarMatrizTrain(train);


    switch(metodo){

        case KNN:
        {
            void kNN(K, k, Klineas, train, salida);
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

    int filaM = 0;
    while(string linea, getline(baseTrain, linea)) 
    {
        vector<string> valores = separar(linea);

        if (valores.size() != 785) {
            throw runtime_error("Se esperaban 785 valores en línea ");
        }

        for(int colM = 0; colM < 785; colM++){

            train[filaM][colM] = string_to_type<unsigned int>(valores[colM]);
        }

        filaM++;
    }
}


void cargarMatrizEntrada(Matriz<bool>& Klineas, ifstream entrada)
{
    int filaM = 0;  // fila de la matriz

    //se empieza a analizar desde la segunda linea
    while(string linea, getline(entrada, linea)) 
    {
        vector<string> valores = separar(linea);

        if (valores.size() != 42000) {
            throw runtime_error("Se esperaban 42000 valores en línea ");
        }

        for(int colM = 0; colM < 42000; colM++){

            Klineas[filaM][colM] = string_to_type<bool>(valores[colM]);
        }

        filaM++;

    }
}

void leerParametros(unsigned int& k, unsigned int& alpha, unsigned int& K)
{
    string lineaInicial;
    getline(entrada, lineaInicial); //la linea de la entrada avanzo
    vector<string> valoresIniciales = separar(lineaInicial);

     if (valoresIniciales.size() != 4) {
        throw runtime_error("Se esperaban 4 valores en línea ");
    }

    path = valoresIniciales[0];
    k = string_to_type<unsigned int>(valoresIniciales[1]);
    alpha = string_to_type<unsigned int>(valoresIniciales[2]);
    K = string_to_type<unsigned int>(valoresIniciales[3]);
}

void kNN(unsigned int K, unsigned int k, const Matriz<bool>& Klineas, const Matriz<unsigned int>& train, ofstream salida)
{
    vector<int> tasaDeReconocimiento(K);

    for(int i = 0; i < K; i++)
    {
        vector<pair<unsigned int, vector<unsigned int> > > imagenesDeTrainParaReconocer;
        vector<pair<unsigned int, vector<unsigned int> > > imagenesDeTrainParaEntrenar;

        for(int j = 0; j < Klineas.columnas(); j++)
        {
            vector<unsigned int> imagen;
            for(int l = 0; l < 784; l++)
                imagen.push_back(train[j + 1][l + 1]);

            pair<unsigned int, vector<unsigned int> > p(train[j + 1][0], imagen);

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

                unsigned int digitos[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
                for(int t = 0; t < k; t++)
                    digitos[normas2AlCuadrado[t].first]++;

                int ganador = -1;
                for(int x = 0; x < 9; x++)
                {
                    if(digitos[x] > ganador)
                        ganador = x;
                }

                if(ganador + 1 == imagenesDeTrainParaReconocer[z].first)
                    reconocidos++;
            }
        }
    }
}

unsigned int norma2AlCuadrado(vector<unsigned int> v1, vector<unsigned int> v2)
{
    unsigned int n = 0;
    for(int i = 0; i < v1.size(); i++)
    {
        if(v1[i] > v2[i])
            n += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }

    return n;
}

void init_time()
{
      gettimeofday(&start,NULL);
}

double get_time()
{
      gettimeofday(&endTime,NULL);
      return
(1000000*(endTime.tv_sec-start.tv_sec)+(endTime.tv_usec-start.tv_usec))/1000000.0;
}


void usage() {
    cout << "./tp <input_filename> <output_filename> <metodo>" << endl;
}
