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