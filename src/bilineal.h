#include "util.h"

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

int calculo_bilineal_por_filas(int k, int dato_c, int i, int j, vector<vector <int> > &expandida)
{
	int limite_c = expandida[0].size() - 1;
	int datoSig_c;

	if(dato_c + k + 1 <= limite_c) {datoSig_c = dato_c + k +1;}
	else {datoSig_c = limite_c;}

	int res = (int)((double)expandida[i][dato_c] + (((double)expandida[i][datoSig_c] - (double)expandida[i][dato_c])/(double)(k+1))*((double)j-(double)dato_c));

	if(res < 255) {return res;}
	else {return 255;} //no estoy seguro de esto
}

int calculo_bilineal_por_columnas(int k, int dato_f, int i, int j, vector<vector <int> > &expandida)
{
	int limite_f = expandida.size() - 1;
	int datoSig_f;

	if(dato_f + k + 1 <= limite_f) {datoSig_f = dato_f + k +1;}
	else {datoSig_f = limite_f;}

	int res = (int)((double)expandida[dato_f][j] + (((double)expandida[datoSig_f][j] - (double)expandida[dato_f][j])/(double)(k+1))*((double)i-(double)dato_f));

	if(res < 255) {return res;}
	else {return 255;} //no estoy seguro de esto
}

void originBilineal(vector<vector<int> > &expandida, int k)
{
	//Primero interpolo por filas, de a k filas
	int dato_c;
	for(int i = 0; i < expandida.size(); i = i+k+1)
	{
	  	dato_c = 0;
	  	for(int j = 0; j < expandida[i].size() ; j++)
	   	{
	   		if(expandida[i][j] != -1 && i != expandida.size() - 1) {dato_c = j;}
	   		else
	    		{
		   			if(j == dato_c + k + 1 && j != expandida[i].size() -1) {dato_c = j;}
		   			expandida[i][j] = calculo_bilineal_por_filas(k, dato_c, i, j, expandida);
		   		}
    	}
    }

	//Ahora interpolo por columnas, de a una fila
	int dato_f;
	for(int j = 0; j < expandida[0].size(); j++)
	{
	   	dato_f = 0;
	   	for(int i = 0; i < expandida.size() ; i++)
	   	{
	   		if(expandida[i][j] != -1 && j != expandida[0].size() - 1) {dato_f = i;}
	   		else
	   		{
	   			if(i == dato_f + k + 1 && i != expandida.size() -1) {dato_f = i;}
	   			expandida[i][j] = calculo_bilineal_por_columnas(k, dato_f, i, j, expandida);
	   		}
	   	}
	}
}

pair<double,double> dameBilineal(double x0, double y0, double x1, double y1){// devuelve la pendiente y ordenada al origen de una bilineal(en ese orden)

    pair<double, double> res;
    res.first =  ((double)(y1-y0))/(x1-x0);
    res.second = y0 - res.first*x0;
    return res;

}

void porDiagBilineal(vector<vector<int> > &expandida, int k){


//Primero interpolo por filas, de a k filas
				int dato_c;
				for(int i = 0; i < expandida.size(); i = i+k+1)
				{
					dato_c = 0;
					for(int j = 0; j < expandida[i].size() ; j++)
					{
						if(expandida[i][j] != -1 && i != expandida.size() - 1) {dato_c = j;}
						else
							{
								if(j == dato_c + k + 1 && j != expandida[i].size() -1) {dato_c = j;}
								expandida[i][j] = calculo_bilineal_por_filas(k, dato_c, i, j, expandida);
							}
					}
				}

                    //Ahora interpolo por columnas, de a una fila solo en los valores originales
                    int dato_f;
                    for(int j = 0; j < expandida[0].size(); j+=k+1)
                    {
                        dato_f = 0;
                        for(int i = 0; i < expandida.size() ; i++)
                        {
                            if(expandida[i][j] != -1 && j != expandida[0].size() - 1) {dato_f = i;}
                            else
                            {
                                if(i == dato_f + k + 1 && i != expandida.size() -1) {dato_f = i;}
                                expandida[i][j] = calculo_bilineal_por_columnas(k, dato_f, i, j, expandida);
                            }
                        }
                    }

                    //ahora interpolo cada cuadradito


            //ic, jc -> filas y columnas del cuadradito
		    for(int ic = 0; ic < expandida.size() -1; ic +=k+1){

		    	for(int jc = 0; jc < expandida[jc].size() -1 ; jc+=k+1){//para cada cuadradito

                    int id=ic;
                    int jd=jc+k+1;
                    for(int cont=0;cont<=k-1;cont++){ //completamos las diagonales inferiores incluyendo la principal

                        pair<double, double> pendiente_ordenada=dameBilineal(0,expandida[id][jc],k+1-cont,expandida[ic+k+1][jd]);

                        for(int cont_diagonal=1;cont_diagonal<=k-cont;cont_diagonal++){ // calculamos e insertamos los valores de una diagonal

                            expandida[id+cont_diagonal][jc+cont_diagonal]= pendiente_ordenada.first* cont_diagonal + pendiente_ordenada.second;

                        }


                        id++;
                        jd--;

                    }


                    //ahora completamos las diagonales superiores, sin incluir la principal

                    jd=jc+1;
                    id=ic+k;
                    for(int cont=0;cont<=k-2;cont++){

                        pair<double, double> pendiente_ordenada=dameBilineal(0,expandida[ic][jd],k-cont,expandida[id][jc+k+1]);

                        for(int cont_diagonal=1;cont_diagonal<=k-1-cont;cont_diagonal++){ // calculamos e insertamos los valores de una diagonal

                            expandida[ic+cont_diagonal][jd+cont_diagonal]= pendiente_ordenada.first* cont_diagonal + pendiente_ordenada.second;

                        }


                        jd++;
                        id--;

                    }
                }
            }

}





void expandidoBilineal(vector<vector<int> > &expandida, int k)
{
	int cantDePixelesAIgnorar = 0; //Esto podria ser un paramentro, pero no se hasta donde tendria sentido
	//Primero interpolo por filas, de a k filas
	int dato_c;
	int contador = 0;
	for(int i = 0; i < expandida.size(); i = i+k+1)
	{
	  	dato_c = 0;
	  	for(int j = 0; j < expandida[i].size() ; j++)
	   	{
	   		if(expandida[i][j] != -1 &&  i != expandida.size() - 1)
	   		{
	   			if(contador == cantDePixelesAIgnorar)
	   			{
	   				dato_c = j;
	   				contador = 0;
	   			}
	   			else
	   			{
	   				contador++;
	   			}
	   		}
	   		else
	    	{
		   		if(j == dato_c + k + 1 && j != expandida[i].size() -1) {dato_c = j;}
		   		expandida[i][j] = calculo_bilineal_por_filas(k, dato_c, i, j, expandida);
		   	}
    	}
    }

	//Ahora interpolo por columnas, de a una fila
	int dato_f;
	contador = 0;
	for(int j = 0; j < expandida[0].size(); j++)
	{
	   	dato_f = 0;
	   	for(int i = 0; i < expandida.size() ; i++)
	   	{
	   		if(expandida[i][j] != -1 && j != expandida[0].size() - 1)
	   		{
	   			if(contador == cantDePixelesAIgnorar)
	   			{
	   				dato_f = i;
	   				contador = 0;
	   			}
	   			else
	   			{
	   				contador++;
	   			}
	   		}
	   		else
	   		{
	   			if(i == dato_f + k + 1 && i != expandida.size() -1) {dato_f = i;}
	   			expandida[i][j] = calculo_bilineal_por_columnas(k, dato_f, i, j, expandida);
	   		}
	   	}
	}
}
