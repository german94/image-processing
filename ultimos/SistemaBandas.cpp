#include "SistemaBandas.h"
#include "eliminarSan.h"
#include <algorithm> 



SistemaBandas::SistemaBandas(unsigned int _filas, unsigned int _ancho) : filas(_filas), columnas((2 * _ancho) + 2), ancho(_ancho)
{
	matriz = vector<vector<double > >(filas);
	for(unsigned int i = 0; i < filas; i++)
		matriz[i] = vector<double>(columnas);
}

double SistemaBandas::Obtener(unsigned int fila, unsigned int columna) const
{
	pair<int, int> indices = indexar(fila, columna);

	if(indices.first == -1)
		return 0;

	return matriz[indices.first][indices.second];
}

void SistemaBandas::Modificar(unsigned int fila, unsigned int columna, double valor)
{
	pair<int, int> indices = indexar(fila, columna);

	if(indices.first != -1)
		matriz[indices.first][indices.second] = valor;
}

void SistemaBandas::Mostrar() const
{
	for(unsigned int i = 0; i < filas; i++)
	{
		for(unsigned int j = 0; j < columnas; j++)
			cout<<matriz[i][j]<<"   ";
		cout<<"\n";
	}
}

pair<int, int> SistemaBandas::indexar(unsigned int fila, unsigned int columna) const
{
	int m = ancho;
	pair<int, int> ret(-1, -1);

	if(fila == columna)
	{
		ret.first = fila;
		ret.second = m;
	}
	else if(columna == filas)//estoy en las pos axb es decir al indexar desde cero en la columna de los B
	{
		ret.first = fila;
		ret.second = columnas - 1;
	}
	else
	{
		if(columna > fila) //esta del lado derecho a partir de la mitad de la fila
		{
			if(columna - fila <= ancho)
			{
				ret.first = fila;
				ret.second = m + columna - fila; //esta dentro del rango de la matriz
			}
		}
		else
		{
			if(fila - columna <=  ancho) //esta del lado izquierdo y entra en el rango
			{
				ret.first = fila;
				ret.second = m - (fila - columna);
			}
		}
	}
	return ret;
}

void SistemaBandas::CerosAIzquierda(unsigned int fila, unsigned int hastaColumna)
{
	for(unsigned int i = 0; i < hastaColumna; i++) //para evitar que nos quede 0,0000001 donde ya sabemos que debe ir un cero 
		Modificar(fila,i, 0);
}

void SistemaBandas::RestarFila(double coeficiente, unsigned int primera, unsigned int segunda, unsigned int desdeColumna, char metodo)
{

	int maxRestasPos = 2*ancho + 1; // filas es de tamaño a*b pero no hay que hacer todas las restas a los sumo 2 por el ancho + 1;
	for(unsigned int i = desdeColumna, j = 0 ; i < filas && j <= maxRestasPos; i++, j++) //filas es la cantidad de filas de la matriz banda, la misma es de tamaño (a*b)(a*b + 1(vector B))
	{//como se indexa desde cero debe restar hasta la columna a*b +1 - 1 = filas 
		Modificar(segunda,i, Obtener(segunda,i) - (coeficiente*Obtener(primera,i)));
	}
	if(metodo == '0' || metodo == '2' ) {Modificar(segunda, filas, Obtener(segunda,filas) - (coeficiente*Obtener(primera,filas)));} //sino solo lo estoy usando para obtener LU
}

void SistemaBandas::LU1(char metodo)
{
	for(unsigned int j = 0; j < filas; j++) //por ser bandas una matriz cuadrada +1 una columna con los b's de B (Ax = B)
	{// solo deben triangularse a*b columnas
		for (unsigned int i = j+1; (i<= j + ancho) && (i < filas); i++)
		{
			double coeficiente = (Obtener(i,j)/Obtener(j,j));
			Modificar(i, j, coeficiente);
			if(Obtener(i,j) !=0) //para no hacer restas de mas
			{	
				RestarFila(coeficiente, j, i, j+1, metodo);
			}
		}
	}
}
void SistemaBandas::EliminacionGaussiana0(char metodo)
{
	for(unsigned int j = 0; j < filas; j++) //por ser bandas una matriz cuadrada +1 una columna con los b's de B (Ax = B)
	{// solo deben triangularse a*b columnas
		for (unsigned int i = j+1; (i <= j + ancho) && (i < filas); i++) //hasta fila + ancho puede que haya algun numero != de cero. luego no. ya va a estar  
		{//triangulada esa parte de la columna
			if(Obtener(i,j) !=0) //para no hacer restas de mas
			{	
				double coeficiente = (Obtener(i,j)/Obtener(j,j));
				RestarFila(coeficiente, j, i, j+1, metodo);
			}
		}
		CerosAIzquierda(j,j);
	}
}

vector<double> SistemaBandas::BackWardSubstitution()
{
	vector<double> result = vector<double>(filas);
    for (int i = filas -1; i>=0; i--)
    {
        result[i] = Obtener(i,filas);
        for (int k=i+1; k< filas && (k <= ( i + ancho)); k++)
        {
            result[i] = result[i] - Obtener(i,k)*result[k];
        }
        result[i] = result[i]/Obtener(i,i);
    }
    return result;
}

vector<vector<double> > SistemaBandas::ObtenerL()
{

	vector<vector<double> > res;
	for(int i = 0; i < filas; i++)
	{
		vector<double> a;
		res.push_back(a);
		for(int j = max(0, (int)(i- ancho)); j <= i; j++)
		{
			if(i ==j)
			{
				res[i].push_back(1);

			}
			else
			{
				res[i].push_back(Obtener(i,j));
			}
		}
	}

	return res;
}

vector<vector<double> > SistemaBandas::ObtenerU()
{
	vector<vector<double> > res ;
	for(int i = 0; i < filas; i++)
	{
		vector<double> a;
		res.push_back(a);
		for(int j = i; j < filas && j <= (ancho + i); j++)
		{
			res[i].push_back(Obtener(i,j));
		}
	}

	return res;
}

vector<double> SistemaBandas::ObtenerTempModo1()
{
	vector<vector<double> > L = ObtenerL();
	vector<vector<double> > U = ObtenerU();
	vector<double> b;
	for(int i = 0; i < filas; i++) {b.push_back(Obtener(i, filas));}

	vector<double> y = ForWardSubstitution(b, L, ancho);
	vector<double> res = BackWardSubstitution2(y, U);

	return res;
}