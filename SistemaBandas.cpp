#include "SistemaBandas.h"

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

void SistemaBandas::RestarFila(double coeficiente, unsigned int primera, unsigned int segunda, unsigned int desdeColumna)
{
	for(unsigned int i = desdeColumna; i <= filas; i++) //filas es la cantidad de filas de la matriz banda, la misma es de tamaño (a*b)(a*b + 1(vector B))
	{//como se indexa desde cero debe restar hasta la columna a*b +1 - 1 = filas 
		Modificar(segunda,i, Obtener(segunda,i) - (coeficiente*Obtener(primera,i)));
	}
}

void SistemaBandas::CerosAIzquierda(unsigned int fila, unsigned int hastaColumna)
{
	for(unsigned int i = 0; i < hastaColumna; i++)
		Modificar(fila,i, 0);
}

void SistemaBandas::EliminacionGaussiana()
{
	for(unsigned int j = 0; j < filas; j++) //por ser bandas una matriz cuadrada +1 una columna con los b's de B (Ax = B)
	{// solo deben triangularse a*b columnas
		for (unsigned int i = j+1; i < filas; i++)
		{
			if(Obtener(i,j) !=0) //para no hacer restas de mas
			{	
				double coeficiente = (Obtener(i,j)/Obtener(j,j));
				RestarFila(coeficiente, j, i, j+1);
			}
		}
		CerosAIzquierda(j,j);
	}
}



