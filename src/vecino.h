#include "util.h"

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

void origin(vector<vector<int> > &expandida, int k)
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
}

void porCol(vector<vector<int> > &expandida)
{
	for(int j = 0; j <  expandida[0].size(); j++)
    {
    	for(int i = 0; i < expandida.size() ; i++)
	    {
	    	if(expandida[i][j] == -1)
		   	{
		    	if(i == 0) {expandida[i][j] = expandida[i][j-1];}
		    	else {expandida[i][j] = expandida[i-1][j];}
		    }	
		}
	}
}	

void porFil(vector<vector<int> > &expandida)
{		
    for(int i = 0; i <  expandida.size(); i++)
    {
    	for(int j = 0; j < expandida[i].size() ; j++)
    	{
    		if(expandida[i][j] == -1)
    		 {
    		 	if(j == 0) {expandida[i][j] = expandida[i-1][j];}
    		 	else {expandida[i][j] = expandida[i][j-1];}
    		 }
    		
    	}
    }
}

void prom(vector<vector<int> > &expandida)
{
    for(int i = 0; i <  expandida.size(); i++)
    {
    	for(int j = 0; j < expandida[i].size() ; j++)
    	{
    		if(expandida[i][j] == -1)
    		 {
    		 	int cantidad = 0;
    		 	int valor = 0;
    		 	if(j!=0 && j!= expandida[i].size() -1 && i!= 0 && i != expandida.size() -1)
    		 	{
    		 		if(expandida[i-1][j] != -1) {cantidad++; valor+= expandida[i-1][j];}
    		 		if(expandida[i+1][j] != -1) {cantidad++; valor+= expandida[i+1][j];}
    		 		if(expandida[i][j-1] != -1)	{cantidad++; valor+= expandida[i][j-1];}
    		 		if(expandida[i][j+1] != -1)	{cantidad++; valor+= expandida[i][j+1];}
    		 	}
    		 	if(j == 0) 
    		 	{
    		 		if(expandida[i-1][j] != -1) {cantidad++; valor+= expandida[i-1][j];}
    		 		if(expandida[i+1][j] != -1) {cantidad++; valor+= expandida[i+1][j];}
    		 		if(expandida[i][j+1] != -1)	{cantidad++; valor+= expandida[i][j+1];}
    		 	}
    		 	if(j== expandida[i].size() -1)
    		 	{	
    		 		if(expandida[i-1][j] != -1) {cantidad++; valor+= expandida[i-1][j];}
    		 		if(expandida[i+1][j] != -1) {cantidad++; valor+= expandida[i+1][j];}
    		 		if(expandida[i][j-1] != -1)	{cantidad++; valor+= expandida[i][j-1];}
    		 	}
    		 	if(i == 0) 
    		 	{
    		 		if(expandida[i][j-1] != -1) {cantidad++; valor+= expandida[i][j-1];}
    		 		if(expandida[i+1][j] != -1) {cantidad++; valor+= expandida[i+1][j];}
    		 		if(expandida[i][j+1] != -1)	{cantidad++; valor+= expandida[i][j+1];}
    		 	}
    		 	if(i== expandida.size() -1)
    		 	{
    		 		if(expandida[i][j-1] != -1) {cantidad++; valor+= expandida[i][j-1];}
    		 		if(expandida[i-1][j] != -1) {cantidad++; valor+= expandida[i-1][j];}
    		 		if(expandida[i][j+1] != -1)	{cantidad++; valor+= expandida[i][j+1];}	
    		 	}	
				expandida[i][j] = double(valor/cantidad);
    		 }
    		
    	}
    }
}    