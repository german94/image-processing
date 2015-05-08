#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/time.h>
#include "util.h"
#include "Matriz.h"
#include "mPotencia.h"
#include <math.h>

// #define DEBUG

using namespace std;

bool haymayor(vector<pair<int, int> >  &normas2AlCuadrado,  unsigned int distanciaAlCuadrado);
int dondemayor(vector<pair<int, int> >  &normas2AlCuadrado);
void cargarMatrizTrain(Matriz<int>& train);
void cargarMatrizTest(Matriz<double>& test);
unsigned int norma2AlCuadrado(vector<unsigned int> &v1, vector<unsigned int> &v2);
vector<double> kNN(unsigned int k, Matriz<double> &tcTest, Matriz<double> &tcTrain, vector<int> &digitosImagenesTrain);


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

void usage() {
	cout << "./main <output_filename> <metodo> <k> <alpha>" << endl;
}

const int CIMAGENESTRAIN=20;
const int CIMAGENESTEST=10;
const int CPIXELES=7;
const int CPIXELESYETIQUETA=8; //solo train

int main(int argc, char *argv[]) {
	if(argc != 5) {
		usage();
		return 0;
	}

    //Ejmplo de entrada "./main <output_filename> <metodo> <k> <alpha>"
	ofstream salida(argv[1],std::ofstream::out);
	Metodo metodo = (Metodo) string_to_type<int>(argv[2]);
    unsigned int k = string_to_type<unsigned int>(argv[3]);
    unsigned int alpha = string_to_type<unsigned int>(argv[4]);
 

    Matriz<int> train(CIMAGENESTRAIN, CPIXELESYETIQUETA);     //42k de imagenes, cada imagen son 784 valores y un valor mas que representa el digito al que corresponde

    cargarMatrizTrain(train);

    Matriz<double> test(CIMAGENESTEST, CPIXELES);

    cargarMatrizTest(test);

    switch(metodo){

        case KNN:{

                vector<int> digitosImagenesTrain; ///importante este vector tiene los digitos de la etiqueta de los Train
       
                for(int j=0; j<CIMAGENESTRAIN; j++)
                {
                    digitosImagenesTrain.push_back(train[j][0]);   
                }

                //casteo para que ande con knn que ahora toma double
                Matriz<double> imagenesDeTrain(CIMAGENESTRAIN,CPIXELES); /// le faltan los digitos etiquetados de la primera columna

                int imTrain=0;// contador de la fila de Train
                for(int imagen = 0; imagen < CIMAGENESTRAIN; imagen++)
                {                     
                    for(int j=0;j<CPIXELES;j++){ imagenesDeTrain[imTrain][j] = train[imagen][j+1];}
                    imTrain++; 
                }

                vector<double> prediccionDigitos = kNN(k, test, imagenesDeTrain, digitosImagenesTrain);
                if(prediccionDigitos.size() != CIMAGENESTEST){
                    throw runtime_error("La longitud de la prediccion de digitos no es la esperada");
                }
             
                salida<<"ImageId,Label"<<endl;
                for (int i = 0; i < prediccionDigitos.size(); ++i)
                {
                    salida<<(i+1)<<","<<prediccionDigitos[i]<<endl;
                }

	            break;
	        }

        case PCA_KNN:{
              
                	vector<int> digitosImagenesTrain; ///importante este vector tiene los digitos de la etiqueta de los Train          
                	
                    for(int j=0; j<CIMAGENESTRAIN; j++)
                    {                    	
                        digitosImagenesTrain.push_back(train[j][0]); 
                    }           

                	Matriz<double> imagenesDeTrain(CIMAGENESTRAIN,CPIXELES); /// le faltan los digitos etiquetados de la primera columna
                	int imTrain=0;// contador de la fila de Train
                	int imTest=0;// contador de la fila de Test
                    
                              
                    for(int imagen = 0; imagen < CIMAGENESTRAIN; imagen++)
                    {
                            for(int j=0;j<CPIXELES;j++){ imagenesDeTrain[imTrain][j] = train[imagen][j+1];}
                            imTrain++;
                 
                    }

                    Matriz <double> promedioImagenesTrain(1,CPIXELES);
            
                    ///calculo el promedio train
                    for(int j=0;j<CPIXELES;j++)
                    {
                    	promedioImagenesTrain[0][j] = 0;
                        for(int i=0;i<imagenesDeTrain.filas();i++){ promedioImagenesTrain[0][j] += imagenesDeTrain[i][j];}
                        promedioImagenesTrain[0][j] = promedioImagenesTrain[0][j]/CIMAGENESTRAIN;
                    }

                    ///dividimos para calcular el promedio de las filas
                    double cteTrain=1.0/ ((double) CIMAGENESTRAIN);
                                   
                    for (int j=0; j<CPIXELES; j++)
                    {
                        for(int fila=0; fila<CIMAGENESTRAIN;fila++)
                        {
                            imagenesDeTrain[fila][j] =imagenesDeTrain[fila][j]- promedioImagenesTrain[0][j];
                       		imagenesDeTrain[fila][j] = imagenesDeTrain[fila][j]/(sqrt(CIMAGENESTRAIN -1));
                        }
                    }

                    Matriz<double> covarianza(CPIXELES, CPIXELES);
                    Matriz<double> traspuestaTrain(CPIXELES, CIMAGENESTRAIN); //ANTES DECIA CIMAGENESTEST ESO ESTA BIEN?

                    traspuestaTrain= imagenesDeTrain.traspuesta();
         
                    covarianza= traspuestaTrain * imagenesDeTrain;
       
                    /// ahora que tenemos la matriz de covarianza aplicamos el metodo de la potencia

                    vector<double> valoresSingulares;
                    Matriz<double> P(covarianza.filas(), alpha);
                    Matriz<double> v(CPIXELES,1);

                    for(int i=0;i<CPIXELES;i++){v[i][0]=2;}

                    valoresSingulares= metodoPotencias(covarianza,alpha,P,v);         
                            
                    // matriz con las transformaciones caracteristicas para las imagenes de train
    
                    Matriz<double> tcTrain(imagenesDeTrain.filas(), alpha);
                    for(int i = 0; i < tcTrain.filas(); i++)
                    {
                        for(int j = 0; j < tcTrain.columnas(); j++)
                            {
                                tcTrain[i][j] = 0;
                                for(int Im_c = 0; Im_c < imagenesDeTrain.columnas(); Im_c++ )
                                {
                                    tcTrain[i][j] = tcTrain[i][j] + P[Im_c][j] + imagenesDeTrain[i][Im_c]; 
                                }        
                            }
                    } 
                    
                    // matriz con las transformaciones caracteristicas para las imagenes de test

                    for (int j=0; j<CPIXELES; j++)//resto la media de las de train
                    {
                        for(int fila=0; fila<test.filas();fila++)
                        {
                            test[fila][j] =test[fila][j]- promedioImagenesTrain[0][j];
                        	test[fila][j] = test[fila][j]/(sqrt(CIMAGENESTRAIN -1));
                        }
                    }

                    Matriz<double> tcTest(test.filas(), alpha); // y ahora la tc
                    for(int i = 0; i < tcTest.filas(); i++)
                    {
                        for(int j = 0; j < tcTest.columnas(); j++)
                            {
                                tcTest[i][j] = 0;
                                for(int Im_c = 0; Im_c < test.columnas(); Im_c++ )
                                {
                                    tcTest[i][j] = tcTest[i][j] + P[Im_c][j] + test[i][Im_c]; 
                                }        
                            }
                    }



                    vector<double> prediccionDigitos = kNN(k, tcTest, tcTrain, digitosImagenesTrain);
                    if(prediccionDigitos.size() != CIMAGENESTEST){
                        throw runtime_error("La longitud de la prediccion de digitos no es la esperada");
                    }

                    salida<<"ImageId,Label"<<endl;
                    for (int i = 0; i < prediccionDigitos.size(); ++i)
                    {
                        salida<<(i+1)<<","<<prediccionDigitos[i]<<endl;
                    }


                break;
        	}

        default:
            throw runtime_error("No existe ese metodo");
    }

    return 0;
}


void cargarMatrizTrain(Matriz<int>& train)
{
    ifstream baseTrain("../data/trainChico.csv", std::ifstream::in);
    string lineaNombrePixel;
    getline(baseTrain, lineaNombrePixel); //la linea de la entrada avanzo

   // cout<<lineaNombrePixel<<endl;

    int filaM = 0;
    for (string linea; getline(baseTrain, linea);)
    {
        vector<string> valores = separarComa(linea);

        if (valores.size() != CPIXELESYETIQUETA) {
            throw runtime_error("Se esperaban CPIXELESYETIQUETA valores en línea ");
        }

        for(int colM = 0; colM < CPIXELESYETIQUETA; colM++){

            train[filaM][colM] = string_to_type<int>(valores[colM]);
        }

        filaM++;
    }
}


void cargarMatrizTest(Matriz<double>& test)
{
    ifstream baseTest("../data/testChico.csv", std::ifstream::in);
    string lineaNombrePixel;
    getline(baseTest, lineaNombrePixel); //la linea de la entrada avanzo

   // cout<<lineaNombrePixel<<endl;

    int filaM = 0;
    for (string linea; getline(baseTest, linea);)
    {
        vector<string> valores = separarComa(linea);

        if (valores.size() != CPIXELES) {
            //cout<<valores.size()<<endl;
            throw runtime_error("Se esperaban CPIXELES valores en línea de test ");
        }

        for(int colM = 0; colM < CPIXELES; colM++){

            test[filaM][colM] = string_to_type<double>(valores[colM]);
        }

        filaM++;
    }
}


vector<double> kNN(unsigned int k, Matriz<double> &tcTest, Matriz<double> &tcTrain, vector<int> &digitosImagenesTrain)
{
	vector<double> predicDigitos;
    
    for(int z = 0; z < tcTest.filas(); z++)
    {
        vector<pair<int, int> > normas2AlCuadrado;

        for(int m = 0; m < tcTrain.filas(); m++)
        {
            unsigned int distanciaAlCuadrado = 0;
    		for(int i = 0; i < tcTest.columnas(); i++)
    		{
        		distanciaAlCuadrado += (tcTest[z][i] - tcTrain[m][i]) *(tcTest[z][i] - tcTrain[m][i]);
    		}

            if(normas2AlCuadrado.size() < k) //coloco las primeras k normas
            {
                pair<unsigned int, int> a;
                a.first =  digitosImagenesTrain[m];
                a.second = distanciaAlCuadrado;
                normas2AlCuadrado.push_back(a);
            }
            else
            {
                if(haymayor(normas2AlCuadrado, distanciaAlCuadrado)) //si ya tengo k voy sacando las mayores
                {
                    int pos_mayor = dondemayor(normas2AlCuadrado);
                    normas2AlCuadrado[pos_mayor].first = digitosImagenesTrain[m];
                    normas2AlCuadrado[pos_mayor].second = distanciaAlCuadrado;
                }
            }
        }

        unsigned int digitos[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        for(int t = 0; t < k; t++)
            digitos[normas2AlCuadrado[t].first]++;

        unsigned int ganador = digitos[0];
        for(int x = 0; x < 10; x++)
        {
            if(digitos[x] > digitos[ganador])
                ganador = x;
        }

        predicDigitos.push_back(ganador);

    }

      
    return predicDigitos;
}



unsigned int norma2AlCuadrado(vector<unsigned int> &v1, vector<unsigned int> &v2)
{
    unsigned int n = 0;
    for(int i = 0; i < v1.size(); i++)
    {
        n += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }

    return n;
}

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
