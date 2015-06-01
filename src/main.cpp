#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/time.h>
#include "util.h"
#include <math.h>
#include <algorithm>  

using namespace std;

enum Metodo { VECINO = 0, BILINEAL = 1 };

void usage() { cout << "./tp <input_filename> <K > <metodo>" << endl; }


int calculo_bilineal(int k, int dato_c, int dato_f, int i, int j, vector<vector <int> > &expandida)
{
	int limite_c = expandida[0].size() - 1;
	int limite_f = expandida.size() - 1;

	int datoSig_c, datoSig_f;

	if(dato_c + k + 1 <= limite_c) {datoSig_c = dato_c + k +1;}
	else {datoSig_c = limite_c;}

	if(dato_f + k + 1 <= limite_f) {datoSig_f = dato_f + k +1;}
	else {datoSig_f = limite_f;}

	int f_r2 = ((datoSig_c - j)/k)*expandida[dato_f][dato_c] + ((j - dato_c)/k)*expandida[dato_f][datoSig_c];
	int f_r1 = ((datoSig_c - j)/k)*expandida[datoSig_f][dato_c] + ((j - dato_c)/k)*expandida[datoSig_f][datoSig_c];

	int res = ((i - dato_f)/k)*f_r1 + ((datoSig_f - i)/k)*f_r2; //los numeradores estan al reves pero es porque la i aumenta a medida
	//que descendemos las filas lo que es inverso a un eje de coordenadas
	if(res < 255) {return res;}
	else {return 254;} //no estoy seguro de esto
}	

	

int mas_cercano(int k, int dato_c, int dato_f, int i, int j, vector<vector <int> > &expandida)
{
	int datoSig_c = dato_c + k +1;
	int datoSig_f = dato_f + k + 1;
	if(dato_f == i) 
	{
		if(abs(j - dato_c) < abs(j - datoSig_c)) {return expandida[dato_f][dato_c];}
		else {return expandida[dato_f][datoSig_c];}
	}
	if(dato_c == j)
	{
		if(abs(i - dato_f) < abs(i - datoSig_f)) {return expandida[dato_f][dato_c];}
		else {return expandida[datoSig_f][dato_c];}	
	}	
	else
	{
		int a = (dato_c - j)*(dato_c - j) + (dato_f - i)*(dato_f - i);
		int b = (datoSig_c - j)*(datoSig_c - j) + (dato_f - i)*(dato_f - i);
		int c = (datoSig_f - i)*(datoSig_f - i) + (dato_c - j)*(dato_c - j);
		int d = (datoSig_f - i)*(datoSig_f - i) + (datoSig_c - j)*(datoSig_c - j);
		int cercano = min(min(a, b), min(c, d));

		if(cercano == a) {return expandida[dato_f][dato_c];}
		else
		{
			if (cercano == b) {return expandida[dato_f][datoSig_c];}
			else
			{
				if(cercano == c) {return expandida[datoSig_f][dato_c];}
				else
				{
					return expandida[datoSig_f][datoSig_c];	
				}	
			}
       	}	
	}
}	


int main(int argc, char *argv[])
{
	if(argc != 4) { //archivo_input ; k; metodo
		usage();
		return 0;
	}

	string matlabCommand1 = "matlab -nodisplay -nosplash -nojvm -r \"Image2Csv('";
	matlabCommand1 += argv[1];
	matlabCommand1 += "');quit\"";

	system(matlabCommand1.c_str());

	string newFile = "";
	newFile += argv[1];
	newFile += ".csv";

	ifstream entrada(newFile.c_str(),std::ifstream::in); //la matriz de entrada

	int k = string_to_type<unsigned int>(argv[2]);
    
	Metodo metodo = (Metodo)string_to_type<int>(argv[3]);

    vector<vector<int> > img_reducida; // matriz con las particiones a realizar

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
/////////////cargada em img_reducida////////////////
    vector<vector <int> > expandida;

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


    ////////////////esta "ampliada" //////////////////////////

    switch(metodo){

    	case VECINO:
    	{
	 
		    int dato_f, dato_c; 
		    for(int i = 0; i < expandida.size(); i++)
		    {
		    	dato_c = 0;
		    	for(int j = 0; j < expandida[i].size() ; j++)
		    	{
		    		
		    		if(expandida[i][j] != -1) {dato_f = i; dato_c = j;}
		    		else
		    		{
		    			if(j == dato_c + k + 1) {dato_c = j;}
		    			expandida[i][j] = mas_cercano(k, dato_c, dato_f, i, j, expandida);	
		    		}
		    	}
		    }

		    ///////////muestro
		   
			ofstream salida("salida.csv",std::ofstream::out);// genero este nuevo archivo de salda con el formato pero el vector tiene valores cero
	        
	        for(int i = 0; i < expandida.size(); i++)
	        {
	        	for(int j = 0; j < expandida[i].size(); j++)
	        	{
	        		salida << expandida[i][j] << " ";
	        	}
	        	salida << endl;
	        } 

	        string matlabCommand2 = "matlab -nodisplay -nosplash -nojvm -r \"ImageTxtToBmp('salida.csv', '";
			matlabCommand2 += argv[1];
			matlabCommand2 += "Output.bmp');quit\"";

			system(matlabCommand2.c_str());
		
	        break;
		}

		case BILINEAL:
    	{
	 
		    int dato_f, dato_c; 
		    for(int i = 0; i < expandida.size(); i++)
		    {
		    	dato_c = 0;
		    	for(int j = 0; j < expandida[i].size() ; j++)
		    	{
		    		if(expandida[i][j] != -1 && i != expandida.size() - 1) {dato_f = i; dato_c = j;}
		    		else
		    		{
		    			if(j == dato_c + k + 1 && j != expandida[i].size() -1) {dato_c = j;}
		    			expandida[i][j] = calculo_bilineal(k, dato_c, dato_f, i, j, expandida);	
		    		}
		    	}
		    }

		    ///////////muestro
		   
			ofstream salida("salida1.csv",std::ofstream::out);// genero este nuevo archivo de salda con el formato pero el vector tiene valores cero
	        
	        for(int i = 0; i < expandida.size(); i++)
	        {
	        	for(int j = 0; j < expandida[i].size(); j++)
	        	{
	        		salida << expandida[i][j] << " ";
	        	}
	        	salida << endl;
	        } 
		
	        break;
		}
		
		default:
            throw runtime_error("No existe ese metodo");
    }
    return  0;

}       
      