vector<double> SistemaBandas::ForWardSubstitution()
{
	vector<double> result = vector<double>(filas);
    for (int i= 0; i < filas; i++)
    {
        result[i] = Obtener(i,filas);
        for (int k=0; k< i; k++)
        {
            result[i] = result[i] - Obtener(i,k)*result[k];
        }
        result[i] = result[i]/Obtener(i,i);

    }
    return result;
}

void SistemaBandas::LU()
{
	for(unsigned int j = 0; j < filas; j++) //por ser bandas una matriz cuadrada +1 una columna con los b's de B (Ax = B)
	{// solo deben triangularse a*b columnas
		for (unsigned int i = j+1; i < filas; i++)
		{

			double coeficiente = (Obtener(i,j)/Obtener(j,j));
			Modificar(i, j, coeficiente);
			if(Obtener(i,j) !=0) //para no hacer restas de mas
			{	
				RestarFila(coeficiente, j, i, j+1);
			}
		}
	//	CerosAIzquierda(j,j);
	}
}


vector<vector<double> > SistemaBandas::ObtenerL()
{
	vector<vector<double> > res;
	for(int i = 0; i < filas; i++)
	{
		vector<double> a;
		res.push_back(a);
		for(int j = 0; j <= i; j++)
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
		for(int j = i; j < filas; j++)
		{
			res[i].push_back(Obtener(i,j));
		}
	}
	return res;
}


