#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/time.h>
#include "util.h"
#include <math.h>
#include <algorithm>  

using namespace std;

enum Metodo { VECINO = 0, BILINEAL = 1, BILINEALBIS = 2, SPLINE = 3};

void usage() { cout << "./tp <input_filename> <K > <metodo>" << endl; }

struct Spline 
{
	vector<double> a, b, c, d;

	Spline(vector<double> _a, vector<double> _b, vector<double> _c, vector<double> _d) : a(_a), b(_b), c(_c), d(_d) {  }
};

//implementacion del algoritmo del burden pagina 149/150
Spline dame_Spline(vector<int>& x, vector<double>& a, unsigned int k)
{
	unsigned int n = x.size() - 1;
	double h_i = k + 1;

	for(int i = 1; i < n; i++)
		a[i] = ((double)3 / h_i)*(a[i + 1] - 2 * a[i] + a[i - 1]);

	vector<double> l(n + 1, 0), u(n + 1, 0), z(n + 1, 0);
	l[0] = 1;

	for(int i = 1; i < n; i++)
	{
		l[i] = h_i * ((double)4 - u[i - 1]);
		u[i] = h_i / l[i];
		z[i] = (a[i] - h_i*z[i - 1]) / l[i];
	}

	vector<double> c(n, 0), b(n, 0), d(n, 0);

	l[n] = 1;
	z[n] = 0;

	for(int j = n - 1; j >= 0; j--)
	{
		c[j] = z[j] - u[j]*c[j + 1];
		b[j] = (a[j + 1] - a[j]) / h_i - h_i * (c[j + 1] + (double)2 * c[j]) / (double)3;
		d[j] = (c[j + 1] - c[j]) / ((double)3 * h_i);
	}

	a.pop_back();

	Spline s(a, b, c, d);

	return s;
}

int calculo_bilineal(int k, int dato_c, int dato_f, int i, int j, vector<vector <int> > &expandida)
{
	int datoSig_c = dato_c + k +1;
	int datoSig_f = dato_f + k +1;

	double f_r2 = (double)((datoSig_c - j)*expandida[dato_f][dato_c])/(double)(k+1) + (double)((j - dato_c)*expandida[dato_f][datoSig_c])/(double)(k+1);
	double f_r1 = (double)((datoSig_c - j)*expandida[datoSig_f][dato_c])/(double)(k+1) + (double)((j - dato_c)*expandida[datoSig_f][datoSig_c])/(double)(k+1);
	
	int res = (double)((i - dato_f)*f_r1)/(double)(k+1) + (double)((datoSig_f - i)*f_r2)/(double)(k+1); //los numeradores estan al reves pero es porque la i aumenta a medida
	
	//que descendemos las filas lo que es inverso a un eje de coordenadas
	if(res < 255) {return res;}
	else {return 255;} //no estoy seguro de esto
}

int calculo_bilineal_por_filas(int k, int dato_c, int dato_f, int i, int j, vector<vector <int> > &expandida)
{
	int limite_c = expandida[0].size() - 1;
	int datoSig_c;

	if(dato_c + k + 1 <= limite_c) {datoSig_c = dato_c + k +1;}
	else {datoSig_c = limite_c;}

	int res = (int)((double)expandida[i][dato_c] + (((double)expandida[i][datoSig_c] - (double)expandida[i][dato_c])/(double)(k+1))*((double)j-(double)dato_c));

	if(res < 255) {return res;}
	else {return 255;} //no estoy seguro de esto
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

	string matlabCommand1 = "nohup matlab -nodisplay -nosplash -nojvm -r \"Image2Csv('";
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

	        string matlabCommand2 = "nohup matlab -nodisplay -nosplash -nojvm -r \"ImageTxtToBmp('salida.csv', '";
			matlabCommand2 += argv[1];
			matlabCommand2 += "Output.bmp');quit\"";

			system(matlabCommand2.c_str());
		
	        break;
		}

		case SPLINE:
    	{
    		vector<Spline> splinesPorFila, splinesPorColumna;

    		//me armo un spline por cada fila que contiene pixeles originales (es decir las filas que tienen todos -1 no las uso)
		    for(int i = 0; i < expandida.size(); i += k + 1)
		    {
		    	vector<int> x;
		    	vector<double> a;

		    	for(int j = 0; j < expandida[i].size(); j += k + 1)
		    	{
		    		x.push_back(j);
		    		a.push_back(expandida[i][j]);
		    	}
		 
		    	splinesPorFila.push_back(dame_Spline(x, a, k));
		    }

		    //recorro cada una de las filas que tienen pixeles originales para calcular los pixeles que hay entre medio de cada par de
		    //pixeles originales (es decir los que tienen -1)
		    for(int i = 0; i < expandida.size(); i += k + 1)		//salto de a k + 1 filas para no tocar las que tienen todos -1
		    {
		    	for(int j = 1; j < expandida[i].size(); j += k + 1)	//salto de a k + 1 columnas para no tocar los pixeles originales, con el proximo for voy rellenando los pixeles nuevos entre cada par de pixeles originales
		    	{
		    		int p = j - 1;		//p va a ser el punto "anterior" al punto que quiero calcular usando interpolacion, es decir si quiero calcular S(x) (S es el spline) con x el punto con el -1, p seria el punto correspondiente al pixel original anterior a x

		    		for(int t = 0; t < k; t++)
		    		{
		    			//como voy saltando d a k + 1 filas y para calcular los splines por fila hicimos lo mismo, divido por k + 1 para obtener el spline correspondiente a la fila con la que estoy trabajando

			    		double a_j = splinesPorFila[i / (k + 1)].a[p];
			    		double b_j = splinesPorFila[i / (k + 1)].b[p];
			    		double c_j = splinesPorFila[i / (k + 1)].c[p];
			    		double d_j = splinesPorFila[i / (k + 1)].d[p];

			    		//Lo siguiente es evaluar el polinomio en el punto, en realidad esta simplificando porque:
			    		//t + 1 = j + t - p = j + t - j + 1 = t + 1, donde j + t es el punto que quiero calcular y p es el punto
			    		//anterior. Se calcula asi porque el algoritmo para hallar el spline te lo devuelve expresado de determinada manera,
			    		//que se puede ver en el burden en la pagina 149 donde dice "Note" entre parentesis debajo de OUTPUT
			    		int interpolacion = a_j + b_j * (t + 1) + c_j * (t + 1) * (t + 1) + d_j * (t + 1) * (t + 1) * (t + 1);

			    		if(interpolacion > 255)
			    			interpolacion = 255;
			    		if(interpolacion < 0)
			    			interpolacion = 0;

			    		expandida[i][j + t] = interpolacion;
		    		}
		    	}
		    }


		    //Ahora es simetrico, como por cada fila donde habia pixeles originales ahora estan calculados todos los pixeles nuevos
		    //entre cada par de pixeles originales, faltan calcular los pixeles de las filas que estan entre cada par de filas de
		    //pixeles originales. O sea las filas que tienen todos -1.
		    //Para eso me armo un spline por columna y despues el recorrido y demas es todo simetrico al paso anterior.
		    for(int j = 0; j < expandida[0].size(); j++)
		    {
		    	vector<int> x;
		    	vector<double> a;

		    	for(int i = 0; i < expandida.size(); i += k + 1)
		    	{
		    		x.push_back(i);
		    		a.push_back(expandida[i][j]);
		    	}
		 
		    	splinesPorColumna.push_back(dame_Spline(x, a, k));
		    }

		    for(int j = 0; j < expandida[0].size(); j++)
		    {
		    	for(int i = 1; i < expandida.size(); i += k + 1)
		    	{
		    		int p = i - 1;

		    		for(int t = 0; t < k; t++)
		    		{		    			
		    			int a_i = splinesPorColumna[j].a[p];
			    		int b_i = splinesPorColumna[j].b[p];
			    		int c_i = splinesPorColumna[j].c[p];
			    		int d_i = splinesPorColumna[j].d[p];

			    		int interpolacion = a_i + b_i * (t + 1) + c_i * (t + 1) * (t + 1) + d_i * (t + 1) * (t + 1) * (t + 1);

			    		if(interpolacion > 255)
			    			interpolacion = 255;
			    		if(interpolacion < 0)
			    			interpolacion = 0;

			    		expandida[i + t][j] = interpolacion;
		    		}
		    	}
		    }

		    

		 //    ///////////muestro
		   
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
		    		if(j == dato_c + k + 1 && j != expandida[i].size() -1) {dato_c = j;}
		    		if(expandida[i][j] != -1)
		    		{
		    			if(i != expandida.size() - 1)  {dato_f = i;}	
		    		} 
		    		else
		    		{
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
	        		if(j != expandida[i].size() - 1) {salida << expandida[i][j] << " ";}
	        		else {salida << expandida[i][j];}
	        	}
	        	if(i != expandida.size() -1) {salida << endl;}
	        } 
	        salida.close();
	        string matlabCommand2 = "nohup matlab -nodisplay -nosplash -nojvm -r \"ImageTxtToBmp('salida1.csv', '";
			matlabCommand2 += argv[1];
			matlabCommand2 += "Output.bmp');quit\"";

			system(matlabCommand2.c_str());
		
	        break;
		}

		case BILINEALBIS:
		{
			//Primero interpolo por filas, de a k filas
			int dato_f, dato_c; 
		    for(int i = 0; i < expandida.size(); i = i+k+1)
		    {
		    	dato_c = 0;
		    	for(int j = 0; j < expandida[i].size() ; j++)
		    	{
		    		if(expandida[i][j] != -1 && i != expandida.size() - 1) {dato_f = i; dato_c = j;}
		    		else
		    		{
		    			if(j == dato_c + k + 1 && j != expandida[i].size() -1) {dato_c = j;}
		    			expandida[i][j] = calculo_bilineal_por_filas(k, dato_c, dato_f, i, j, expandida);	
		    		}
		    	}
		    }


		    //Ahora interpolo todas las columnas
		    //traspongo a lo cabeza
		   vector<vector <int> > expandidaAux;
		 
		   for (int i = 0; i < expandida.size(); ++i)
		   {
			vector<int> fila_exp;
		   		for (int j = 0; j < expandida.size(); ++j)
		   		{
		   			fila_exp.push_back(expandida[j][i]);
		   			
		   		}
		   		expandidaAux.push_back(fila_exp);

		   }


		    for(int i = 0; i < expandidaAux.size(); i++)
		    {
		    	dato_c = 0;
		    	for(int j = 0; j < expandidaAux[i].size() ; j++)
		    	{
		    		if(expandidaAux[i][j] != -1 && i != expandidaAux.size() - 1) {dato_f = i; dato_c = j;}
		    		else
		    		{
		    			if(j == dato_c + k + 1 && j != expandidaAux[i].size() -1) {dato_c = j;}
		    			expandidaAux[i][j] = calculo_bilineal_por_filas(k, dato_c, dato_f, i, j, expandidaAux);	
		    		}
		    	}
		    }

		    //vuelvo a trasponer a lo cabeza, perdon
		    vector<vector <int> > expandidaAux2;
		 
		   for (int i = 0; i < expandidaAux.size(); ++i)
		   {
			vector<int> fila_exp2;
		   		for (int j = 0; j < expandidaAux.size(); ++j)
		   		{
		   			fila_exp2.push_back(expandidaAux[j][i]);
		   			
		   		}
		   		expandidaAux2.push_back(fila_exp2);

		   }



		    ofstream salida("salida2bis.csv",std::ofstream::out);// genero este nuevo archivo de salda con el formato pero el vector tiene valores cero
	        
	        for(int i = 0; i < expandidaAux2.size(); i++)
	        {
	        	for(int j = 0; j < expandidaAux2[i].size(); j++)
	        	{
	        		salida << expandidaAux2[i][j] << " ";
	        	}
	        	salida << endl;
	        } 

	        string matlabCommand2 = "nohup matlab -nodisplay -nosplash -nojvm -r \"ImageTxtToBmp('salida2bis.csv', '";
			matlabCommand2 += argv[1];
			matlabCommand2 += "Output.bmp');quit\"";

			system(matlabCommand2.c_str());


			break;
		}
		
		default:
            throw runtime_error("No existe ese metodo");
    }
    return  0;

}       
      
