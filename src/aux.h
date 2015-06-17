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

void usageBilineal() { cout << "./tp <input_filename> <K > <metodo> <submetodoBilineal>" << endl; }

void grabarImagen(char* str, vector<vector<int> >& expandida)
{
    cout << "Tiempo de computo "<<get_time()<<endl;

	ofstream salida("salida.csv",std::ofstream::out);// genero este nuevo archivo de salda con el formato pero el vector tiene valores cero

    for(int i = 0; i < expandida.size(); i++)
    {
    	for(int j = 0; j < expandida[i].size(); j++)
    	{
    		salida << expandida[i][j] << " ";
    	}
    	salida << endl;
    }

	string matlabCommand2 = "nohup matlab -nodisplay -nosplash -nojvm -r \"ImageTxtToBmp('salida.csv', '";
	matlabCommand2 += str;
	matlabCommand2 += "Output.tiff');quit\"";

	system(matlabCommand2.c_str());
    system("rm nohup.out");
    system("rm salida.csv");
}

void preprocesarImagen(string newFile, int k, vector<vector<int> >& expandida)
{
	ifstream entrada(newFile.c_str(),std::ifstream::in); //la matriz de entrada

	vector<vector<int> > img_reducida;

	for (string linea; getline(entrada, linea); ) //tengo la imagen "original cargada"
    {
        vector<string> valores = separar(linea);

		vector<int> fila_img(valores.size());

        for(int c =0; c < valores.size() ;c++)
        {
        	fila_img[c] = string_to_type<int>(valores[c]);
        }

        img_reducida.push_back(fila_img);
    }

    for(int i = 0; i < img_reducida.size(); i++)
    {
    	vector<int> fila_exp;
		int j = 0;
    	for(; j < img_reducida[i].size() -1 ; j++)
    	{
    		fila_exp.push_back(img_reducida[i][j]);

    		for(int j_2 = 0; j_2 < k; j_2++)
    		{
    			fila_exp.push_back(-1);
    		}
    	}
    	fila_exp.push_back(img_reducida[i][j]);
    	expandida.push_back(fila_exp);

    	for(int i_2 = 0; i_2 < k && i < img_reducida.size() -1; i_2++)
    	{
    		vector<int> vacio;
    		for (int j_v = 0; j_v < (img_reducida[i].size() -1)*k + img_reducida[i].size(); j_v++)
    		{
    			vacio.push_back(-1);
    		}
    		expandida.push_back(vacio);
    	}
    }
}
