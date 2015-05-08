#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Matriz.h"
#include <math.h>
#include <cmath>

using namespace std;

double norma2(Matriz<double>& v)
{
    double n = 0;
    for(int i = 0; i < v.filas(); i++)
   		n = n+ v[i][0] * v[i][0];

    return sqrt(n);
}

double normaInf(Matriz<double>& v)
{
    double res = abs(v[0][0]);
    for(int i = 0; i < v.filas(); i++)
    {
   		if(res < abs(v[i][0])) {res = abs(v[i][0]);}
    }

    return res;
}


void copiarMatriz(Matriz<double>& m1, Matriz<double>& m2)
{
	for(int i = 0; i < m2.filas(); i++)
	{
		for(int j = 0; j < m2.columnas(); j++)
			m1[i][j] = m2[i][j];
	}
}

bool haypocadif(Matriz<double>&v0, Matriz<double>&v1)
{
    for(int i = 0; i < v0.filas(); i++)
    {
        if(abs(v0[i][0] - v1[i][0]) > 0.0000009) {return false;}
    }
    return true;
}

double ObtenerAutovalorMayor(Matriz<double> &A, Matriz<double>& v0){

    double lambda= normaInf(v0);
    double normalizar= norma2(v0);

    for(int i = 0; i < v0.filas(); i++){ v0[i][0] = v0[i][0]/normalizar;}

    Matriz<double> v1(v0.filas(),1);

    for(; ;)
    {

        v1= A* v0;

        lambda= normaInf(v1)/normaInf(v0);

        normalizar = norma2(v1);

        for(int j = 0; j < v1.filas(); j++){ v1[j][0] = v1[j][0]/normalizar; }
        if(haypocadif(v0, v1)){break;}
        v0=v1;
    }

    return lambda;
}

vector<double> metodoPotencias(Matriz<double>& A, unsigned int alpha, Matriz<double>& P, Matriz<double>& v){

    vector<double> autovalores;

    for(int i=0;i<alpha;i++)
    {
        double autovalori= ObtenerAutovalorMayor(A,v);

        autovalores.push_back(autovalori);

        //guardamos en P los autovectores como columnas
        for(int j=0;j<A.columnas();j++){ P[j][i]=v[j][0]; }

        Matriz<double> vt(1, v.filas());

        vt= v.traspuesta();

        Matriz<double> prod(v.filas(),v.filas());
        prod= v*vt*autovalori;

        A =A - prod;
    }

    return autovalores;
}
/*
Matriz<double> multiplicar(Matriz<double>& m1, Matriz<double>& m2 ){

Matriz<double> res(m1.filas(),m2.columnas());

    for(int i=0;i<m1.filas();i++){

        for(int j=0;j<m2.columnas();j++){
            int sum=0;
            for(int k=0;k<m1.columnas();k++){
                sum+=m1[i][k]*m2[k][j];

            }
            res[i][j]= sum;
        }
    }
}
*/
