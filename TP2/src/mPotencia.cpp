#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Matriz.h"
#include <math.h>

using namespace std;

double norma2(Matriz<double>& v)
{
    double n = 0;
    for(int i = 0; i < v.filas(); i++)
   		n += v[i][0] * v[i][0];

    return sqrt(n);
}

void copiarMatriz(Matriz<double>& m1, Matriz<double>& m2)
{
	for(int i = 0; i < m2.filas(); i++)
	{
		for(int j = 0; j < m2.columnas(); j++)
			m1[i][j] = m2[i][j];
	}
}

double ObtenerAutovalor(Matriz<double> A, Matriz<double>& v0, unsigned int k){

    double lambda=v0[0][0];

    double normalizar= 1.0/ norma2(v0);

    v0= v0* normalizar;

    Matriz<double> v1(v0.filas(),1);

    for(int i=0;i<k;i++){

        v1= A* v0;

/*

        int sum0=0;
        int sum1=0;
        for(int j=0; j<v1.filas();j++){
            sum0+=v0[j][0];
            sum1+=v1[j][0];

        }

           if(sum0 == 0){
            //throw runtime_error("Se esperaban 4 valores en línea ");
            cerr<<"se dividio por cero\n";
            return -1;
        }
        lambda= sum1/sum0;

        */

        if(v0[0][0] == 0){
            //throw runtime_error("Se esperaban 4 valores en línea ");
            cerr<<"se dividio por cero\n";
            return -1;
        }

         lambda= v1[0][0]/v0[0][0];

      //  cout<<"lamda "<<lambda<<endl;

        normalizar= 1.0/ norma2(v1);

        v1= v1* normalizar;

        v0=v1;
    }

    return lambda;

}

// k es la cantidad de iteraciones para aproximar
vector<double> metodoPotencias(Matriz<double>& A, unsigned int alpha, unsigned int k, Matriz<double>& P,Matriz<double>& v){


    vector<double> autovalores;

    for(int i=0;i<alpha;i++){


        double autovalori= ObtenerAutovalor(A,v,k);

        autovalores.push_back(autovalori);

        //guardamos en P los autovectores como columnas
        for(int j=0;j<A.columnas();j++){

            P[j][i]=v[j][0];

        }

        Matriz<double> vt(1, v.filas());

        vt= v.traspuesta();

        Matriz<double> prod(v.filas(),v.filas());
        prod= v*vt*autovalori;

        A =A - prod;


    }

    return autovalores;
}

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

