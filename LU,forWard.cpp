/*	int mejor = eliminarSanguijuela(unSistema, sanguijuelas);
	if(mejor == -1)
	{
		cout << "no es posible salvar el parabrisas" << endl;
	}
	else
	{
		cout << "es posible salvar el parabrisas eliminando la sanguijuela n° " << mejor << endl; 
	}

*/ //para el main


/*
	int eliminarSanguijuela(SistemaBandas& unSistema, vector<vector<double> > sanguijuelas, unsigned int matrixSize, unsigned int discrWidth, unsigned int discrHeight)
	{	//AQUELLAS SANGUIJUELAS QUE NO ACTUAN EN NINGUN PUNTO LAS DENOTO COMO -1, LAS QUE ACTUAN EN MAS DE 1 PUNTO COMO -2, Y LAS QUE SOLO EN UN PUNTO, LAS DENOTO CON LAS POS SOBRE LA QUE ACTUA QUE ES >=0

		int cual = -1;
		int mejorTemp;
		double nuevatemp;
		for(int i = 0; i < sanguijuelas.size(); i++)
		{
			if (sanguijuelas[i][4] >= 0)
			{
				nuevatemp = eliminar1sang(unSistema, sanguijuelas[i][4]);
				if (nuevatemp < 235 && (nuevatemp < mejorTemp)) {mejorTemp == nuevatemp; cual = i;}

			}
			else 
			{
				if (sanguijuelas[i][4] == -2)
				{
					vector<vector<double> > nuevasSang = filtrar(sanguijuelas, i);
					SistemaBandas nuevo_sistema(matrixSize, discrWidth - 1);
					vector<double> solucion = obtenerTemperatura(nuevo_sistema, sanguijuelas, matrixSize, discWidth);
					nuevatemp = dameTempPtoCritico(solucion, discrHeight-1, discWidth-1);
					if (nuevatemp < 235 && (nuevatemp < mejorTemp)) {mejorTemp == nuevatemp; cual = i;}
				}
			}
		}
		return cual;
	}
*/





/*

vector<double> SistemaBandas::BackWardSubstitution()
{
	vector<double> result = vector<double>(filas);
    for (int i = filas -1; i>=0; i--)
    {
        result[i] = Obtener(i,filas);
        for (int k=i+1; k< filas ; k++)
        {
            result[i] = result[i] - Obtener(i,k)*result[k];
        }
        result[i] = result[i]/Obtener(i,i);
    }
    return result;
}


////////////////////////////////////////////////////



eliminar1sang(SistemaBandas& unSistema, int f_v)
{
	vector<vector<double> > L = unSistema.ObtenerL();
	vector<vector<double> > U = unSistema.ObtenerU();



}




void Sherman–Morrison(vector<vector<double> > &L, vector<vector<double> > &U, vector<double> &vt, vector<double> &u, int f_v)
{//pese a que u solo es un vector con un solo 1 por como esta implementado forward necesita de un vector, a menos que cree otra variante de forward
	//obtner A(-1)*nuevoB ES DECIR A*x = nuevoB , LU*x= nuevoB -> paso 1) Ly = nuevoB y paso 2) U*x = y 

	vector<double> y = ForWardSubstitution(nuevoB, L); //paso 1

	vector<double> A-1PorNuevoB = BackWardSubstitution2(y, U); // paso 2

	//obtner A(-1)*u ES DECIR A*x = u , LU*x= u -> paso 1) Lz = u y paso 2) U*x = z 

	vector<double> z = ForWardSubstitution(u, L); //paso 1

	vector<double> A-1Poru = BackWardSubstitution2(z, U); // paso 2

	//resolver A(-1)*u*vt*A(-1).nuevoB; vt es de largo ancho*largo pero a lo sumo tiene tres -1s y un 3. no vale la pena un vector del tal tamaño
	// k = vt*A(-1)
	double k = A-1PorNuevoB[f_v - (discrWidth -1)]*vt[0] + A-1PorNuevoB[f_v - 1]*vt[1] + A-1PorNuevoB[f_v]*vt[2] + A-1PorNuevoB[f_v +1 ]*vt[3] + A-1PorNuevoB[f_v + discrWidth -1]*vt[4]

	// resolver l = + vt*A(-1)poru; l = vt*A(-1)poru 

	double l = 1 + 	A-1Poru[f_v - (discrWidth -1)]*vt[0] + A-1Poru[f_v - 1]*vt[1] + A-1Poru[f_v]*vt[2] + A-1Poru[f_v +1 ]*vt[3] + A-1Poru[f_v + discrWidth -1]*vt[4]

	// resolver ahora (A-1poru*k)/l ; segundo termindo de Morrinson
	A-1poru = k/l*A-1Poru;
	//resta final
	A-1PorNuevoB = A-1PorNuevoB - A-1Poru;
}

vector<double> ForWardSubstitution( vector<double> b; vector<vector<double> L)
{
	vector<double> result = vector<double>(L.size());
    for (int i= 0; i < filas; i++)
    {
        result[i] = b(i);
        for (int k=0; k< i; k++)
        {
            result[i] = result[i] - L[i][k]*result[k];
        }
        result[i] = result[i]/L[i][i];

    }
    return result;
}

vector<double> BackWardSubstitution2(vector<double> y; vector<vector> U)
{
	vector<double> result = vector<double>(U.size());
    for (int i = U.size() -1; i>=0; i--)
    {
        result[i] = y(i);
        for (int k=i+1; k< U.size() ; k++)
        {
            result[i] = result[i] - U[i][k]*result[k];
        }
        result[i] = result[i]/U[i][i];
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
				RestarFila0(coeficiente, j, i, j+1);
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
*/

