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