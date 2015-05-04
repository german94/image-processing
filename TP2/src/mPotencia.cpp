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

/*
pair<Matriz<double>, double> metodoPotencia(Matriz<double> A, Matriz<double>& x, unsigned int k)
{
	Matriz<double> A_kx(A.filas(), 1);
	A_kx = A * x;
	//cout<<A_kx[0][0]<<endl;
	A_kx = A_kx * (1.0f / norma2(A_kx));

	Matriz<double> A_kmenos1x(A.filas(), 1);

	for(int i = 1; i < k + 1; i++)
	{
		if(i == k)
			copiarMatriz(A_kmenos1x, A_kx);

		A_kx = A * A_kx;
		A_kx = A_kx * (1.0f / norma2(A_kx));
	}

	// cout<<"___"<<endl;
	// cout<<A_kmenos1x[0][0]<<" - "<<A_kx[0][0]<<endl;
	// cout<<A_kmenos1x[1][0]<<" - "<<A_kx[1][0]<<endl;
	// cout<<A_kmenos1x[2][0]<<" - "<<A_kx[2][0]<<endl;

	double phiA_kmenos1x = 0, phiA_kx = 0;
	for(int i = 0; i < A.filas(); i++)
	{
		if(phiA_kx != 0 && phiA_kmenos1x != 0)
			break;
		else
		{
			if(phiA_kx == 0 && A_kx[i][0] != 0)
				phiA_kx = A_kx[i][0];

			if(phiA_kmenos1x == 0 && A_kmenos1x[i][0] != 0)
				phiA_kmenos1x = A_kmenos1x[i][0];
		}
	}

	//cout<<phiA_kx/phiA_kmenos1x<<endl;
	//cout<<"___"<<endl;

	return make_pair(A_kx, phiA_kx/phiA_kmenos1x);
}

vector<pair<Matriz<double>, double> > dameAvecYAval(Matriz<double>& A, Matriz<double>& x, unsigned int k)
{
	vector<pair<Matriz<double>, double> > ret;

	Matriz<double> m(A.filas(), A.columnas());
	copiarMatriz(m, A);
	for(int i = 0; i < A.filas(); i++)
	{
		ret.push_back(metodoPotencia(m, x, k));
		Matriz<double> retiT(1, ret[i].first.filas());
		retiT = ret[i].first.traspuesta();
		Matriz<double> retiXretiT(ret[i].first.filas(), ret[i].first.filas());
		retiXretiT = ret[i].first * retiT;
		retiXretiT = retiXretiT * ret[i].second;
		m = m - retiXretiT;
		// cout<<m[0][0]<<" "<<m[0][1]<<" "<<m[0][2]<<" )"<<endl;
		// cout<<m[1][0]<<" "<<m[1][1]<<" "<<m[1][2]<<" )"<<endl;
		// cout<<m[2][0]<<" "<<m[2][1]<<" "<<m[2][2]<<" )"<<endl;
	}

	return ret;
}
*/
//devuelve un vector con alpha autovalores
/*
vector<double> obtenerAutovalores(Matriz<double> A, Matriz<double>& v0, unsigned int k){




}

*/
double ObtenerAutovalor(Matriz<double> &A, Matriz<double>& v0, unsigned int k){

    double lambda=v0[0][0];

    double normalizar= 1.0/ norma2(v0);

    v0= v0* normalizar;

    Matriz<double> v1(v0.columnas(),1);

    for(int i=0;i<k;i++){

        v1= A* v0;

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




int main()
{
	Matriz<double> m(3, 3);

	m[0][0] = 1; m[0][1] = 2; m[0][2] = 7;
	m[1][0] = 2; m[1][1] = 4; m[1][2] = 0;
	m[2][0] = 7; m[2][1] = 0; m[2][2] = 3;

	Matriz<double> v(3, 1);
	v[0][0] = 1;
	v[1][0] = 5;
	v[2][0] = 6;

	/*double autovalor1= ObtenerAutovalor(m,v,150);

	cout<<"v[0]= "<<v[0][0]<<endl;

	cout<<"v[1]= "<<v[1][0]<<endl;

	cout<<"v[2]= "<<v[2][0]<<endl;

	cout<<"autovalor1 "<<autovalor1<<endl;

    Matriz<double> vtras1(1, v.filas());

	vtras1= v.traspuesta();

	Matriz<double> prod(3,3);
	prod= v*vtras1*autovalor1;

	m =m - prod;
	*/

	double autovalor2= ObtenerAutovalor(m,v,150);

	cout<<"v[0]= "<<v[0][0]<<endl;

	cout<<"v[1]= "<<v[1][0]<<endl;

	cout<<"v[2]= "<<v[2][0]<<endl;

	cout<<"autovalor2 "<<autovalor2<<endl;
	


	cout<<"metodopotencias\n";

	vector<double> autoV;

	Matriz<double> P(m.columnas(),3);

    autoV= metodoPotencias(m, 3, 20, P, v);
    cout<<"[";
    for(int i=0; i<autoV.size();i++){

    cout<<autoV[i]<<", ";

    }
    cout<<"]\n";

    P.display();

    return 0;

}

/*
	vector<pair<Matriz<double>, double> > res = dameAvecYAval(m, v, 150);

	cout<<res[0].second<<" - ("<<res[0].first[0][0]<<" "<<res[0].first[1][0]<<" "<<res[0].first[2][0]<<" )"<<endl;
	cout<<res[1].second<<" - ("<<res[1].first[0][0]<<" "<<res[1].first[1][0]<<" "<<res[1].first[2][0]<<" )"<<endl;
	cout<<res[2].second<<" - ("<<res[2].first[0][0]<<" "<<res[2].first[1][0]<<" "<<res[2].first[2][0]<<" )"<<endl;


*/
