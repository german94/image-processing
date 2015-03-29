#include "SistemaBandas.h"
#include <sstream>

int main()
{
	int f, a;
	cout<<"Filas: ";
	cin>>f;
	cout<<"Ancho: ";
	cin>>a;
	SistemaBandas sistema(f, a);

	cin.ignore();

	for(int i = 0; i < f; i++)
	{
		string input;
		getline(cin, input);
		std::istringstream iss(input);
		int temp;
		int k = 0;
		while(iss >> temp && k < f + 1)
		{
			sistema.Modificar(i, k, temp);
			k++;
		}
	}

	sistema.Mostrar();
	
	sistema.BackWardSubstitution();

	for(int i = 0; i < f; i++)
	{
		for(int j = 0; j < f + 1; j++)
			cout<<"("<<i<<", "<<j<<") = "<<sistema.Obtener(i, j)<<endl;
		cout<<"________________"<<endl;
	}

	return 0;
}
