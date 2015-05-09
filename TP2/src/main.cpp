#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/time.h>
#include "util.h"
#include "Matriz.h"
#include "mPotencia.h"
#include <math.h>
using namespace std;

const int CIMAGENES=5;
const int CPIXELES=5;
const int CPIXELESYETIQUETA=6;
enum Metodo { KNN = 0, PCA_KNN = 1};
void usage() { cout << "./tp <input_filename> <output_filename> <metodo>" << endl; }

bool haymayor(vector<pair<int, int> >  &normas2AlCuadrado,  unsigned int distanciaAlCuadrado);
int dondemayor(vector<pair<int, int> >  &normas2AlCuadrado);
void cargarMatrizTrain(Matriz<int>& train);
unsigned int norma2AlCuadrado(vector<unsigned int> &v1, vector<unsigned int> &v2);
double kNN2(unsigned int k, Matriz<double> &tcTest, Matriz<double> &tcTrain, vector<int>  &digitosImagenesTest, vector<int> &digitosImagenesTrain);

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


int main(int argc, char *argv[])
{
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
    for (string linea; getline(entrada, linea); num_linea++)
    {
        vector<string> valores = separar(linea);

        if (valores.size() != CIMAGENES)
        {
            throw runtime_error("Se esperaban CIMAGENES valores en línea ");
        }

        for(int colM=0;colM<CIMAGENES;colM++)
        {
            Klineas[filaM][colM] = string_to_type<bool>(valores[colM]);
        }

        filaM++;
    }

    Matriz<int> train(CIMAGENES, CPIXELESYETIQUETA);     //42k de imagenes, cada imagen son 784 valores y un valor mas que representa el digito al que corresponde

    cargarMatrizTrain(train);

    switch(metodo){

        case KNN:{

        		vector<double> tasaDeReco;

                for(int i = 0; i < K; i++)
                {
       				vector<int> digitosImagenesTrain; ///importante este vector tiene los digitos de la etiqueta de los Train
                	vector<int> digitosImagenesTest; ///importante este vector tiene los digitos de la etiqueta de los Test, parece fea la implementacion, pero no jode
                    unsigned int cantidadImagenesTrain=0;
                    for(int j=0; j<Klineas.columnas(); j++)
                    {
                        if(Klineas[i][j])
                        {
                            cantidadImagenesTrain++;
                            digitosImagenesTrain.push_back(train[j][0]);
                        }
                        else
                        {
                            digitosImagenesTest.push_back(train[j][0]);
                        }
                    }

                    unsigned int cantidadImagenesTest=CIMAGENES-cantidadImagenesTrain;

                    Matriz<double> imagenesDeTrain(cantidadImagenesTrain,CPIXELES); /// le faltan los digitos etiquetados de la primera columna

                    Matriz<double> imagenesDeTest(cantidadImagenesTest,CPIXELES);

                    int imTrain=0;// contador de la fila de Train
                    int imTest=0;// contador de la fila de Test
                    for(int imagen = 0; imagen < CIMAGENES; imagen++)
                    { // cada imagen de train la mandamos a una de las dos matrices segun diga la i
                        if(Klineas[i][imagen])
                        {
                            for(int j=0;j<CPIXELES;j++){ imagenesDeTrain[imTrain][j] = train[imagen][j+1];}
                            imTrain++;
                        }
                        else
                        {
                            for(int j=0;j<CPIXELES;j++){imagenesDeTest[imTest][j] = train[imagen][j+1];}
                            imTest++;
                        }
                    }

                    double tasa = kNN2(k, imagenesDeTest, imagenesDeTrain, digitosImagenesTest, digitosImagenesTrain);
                    tasaDeReco.push_back(tasa);
                }

	            for(int i=0; i<tasaDeReco.size();i++) {cout<<tasaDeReco[i]<<endl;}

	            ofstream tasaReconocimiento("tasaReconocimiento.txt",std::ofstream::out);// genero este nuevo archivo de salda con el formato pero el vector tiene valores cero
	            tasaReconocimiento<<"Particion"<<"\t"<<"Tasa de Reconocimiento\n";

	            for (int i = 0; i < tasaDeReco.size(); ++i){ tasaReconocimiento<<i+1<<"\t\t\t"<<tasaDeReco[i]<<endl;}
	            ///aplicarle knn a la base de test y devover los resultados en el formato que envio agustin por mail
	            break;
	        }

        case PCA_KNN:{

           		salida<<scientific<<setprecision(6);
				vector<double> tasaDeReco;

           	 	for(int particion = 0; particion < 1; particion++)
           	 	{
           	 		vector<int> digitosImagenesTrain; ///importante este vector tiene los digitos de la etiqueta de los Train
          			vector<int> digitosImagenesTest; ///importante este vector tiene los digitos de la etiqueta de los Test, parece fea la implementacion, pero no jode
                //asignamos los digitos(primera columna)
                	unsigned int cantidadImagenesTrain=0;
                	for(int j=0; j<Klineas.columnas(); j++)
                	{
                    	if(Klineas[particion][j])
                    	{
                        	cantidadImagenesTrain++; //problema  este recorrido podria evitarse
                    	    digitosImagenesTrain.push_back(train[j][0]);
                    	}
                   		else
                    	{
                        	digitosImagenesTest.push_back(train[j][0]);
                    	}
                	}

                	unsigned int cantidadImagenesTest=CIMAGENES-cantidadImagenesTrain;

                	Matriz<double> imagenesDeTrain(cantidadImagenesTrain,CPIXELES); /// le faltan los digitos etiquetados de la primera columna

                	Matriz <double> imagenesDeTest(cantidadImagenesTest,CPIXELES);

                	int imTrain=0;// contador de la fila de Train
                	int imTest=0;// contador de la fila de Test
                	Matriz <double> promedioImagenes(1,CPIXELES);
                	for(int imagen = 0; imagen < CIMAGENES; imagen++)
                	{ // cada imagen de train la mandamos a una de las dos matrices segun diga la particion
                    	if(Klineas[particion][imagen])
                    	{
                        /// copiamos en imagenesDeTrain la imagen de train
                       		for(int j=0;j<CPIXELES;j++){ imagenesDeTrain[imTrain][j] = train[imagen][j+1];}
                       		imTrain++;
                    	}
                    	else
                    	{
                        /// copiamos en imagenesDeTest la imagen de train
	                        for(int j=0;j<CPIXELES;j++){imagenesDeTest[imTest][j] = train[imagen][j+1];}
    	                    imTest++;
        	            }
                	}

                	cout<<"imagenesTest\n";
                	imagenesDeTest.display();
                	cout<<endl;
                	cout<<"imagenesTrain\n";
                	imagenesDeTrain.display();
                    cout<<endl;
                	///calculo el promedio
                	for(int j=0;j<CPIXELES;j++)
                    {
                        promedioImagenes[0][j] = 0;
                        for(int i=0;i<imagenesDeTrain.filas();i++){ promedioImagenes[0][j] = promedioImagenes[0][j] + imagenesDeTrain[i][j];}
                        promedioImagenes[0][j] = promedioImagenes[0][j]/cantidadImagenesTrain;
                    }

                	cout<<"promedioImagenes\n";

                    promedioImagenes.display();
        	        ///resto a cada imagen del nuevo train el promedio, modifico imagenesDeTrain(se podria haber copiado)
                    cout<<endl;

                	Matriz<double> imagenesDeTrainRP(imagenesDeTrain.filas(), imagenesDeTrain.columnas());


                    for(int fila=0; fila<cantidadImagenesTrain;fila++)
                    {

                        for (int j=0; j<CPIXELES; j++)
                        {
                            imagenesDeTrainRP[fila][j] =imagenesDeTrain[fila][j]- promedioImagenes[0][j];
                            imagenesDeTrainRP[fila][j] = imagenesDeTrainRP[fila][j]/(sqrt(cantidadImagenesTrain -1));
                        }
                    }

                    //double cteCov= (double) 1/((double) sqrt(cantidadImagenesTrain -1));

                  //  imagenesDeTrainRP=imagenesDeTrainRP*cteCov;



                	cout<<"imagenesDeTrainRP\n";

                	imagenesDeTrainRP.display();

                	cout<<endl;
                	Matriz<double> covarianza(CPIXELES, CPIXELES);
                	Matriz<double> traspuestaTrainRP(CPIXELES, cantidadImagenesTrain);


                	traspuestaTrainRP= imagenesDeTrainRP.traspuesta();

                	covarianza= traspuestaTrainRP * imagenesDeTrainRP;

                	cout<<"matriz de covarianza\n";
                	covarianza.display();
                	cout<<endl;

       		        /// ahora que tenemos la matriz de covarianza aplicamos el metodo de la potencia
                	vector<double> valoresSingulares;
               		Matriz<double> P(covarianza.filas(), alpha);
                	Matriz<double> v(CPIXELES,1);

                	for(int i=0;i<CPIXELES;i++){v[i][0]=2;}

                	valoresSingulares= metodoPotencias(covarianza,alpha, P,v);
                	cout<<"valoresSingulares:\n";

                	for(int i=0;i<valoresSingulares.size();i++)
                	{
                    	cout<<sqrt(valoresSingulares[i])<<endl;
                    	salida<<sqrt(valoresSingulares[i])<<endl;
               		}

                        cout<<"P\n";
                        P.display();
                        cout<<endl;
                	/// ahora que tenemos la matriz de covarianza, y sus autovectores, calculamos tc


                 ///le restamos a las de test el promedio

                	Matriz<double> imagenesDeTestRP(imagenesDeTest.filas(), imagenesDeTest.columnas());


                    for(int fila=0; fila<cantidadImagenesTest;fila++)
                    {
                        for (int j=0; j<CPIXELES; j++)
                        {
                            imagenesDeTestRP[fila][j] = imagenesDeTest[fila][j] - promedioImagenes[0][j];
                            imagenesDeTestRP[fila][j] = imagenesDeTestRP[fila][j]/(sqrt(cantidadImagenesTrain -1));
                        }
                    }

                    imagenesDeTestRP.display();


                    ///cambio de base

                	Matriz<double> tcTrain(imagenesDeTrainRP.filas(), alpha);

                	tcTrain=imagenesDeTrainRP*P;



                    Matriz<double> tcTest(imagenesDeTestRP.filas(), alpha);

                    tcTest=imagenesDeTestRP*P;


                    cout<<"tcTest\n";
                    tcTest.display();
                    cout<<endl;

                    cout<<"tcTrain\n";
                    tcTrain.display();
                    cout<<endl;


		    /*		Matriz<double> tcTrain(imagenesDeTrain.filas(), alpha);
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
                    	for(int fila=0; fila<imagenesDeTest.filas();fila++)
                    	{
                        	imagenesDeTest[fila][j] =imagenesDeTest[fila][j]- promedioImagenes[0][j];
                            imagenesDeTest[fila][j] =imagenesDeTest[fila][j]/(sqrt(cantidadImagenesTrain -1));
                    	}
                	}

		    		Matriz<double> tcTest(imagenesDeTest.filas(), alpha); // y ahora la tc
		    		for(int i = 0; i < tcTest.filas(); i++)
		    		{
		    			for(int j = 0; j < tcTest.columnas(); j++)
		    				{
		    					tcTest[i][j] = 0;
		    					for(int Im_c = 0; Im_c < imagenesDeTest.columnas(); Im_c++ )
		    					{
		   							tcTest[i][j] = tcTest[i][j] + P[Im_c][j] + imagenesDeTest[i][Im_c];
		   						}
		    				}
		    		}
					*/



					double tasa = kNN2(k, tcTest, tcTrain, digitosImagenesTest, digitosImagenesTrain);
                    tasaDeReco.push_back(tasa);

                    cout<<"FIN PARTICION\n";

            }



				cout<<"escribiendo resultados\n";

				for(int i=0; i<tasaDeReco.size();i++) {cout<<tasaDeReco[i]<<endl;}

	            ofstream tasaReconocimiento("tasaReconocimiento.txt",std::ofstream::out);// genero este nuevo archivo de salda con el formato pero el vector tiene valores cero
	            tasaReconocimiento<<"Particion"<<"\t"<<"Tasa de Reconocimiento\n";

	            for (int i = 0; i < tasaDeReco.size(); i++){ tasaReconocimiento<<i+1<<"\t\t\t"<<tasaDeReco[i]<<endl;}
                cout<<"se terminaron de escribir resultados\n";
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

double kNN2(unsigned int k, Matriz<double> &tcTest, Matriz<double> &tcTrain, vector<int>  &digitosImagenesTest, vector<int> &digitosImagenesTrain)
{
	double tasaDeReconocimiento;
    unsigned int reconocidos = 0;
    for(int z = 0; z < tcTest.filas(); z++)
    {/// z imagenes test
        vector<pair<int, int> > normas2AlCuadrado; /// almaceno los k vecimos mas cercanos, su distancia y su digito
        cout<<"digito a reconocer: "<<digitosImagenesTest[z]<<endl<<endl;
        cout<<"se copara con \n";
        cout<<"[ ";
        for(int m = 0; m < tcTrain.filas(); m++)
        {/// m imagenes de train

        cout<<"("<<digitosImagenesTrain[m]<<",";


            ///calculo la distancia al cuadrado entre la imagen de test y la de train
            unsigned int distanciaAlCuadrado = 0;
    		for(int j = 0; j < tcTest.columnas(); j++)
    		{
        		distanciaAlCuadrado += (tcTest[z][j] - tcTrain[m][j]) *(tcTest[z][j] - tcTrain[m][j]);
    		}
        cout<<distanciaAlCuadrado<<"),";
    		/// en caso de ser cercana la almaceno

            if(normas2AlCuadrado.size() < k) //coloco las primeras k normas
            {
                pair<unsigned int, int> a;
                a.first =  digitosImagenesTrain[m]; //almaceno el digito con su distancia
                a.second = distanciaAlCuadrado;
                normas2AlCuadrado.push_back(a);
            }
            else
            {/// si  en normas2alcuadrado hay uno mayor, el elemento se transorma en un elemento de los vecinos mas cercanos
                if(haymayor(normas2AlCuadrado, distanciaAlCuadrado)) //si ya tengo k voy sacando las mayores
                {
                    int pos_mayor = dondemayor(normas2AlCuadrado);
                    normas2AlCuadrado[pos_mayor].first = digitosImagenesTrain[m];
                    normas2AlCuadrado[pos_mayor].second = distanciaAlCuadrado;
                }
            }
            cout<<"  vectorconKminimos: {";
            for(int i=0;i<normas2AlCuadrado.size();i++){

            cout<<normas2AlCuadrado[i].first<<", ";

            }
            cout<<"}   ";

        }


        cout<<"]\n";

        /// una vez que encontre los k vecinos mas cercanos, me quedo con el digito que mas veces aparecio

        unsigned int digitos[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

        for(int t = 0; t < k; t++){
            digitos[normas2AlCuadrado[t].first]++;
        }

        unsigned int ganador = digitos[0];
        for(int x = 0; x < 10; x++)
        {
            if(digitos[x] > digitos[ganador])
                ganador = x;
        }

        if(ganador == digitosImagenesTest[z])
          {  reconocidos++;}
        ///si ese digito de test fue reconocido, aumento la candidad de reconocidos
       cout<<"digitos reconocidos: \n";
        cout<<"[";

        for(int i=0;i<10;i++){
            cout<<digitos[i]<<", ";
        }
cout<<"]\n";
    }

    ///luego de consultar todos los tests, calculo la tasa de reconocimiento

        tasaDeReconocimiento = (double)reconocidos/(double)tcTest.filas();
        return tasaDeReconocimiento;
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
