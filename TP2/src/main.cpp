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


void cargarMatrizTrain(Matriz<int>& train);
vector<double> kNN(unsigned int K, unsigned int k, Matriz<bool>& Klineas, Matriz<int>& train);
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

const int CIMAGENES=42000;
const int CPIXELES=784;
const int CPIXELESYETIQUETA=785;

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
        throw runtime_error("Se esperaban 4 valores en líneaaaa ");
    }

    path = valoresIniciales[0];
    k = string_to_type<unsigned int>(valoresIniciales[1]);
    alpha = string_to_type<unsigned int>(valoresIniciales[2]);
    K = string_to_type<unsigned int>(valoresIniciales[3]);

    Matriz<bool> Klineas(K,CIMAGENES); // matriz con las particiones a realizar
    int filaM=0;  // fila de la matriz

   //se empieza a analizar desde la segunda linea
    for (string linea; getline(entrada, linea); num_linea++) {

        vector<string> valores = separar(linea);

                if (valores.size() != CIMAGENES) {
                    throw runtime_error("Se esperaban CIMAGENES valores en línea ");
                }

                for(int colM=0;colM<CIMAGENES;colM++){

                    Klineas[filaM][colM] = string_to_type<bool>(valores[colM]);
                }

                filaM++;
    }

    Matriz<int> train(CIMAGENES, CPIXELESYETIQUETA);     //42k de imagenes, cada imagen son 784 valores y un valor mas que representa el digito al que corresponde

    cargarMatrizTrain(train);

    switch(metodo){

        case KNN:{

            vector<double> res = kNN(K, k, Klineas, train);
            //en pantalla
            for(int i=0; i<res.size();i++){
            cout<<res[i]<<endl;
            }

            ofstream tasaReconocimiento("tasaReconocimiento.txt",std::ofstream::out);// genero este nuevo archivo de salda con el formato pero el vector tiene valores cero
            tasaReconocimiento<<"Particion"<<"\t"<<"Tasa de Reconocimiento\n";

            for (int i = 0; i < res.size(); ++i){
            tasaReconocimiento<<i+1<<"\t\t\t"<<res[i]<<endl;
            }

            ///aplicarle knn a la base de test y devover los resultados en el formato que envio agustin por mail
            break;
        }

        case PCA_KNN: {

            vector<int> digitosImagenesTrain; ///importante este vector tiene los digitos de la etiqueta de los Train

            vector<int> digitosImagenesTest; ///importante este vector tiene los digitos de la etiqueta de los Test, parece fea la implementacion, pero no jode

            salida<<scientific<<setprecision(6);


            for(int particion = 0; particion < K; particion++){

                //asignamos los digitos(primera columna)
                unsigned int cantidadImagenesTrain=0;
                for(int j=0; j<Klineas.columnas(); j++){

                    if(Klineas[particion][j]){
                        cantidadImagenesTrain++; //problema  este recorrido podria evitarse
                        digitosImagenesTrain.push_back(train[j][0]);
                    }
                    else{
                        digitosImagenesTest.push_back(train[j][0]);
                    }
                }

                unsigned int cantidadImagenesTest=CIMAGENES-cantidadImagenesTrain;

                Matriz<double> imagenesDeTrain(cantidadImagenesTrain,CPIXELES); /// le faltan los digitos etiquetados de la primera columna

                Matriz <int> imagenesDeTest(cantidadImagenesTest,CPIXELES);

                int imTrain=0;// contador de la fila de Train
                int imTest=0;// contador de la fila de Test
                Matriz <double> promedioImagenes(1,CPIXELES);
                for(int imagen = 0; imagen < CIMAGENES; imagen++){ // cada imagen de train la mandamos a una de las dos matrices segun diga la particion
                    if(Klineas[particion][imagen]) {
                        /// copiamos en imagenesDeTrain la imagen de train
                       for(int j=0;j<CPIXELES;j++){
                            imagenesDeTrain[imTrain][j] = train[imagen][j+1];
                       }
                       imTrain++;
                    }
                    else {
                        /// copiamos en imagenesDeTest la imagen de train
                        for(int j=0;j<CPIXELES;j++){
                            imagenesDeTest[imTest][j] = train[imagen][j+1];
                        }
                        imTest++;
                    }
                }

         //       cout<<"imagenesDeTest\n ";
          //     imagenesDeTest.display();
            //    cout<<"imagenesDeTrain "<<endl;
           //    imagenesDeTrain.display();
                ///calculo el promedio
                for(int j=0;j<CPIXELES;j++){
                    for(int i=0;i<imagenesDeTrain.filas();i++){
                        promedioImagenes[0][j] += imagenesDeTrain[i][j];
                    }
                }

             //   promedioImagenes.display();

                ///dividimos para calcular el promedio de las filas
                double cte=1.0/ ((double) cantidadImagenesTrain);
                promedioImagenes=promedioImagenes * cte;
        //       promedioImagenes.display();
        //        cout<<"promedio de imagenes\n";
        //    promedioImagenes.display();
                ///resto a cada imagen del nuevo train el promedio, modifico imagenesDeTrain(se podria haber copiado)

                for (int j=0; j<CPIXELES; j++){
                    for(int fila=0; fila<cantidadImagenesTrain;fila++){
                        imagenesDeTrain[fila][j] =imagenesDeTrain[fila][j]- promedioImagenes[0][j];
                    }
                }

                Matriz<double> covarianza(CPIXELES, CPIXELES);
                Matriz<double> traspuestaTrain(CPIXELES, cantidadImagenesTrain);
        //cout<<"imagenesTrain\n";
           //     imagenesDeTrain.display();
                traspuestaTrain= imagenesDeTrain.traspuesta();
           //     traspuestaTrain.display();
                covarianza= traspuestaTrain * imagenesDeTrain;
                double cteCov=1.0/ ((double) (cantidadImagenesTrain-1));
        //cout<<"constante covarianza "<<cteCov<<endl;
                covarianza = covarianza * cteCov; // esta es la matriz de covarianza
        //      cout<< "matriz de covarianza: \n";
        //        covarianza.display();
                /// ahora que tenemos la matriz de covarianza aplicamos el metodo de la potencia

                vector<double> valoresSingulares;
                Matriz<double> P(covarianza.filas(), alpha);
                Matriz<double> v(CPIXELES,1);

                for(int i=0;i<CPIXELES;i++){

                    v[i][0]=2;
                }


                cout<<"aplicando metodo potencias\n";
                valoresSingulares= metodoPotencias(covarianza,alpha,300,P,v);
                cout<<"escribiendo valoresSingulares\n";
                for(int i=0;i<valoresSingulares.size();i++){
        //     cout<<valoresSingulares[i]<<endl;
                    salida<<sqrt(valoresSingulares[i])<<endl;
               }

        // P.display();

                cout<<"fin de particion\n";
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
    ifstream baseTrain("../data/train.csv", std::ifstream::in);
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

vector<double> kNN(unsigned int K, unsigned int k, Matriz<bool>& Klineas, Matriz<int>& train)
{
    vector<double> tasaDeReconocimiento;

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

                if(ganador == imagenesDeTrainParaReconocer[z].first)
                    reconocidos++;

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
