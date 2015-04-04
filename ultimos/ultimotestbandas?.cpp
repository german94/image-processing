#include "SistemaBandas.h"
#include "fileHandler.h"
#include "eliminarSan.h"
#include <sstream>
#include <math.h>
#include "stdio.h"
#include "stdlib.h"
#include <iomanip>
#include "time.h"


	double tiempo= 0;



int main(int argc, char** argv)
{
	FileHandler myFile(argv[1]);

	ofstream outputFile;
	outputFile.open(argv[2], ios::app);
	vector<double> params;
	params.resize(4);
	myFile.readParameters(params);


	double width = params[0];
	double heigth = params[1];
	double discrInterval = params[2];
	unsigned int nLeeches = params[3];

	unsigned int discrHeight = (unsigned int)heigth/discrInterval; //alto
	unsigned int discrWidth = (unsigned int)width/discrInterval; 	//ancho
	unsigned int matrixSize = (discrHeight - 1)*(discrWidth - 1);		//no hay que contar los bordes, si ponemos discrHeight*discWidth estamos contando un borde de cada dimension

	SistemaBandas sistema(matrixSize, discrWidth - 1);

	vector<vector<double> > sanguijuelasInfo = myFile.readLeeches(nLeeches);

	char metodo = argv[3][0];

	if(metodo == '0' || metodo == '1')
	{
		vector<double> res = obtenerTemperaturas(sistema, matrixSize, discrWidth, discrHeight, discrInterval, sanguijuelasInfo, metodo);

		outputFile << std::fixed << std::setprecision(5);

		for(int i = 0; i <= discrHeight; i++) //fila 
		{
			for(int j = 0; j <= discrWidth; j++) //columna
			{
				if(i == 0 || j==0 || i == discrHeight || j == discrWidth)
				{
					outputFile << i << "\t" << j << "\t" <<fixed<< (double)-100 << endl; 
				}
				else
				{
					outputFile << i << "\t" << j << "\t" <<fixed<< (double)res[(i - 1)*(discrWidth - 1) + j-1] << endl; 
				}
			}
		}
	}
	else
	{	
		int mejor;
	
		cargarValoresSinBordes(sistema, matrixSize, discrWidth - 1, discrHeight - 1);		//primero armo el sistema como si no tuviera ninguna sanguijuela

		cargarSanguijuelas(sistema, matrixSize, discrHeight, discrWidth, discrInterval, sanguijuelasInfo);		//despues piso las filas necesarias con los datos cuando cargo las sanguijuelas

		if (metodo == '2' ){ mejor = eliminarSanguijuelaModo2(sistema, metodo, sanguijuelasInfo, matrixSize, discrWidth, discrHeight, discrInterval);}

		if(metodo == '3') { mejor = eliminarSanguijuelaModo3(sistema, metodo, sanguijuelasInfo, matrixSize, discrWidth, discrHeight, discrInterval);}

		if(mejor == -1)
			{
				cout << "no es posible salvar el parabrisas" << endl;
			}
			else
			{
				cout << "es posible salvar el parabrisas eliminando la sanguijuela n° " << mejor << endl; 
			}
	}	
	outputFile.close();

	return 0;
}

vector<double> obtenerTemperaturas(SistemaBandas& sistema, unsigned int matrixSize, unsigned int discrWidth, unsigned int discrHeight, double discrInterval, vector<vector<double> > &sanguijuelasInfo, char metodo)
{
	cargarValoresSinBordes(sistema, matrixSize, discrWidth - 1, discrHeight - 1);		//primero armo el sistema como si no tuviera ninguna sanguijuela

	cargarSanguijuelas(sistema, matrixSize, discrHeight, discrWidth, discrInterval, sanguijuelasInfo);		//despues piso las filas necesarias con los datos cuando cargo las sanguijuelas

	vector<double> res;
	if(metodo == '0'){
		ofstream outputFileTime;
		outputFileTime.open("MedicionesMetodo0.txt", ios::app);
		init_time();
		sistema.EliminacionGaussiana0(metodo);
		res = sistema.BackWardSubstitution();
		tiempo = get_time();
		outputFileTime<<tiempo<<endl;
		outputFileTime.close();
	}
	if(metodo == '1'){sistema.LU1(metodo); res = sistema.ObtenerTempModo1();}
	return res;
}


void cargarValoresSinBordes(SistemaBandas& unSistema, unsigned int alturaSistema, unsigned int anchoDiscrSinBordes, unsigned int altoDiscrSinBordes)
{
	for(unsigned int fila = 0; fila < alturaSistema; fila++)
	{
		double resultado = 0;

		unSistema.Modificar(fila, fila, 4);

		pair<unsigned int, unsigned int> abajo(fila, fila + anchoDiscrSinBordes);
		pair<unsigned int, int> arriba(fila, fila - anchoDiscrSinBordes);
		pair<unsigned int, unsigned int> derecha(fila, fila + 1);
		pair<unsigned int, int> izquierda(fila, fila - 1);

		if(abajo.second >= alturaSistema) //>= porque se indexa desde cero
			resultado = -100;
		else
			unSistema.Modificar(abajo.first, abajo.second, -1);

		if(arriba.second < 0)
			resultado -= 100;
		else
			unSistema.Modificar(arriba.first, arriba.second, -1);

		if((fila + 1) % anchoDiscrSinBordes == 0)
			resultado -= 100;
		else
			unSistema.Modificar(derecha.first, derecha.second, -1);

		if(fila % anchoDiscrSinBordes == 0)
			resultado -= 100;
		else
			unSistema.Modificar(izquierda.first, izquierda.second, -1);

		
		unSistema.Modificar(fila, alturaSistema, resultado);

	}
}

void cargarSanguijuelas(SistemaBandas& unSistema, unsigned int alturaSistema, unsigned int discrHeight, unsigned int discrWidth, double discrInterval, vector<vector<double> >& sanguijuelasInfo){
	for(unsigned int i = 0; i < sanguijuelasInfo.size(); i++){

		double sanguijuelaX = sanguijuelasInfo[i][0];
		double sanguijuelaY = sanguijuelasInfo[i][1];

		double sanguijuelaR = sanguijuelasInfo[i][2];
		double sanguijuelaT = sanguijuelasInfo[i][3];

		vector<pair<unsigned int, unsigned int> > punSang; //LOS PUNTOS SOBRE LOS CUALES ACTUA LA SANG
							
		puntosSanguijuela(discrHeight, discrWidth, discrInterval, sanguijuelaX, sanguijuelaY, sanguijuelaR, punSang);

		if(punSang.size() == 0)
			sanguijuelasInfo[i][4] = -1;

		unsigned int camb_col, camb_fil; //VALORES DENTRO DE LA MATRIZ A MODIFICAR
		for(unsigned int j = 0; j < punSang.size(); j++)
		{ //punSANG TIENE TODOS LOS PUNTOS SOBRE LOS QUE ACTUA
			
			camb_col = punSang[j].first;
			camb_fil = punSang[j].second;
			unsigned int filaEnMatriz = (discrWidth-1)*(camb_fil -1) + camb_col -1; //ya me lo da indexado en cero

			if(punSang.size() == 1)
				sanguijuelasInfo[i][4] = filaEnMatriz;
			else
				sanguijuelasInfo[i][4] = -2;

			if(unSistema.Obtener(filaEnMatriz, alturaSistema) <= sanguijuelaT) //quizas ya hay otra sang actuando en ese punto entonces dejo la mayor
			{
				unSistema.Modificar(filaEnMatriz, filaEnMatriz, 1);
				unSistema.Modificar(filaEnMatriz, filaEnMatriz -1, 0); //IZQ
				unSistema.Modificar(filaEnMatriz, filaEnMatriz +1, 0); //DER
				unSistema.Modificar(filaEnMatriz, filaEnMatriz + (discrWidth-1), 0); //ARRIBA 
				unSistema.Modificar(filaEnMatriz, filaEnMatriz - (discrWidth-1), 0); //ABAJO
				unSistema.Modificar(filaEnMatriz, alturaSistema, sanguijuelaT);
			}
		}
	}
}


void puntosSanguijuela(unsigned int filas, unsigned int columnas, double discr, double sangX, double sangY, double radio, vector<pair<unsigned int, unsigned int> >& resultado)
{
	for(int f_s = 1; f_s < filas; f_s++)
	{
		for(int c_s = 1; c_s < columnas; c_s++)
		{
			pair<unsigned int, unsigned int> punNuevo (c_s, f_s);
			if(enCirculo(c_s, f_s, discr, sangX, sangY, radio)) {resultado.push_back(punNuevo);}
		}	
	}
}

bool enCirculo(unsigned int posX , unsigned int posY, double discr, double circlX, double circlY, double radio){
	return ( pow((posX*discr - circlX),2) + pow((posY*discr - circlY),2) <= pow(radio,2) );
}

double dameTempPtoCritico(vector<double> &solucion, unsigned int alto, unsigned int ancho){
	//ancho y alto son a/h -1 y b/h - 1
	double res = 0;

	if(ancho % 2 == 0 && alto % 2 == 0){

		pair<unsigned int, unsigned int> x1(alto/2, ancho/2);
		pair<unsigned int, unsigned int> x2(alto/2, (ancho/2)-1);
		pair<unsigned int, unsigned int> x3((alto/2)-1, ancho/2);
		pair<unsigned int, unsigned int> x4((alto/2)-1, (ancho/2)-1); 

		double temp1 = solucion[ancho*x1.first + x1.second];
		double temp2 = solucion[ancho*x2.first + x2.second];
		double temp3 = solucion[ancho*x3.first + x3.second];
		double temp4 = solucion[ancho*x4.first + x4.second];

		res = (temp1 + temp2 + temp3 + temp4) / 4;
	}
	else if(ancho % 2 == 1 && alto % 2 == 1)
	{
		pair<unsigned int, unsigned int> xp(floor(alto/2), floor(ancho/2));

		res = solucion[ancho*xp.first + xp.second];
	}
	else if(ancho % 2 == 1 && alto % 2 == 0)
	{
		pair<unsigned int, unsigned int> x1(alto/2 - 1, floor(ancho/2));
		pair<unsigned int, unsigned int> x2(alto/2, floor(ancho / 2));

		double temp1 = solucion[ancho*x1.first + x1.second];
		double temp2 = solucion[ancho*x2.first + x2.second];

		res = (temp1 + temp2) / 2;
	}
	else
	{
		pair<unsigned int, unsigned int> x1(floor(alto/2), ancho/2 - 1);
		pair<unsigned int, unsigned int> x2(floor(alto/2), ancho / 2);

		double temp1 = solucion[ancho*x1.first + x1.second];
		double temp2 = solucion[ancho*x2.first + x2.second];

		res = (temp1 + temp2) / 2;
	}

	return res;
}

vector<vector<double> > filtrar(vector<vector<double> > &sanguijuelasInfo, int i)
{
	vector<vector<double> >	res;
	for(int j = 0; j < sanguijuelasInfo.size(); j++)
	{
		if(i!=j)
		{	
			vector<double> nuevo;
			for(int i = 0; i < 5; i++)
			{
				nuevo.push_back(sanguijuelasInfo[j][i]);
			}
			res.push_back(nuevo);
		}
	}
	return res;
}

int eliminarSanguijuelaModo2(SistemaBandas& unSistema, char metodo, vector<vector<double> > &sanguijuelasInfo, unsigned int matrixSize, unsigned int discrWidth, unsigned int discrHeight, double discrInterval)
{	//AQUELLAS sanguijuelasInfo QUE NO ACTUAN EN NINGUN PUNTO LAS DENOTO COMO -1, LAS QUE ACTUAN EN MAS DE 1 PUNTO COMO -2, Y LAS QUE SOLO EN UN PUNTO, LAS DENOTO CON LAS POS SOBRE LA QUE ACTUA QUE ES >=0
	int cual = -1;
	double mejorTemp, nuevatemp;
	vector<double> solucion;
	vector<double> temp;
	for(int i = 0; i < sanguijuelasInfo.size(); i++)
	{
		if (sanguijuelasInfo[i][4] != -1)
		{
			vector<vector<double> > nuevasSang = filtrar(sanguijuelasInfo, i);

			SistemaBandas nuevo_sistema(matrixSize, discrWidth - 1);

			solucion = obtenerTemperaturas(nuevo_sistema, matrixSize, discrWidth, discrHeight, discrInterval, nuevasSang, '0');

			nuevatemp = dameTempPtoCritico(solucion, discrHeight-1, discrWidth-1);
			temp.push_back(nuevatemp);

			if (nuevatemp < 235 && (cual == -1 || nuevatemp < mejorTemp)) {mejorTemp = nuevatemp; cual = i;}
		}
	}

	for(int i = 0; i < temp.size(); i++) 
	{
		cout << temp[i] << " ";
	}
	cout << endl;
	return cual;
}

int eliminarSanguijuelaModo3(SistemaBandas& unSistema, char metodo, vector<vector<double> > &sanguijuelasInfo, unsigned int matrixSize, unsigned int discrWidth, unsigned int discrHeight, double discrInterval)
{	//AQUELLAS sanguijuelasInfo QUE NO ACTUAN EN NINGUN PUNTO LAS DENOTO COMO -1, LAS QUE ACTUAN EN MAS DE 1 PUNTO COMO -2, Y LAS QUE SOLO EN UN PUNTO, LAS DENOTO CON LAS POS SOBRE LA QUE ACTUA QUE ES >=0
	
	unSistema.LU1(metodo);
	vector<vector<double> > L = unSistema.ObtenerL();
	vector<vector<double> > U = unSistema.ObtenerU();

	vector<double> temp;
	int cual = -1;
	double mejorTemp, nuevatemp;
	vector<double> solucion;
	for(int i = 0; i < sanguijuelasInfo.size(); i++)
	{
		if(sanguijuelasInfo[i][4] != -1)
		{
			if (sanguijuelasInfo[i][4] >= 0)
			{
				solucion = eliminar1sang(unSistema, sanguijuelasInfo[i][4], matrixSize, discrWidth, L, U);
			}
			else 
			{
				vector<vector<double> > nuevasSang = filtrar(sanguijuelasInfo, i);
				SistemaBandas nuevo_sistema(matrixSize, discrWidth - 1);
				solucion = obtenerTemperaturas(nuevo_sistema, matrixSize, discrWidth, discrHeight, discrInterval, nuevasSang, '0');
			}
				nuevatemp = dameTempPtoCritico(solucion, discrHeight-1, discrWidth-1);
				temp.push_back(nuevatemp);
				if (nuevatemp < 235 && (cual == -1 || nuevatemp < mejorTemp)) {mejorTemp = nuevatemp; cual = i;}
		}
	}
	for(int i = 0; i < temp.size(); i++) 
	{
		cout << temp[i] << " ";
	}
	cout << endl;
	return cual;
}


vector<double> eliminar1sang(SistemaBandas& unSistema, int f_v, unsigned int matrixSize, unsigned int discrWidth, vector<vector<double> > &L, vector<vector<double> > &U) //tal como viene
{

	
	vector<double> u;
	u.resize(matrixSize);// inicializar en cero
	u[f_v] = 1;

	vector<double> vt;
	vt.resize(5);// por cada fila hay a lo sumo 5 numeros distintos de cero, si estoy con la incognita x_i me interesa el valor de  x_i-1,  x_i+1.  x_i-ancho,  x_i+ancho
	double b_fv = 0;

	if((int)(f_v - (discrWidth -1)) >= 0) {vt[0] = -1; } else {vt[0] = 0; b_fv-= 100;}

	//if(f_v - 1 >= 0) {vt[1] = -1; } else {vt[1] = 0; b_fv-= 100;} //REVISAR BIEN ESTO

	if(f_v % (discrWidth -1) != 0) {vt[1] = -1; } else {vt[1] = 0; b_fv-= 100;}


	vt[2] = 3;

	//if(f_v +1  < matrixSize) {vt[3] = -1; } else {vt[3] = 0; b_fv-= 100;}

	if((f_v + 1) % (discrWidth -1) != 0) {vt[3] = -1; } else {vt[3] = 0; b_fv-= 100;}

	if((int)(f_v + (discrWidth -1)) < matrixSize ) {vt[4] = -1; } else {vt[4] = 0; b_fv-= 100;}	
	//de esta forma me queda [-1,-1,3,-1,-1]



	vector<double> nuevoB;
	for(int i = 0; i < matrixSize; i++)
	{
		if(i == f_v) { nuevoB.push_back(b_fv);} else {nuevoB.push_back(unSistema.Obtener(i,matrixSize));}
	}
	vector<double>  res = Sherman_Morrison( nuevoB, L, U, vt, u, f_v, discrWidth);
	return res;
}

vector<double>  Sherman_Morrison(vector<double> &nuevoB, vector<vector<double> > &L, vector<vector<double> > &U, vector<double> &vt, vector<double> &u, int f_v, unsigned int discrWidth)
{//pese a que u solo es un vector con un solo 1 por como esta implementado forward necesita de un vector, a menos que cree otra variante de forward
	//obtner A(-1)*nuevoB ES DECIR A*x = nuevoB , LU*x= nuevoB -> paso 1) Ly = nuevoB y paso 2) U*x = y 
	vector<double> y = ForWardSubstitution(nuevoB, L, discrWidth -1); //paso 1

	vector<double> invAxB = BackWardSubstitution2(y, U); // paso 2 falla acá
	//obtner A(-1)*u ES DECIR A*x = u , LU*x= u -> paso 1) Lz = u y paso 2) U*x = z 

	vector<double> z = ForWardSubstitution(u, L, discrWidth -1); //paso 1

	vector<double> invAxu = BackWardSubstitution2(z, U); // paso 2
	//resolver A(-1)*u*vt*A(-1).nuevoB; vt es de largo ancho*largo, no vale la pena un vector del tal tamaño Ya que lo sumo tiene tres -1s y un 3. 
	// k = vt*A(-1)
	double k = 0;

	if (vt[0]!=0) {k += invAxB[(int)(f_v - (discrWidth -1))]*vt[0];} 
	if(vt[1]!=0) {k += invAxB[f_v - 1]*vt[1];}
	k += invAxB[f_v]*vt[2];
	if(vt[3]!=0) {k += invAxB[f_v +1 ]*vt[3];} 
	if(vt[4]!=0) {k += invAxB[(int)(f_v + discrWidth -1)]*vt[4];}; // devido 
//a que los vectores no son del mismo tamaño solo multiplico las coordenadas correspondientes

	// resolver l = 1 + vt*A(-1)poru; 
	double l = 1;
	if (vt[0]!=0) {l += invAxu[(int)(f_v - (discrWidth -1))]*vt[0];} 
	if(vt[1]!=0) {l +=invAxu[f_v - 1]*vt[1];}
	l += invAxu[f_v]*vt[2]; 
	if(vt[3]!=0) {l += invAxu[f_v +1 ]*vt[3];} 
	if(vt[4]!=0) {l += invAxu[(int)(f_v + discrWidth -1)]*vt[4];};

	// resolver ahora (invAxu*k)/l ; segundo termindo de Morrinson

	for(int i = 0; i < invAxu.size(); i++)
	{
		invAxu[i] = (k/l)*invAxu[i];
	}
	//resta final
	for(int i = 0; i < invAxu.size(); i++)
	{
	invAxB[i] = invAxB[i] - invAxu[i];
	}
	return invAxB;
}

vector<double> BackWardSubstitution2(vector<double> &y, vector<vector<double> > &U)
{
	vector<double> result = vector<double>(U.size());
    for (int i = U.size() -1; i>=0; i--)
    {
        result[i] = y[i];
        for (int k=1, t = i+1; k< U[i].size(); k++, t++)
        {
            result[i] = result[i] - U[i][k]*result[t];
        }
        result[i] = result[i]/U[i][0];
    }
    return result;
}


vector<double> ForWardSubstitution(vector<double> &b, vector<vector<double> > &L, unsigned int discrWidth)
{
	vector<double> result = vector<double>(L.size());
    for (int i= 0; i < L.size(); i++)
    {
        result[i] = b[i];
        for (int k = max(0, (int)(i - discrWidth)), t = 0; k< i; k++, t++ )
        {
            result[i] = result[i] - L[i][t]*result[k];
        }
        //result[i] = result[i]/L[i][i]; la diagonal tiene 1s
    }
    return result;
}
