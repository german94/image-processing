#include "SistemaBandas.h"
#include "fileHandler.h"
#include <sstream>
#include <math.h>
#include "stdio.h"
#include "stdlib.h"
#include <iomanip>

vector<double> obtenerTemperaturas(SistemaBandas& sistema, vector<vector<double> > &sanguijuelas, unsigned int matrixSize, unsigned int discWidth, unsigned int discrHeight, double discrInterval);
void cargarValoresSinBordes(SistemaBandas& unSistema, unsigned int alturaSistema, unsigned int anchoDiscrSinBordes, unsigned int altoDiscrSinBordes);
void cargarSanguijuelas(SistemaBandas& unSistema, unsigned int alturaSistema, unsigned int discrHeight, unsigned int discrWidth, double discrInterval, vector<double> sanguijuelasInput);
bool enCirculo(unsigned int posX, unsigned int posY, double discr, double circlX, double circlY, double radio);
void puntosSanguijuela(unsigned int filas, unsigned int columnas, double discr, double sangX, double sangY/*SIN DISC*/, double radio, vector<pair<unsigned int, unsigned int> >& resultado);

int main()
{

	FileHandler myFile("./test1.in");
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

	vector<vector<double> > sanguijuelas = myFile.readLeeches(nLeeches);

	vector<double> res = obtenerTemperaturas(sistema, sanguijuelas, matrixSize, discrWidth, discrHeight, discrInterval);

	std::cout << std::fixed << std::setprecision(5);
	
	for(int i = 0; i <= discrHeight; i++) //fila 
	{
		for(int j = 0; j <= discrWidth; j++) //columna
		{
			if(i == 0 || j==0 || i == discrHeight || j == discrWidth)
			{
				cout << i << "\t" << j << "\t" <<fixed<< (double)-100 << endl; 
			}
			else
			{
				cout << i << "\t" << j << "\t" <<fixed<< (double)res[(i - 1)*(discrWidth - 1) + j-1] << endl; 
			}
		}
	}

	return 0;
}

vector<double> obtenerTemperaturas(SistemaBandas& sistema, vector<vector<double> > &sanguijuelas, unsigned int matrixSize, unsigned int discrWidth, unsigned int discrHeight, double discrInterval)
	{
		cargarValoresSinBordes(sistema, matrixSize, discrWidth - 1, discrHeight - 1);		//primero armo el sistema como si no tuviera ninguna sanguijuela
		cargarSanguijuelas(sistema, matrixSize, discrHeight, discrWidth, discrInterval, sanguijuelas);		//despues piso las filas necesarias con los datos cuando cargo las sanguijuelas
		sistema.EliminacionGaussiana1();
		vector <double> res = sistema.BackWardSubstitution();
		return res;
	}

	//cargarSanguijuelas(sistema, matrixSize, discrHeight /*alto como viene*/, discrWidth/*ancho como vienen*/, discrInterval, sanguijuelas);		//despues piso las filas necesarias con los datos cuando cargo las sanguijuelas
void cargarSanguijuelas(SistemaBandas& unSistema, unsigned int alturaSistema, unsigned int discrHeight, unsigned int discrWidth, double discrInterval, vector<vector<double> > sanguijuelasInput){
	for(unsigned int i = 0; i < sanguijuelasInput.size(); i=i+4){

		//Las sanguijuelas pueden no caer en la discretizacion (es decir, que sanguijuelaX y sanguijuelaY no sean enteros). 
		//Si no caen en la discretizacion no hay que "hacer que caigan" porque estariamos moviendo la sanguijuela de lugar y podrian cambiar los puntos de la discretizacion afectados.
		double sanguijuelaX = sanguijuelasInput[i];
		double sanguijuelaY = sanguijuelasInput[i+1];

		double sanguijuelaR = sanguijuelasInput[i+2];
		double sanguijuelaT = sanguijuelasInput[i+3];

		vector<pair<unsigned int, unsigned int> > punSang; //LOS PUNTOS SOBRE LOS CUALES ACTUA LA SANG
							//ALTO		//ANCHO
		puntosSanguijuela(discrHeight, discrWidth, discrInterval, sanguijuelaX, sanguijuelaY/*SIN DISC*/, sanguijuelaR, punSang);

		unsigned int camb_col, camb_fil; //VALORES DENTRO DE LA MATRIZ A MODIFICAR
		for(unsigned int j = 0; j < punSang.size(); j++)
		{ //punSANG TIENE TODOS LOS PUNTOS SOBRE LOS QUE ACTUA
			
			camb_col = punSang[j].first;
			camb_fil = punSang[j].second;
			unsigned int filaEnMatriz = (discrWidth-1)*(camb_fil -1) + camb_col -1; //ya me lo da indexado en cero

			/*if((filaEnMatriz + 1) % (discrWidth -1) != 0) {unSistema.Modificar(filaEnMatriz, filaEnMatriz -1, 0);} //al X_i,i-1 lo dejo en 0 IZQUIERDA
			if(filaEnMatriz != alturaSistema -1) {unSistema.Modificar(filaEnMatriz, filaEnMatriz -1, 0);} //al X_i,i-1 lo dejo en 0  DERECHA

			if((alturaSistema -1) - filaEnMatriz >= discrWidth) {unSistema.Modificar(filaEnMatriz, filaEnMatriz - (discrWidth -1=), 0);} //al X_i-ancho,i lo dejo en 0  ARRIBA
			if(filaEnMatriz >= discrWidth - 1) {unSistema.Modificar(filaEnMatriz, filaEnMatriz + (discrWidth - 1), 0);} //al X_i+ancho,ilo dejo en 0  ABAJO
*/
			//CREERIA QUE SI QUIERO ACCEDER A ALGO QUE ESTA FUERA DE RANGO NO HACE NADA , PREGUNTAR

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


//circlX y circlY representan la posicion de la sanguijuela, deben ser double por si no cae dentro de la discretizacion

bool enCirculo(unsigned int posX , unsigned int posY, double discr, double circlX, double circlY/*reales*/, double radio){
	return ( pow((posX*discr - circlX),2) + pow((posY*discr - circlY),2) <= pow(radio,2) );
}

void puntosSanguijuela(unsigned int filas/*como viene*/, unsigned int columnas/*como viene*/, double discr, double sangX, double sangY/*SIN DISC*/, double radio, vector<pair<unsigned int, unsigned int> >& resultado)
{
	for(int f_s = 1; f_s < filas; f_s++)
	{
		for(int c_s = 1; c_s < columnas; c_s++)
		{
			pair<unsigned int, unsigned int> punNuevo (c_s, f_s);
			if(enCirculo(c_s, f_s, discr, sangX, sangY, radio))
			{	
				resultado.push_back(punNuevo);
			}
		}	
	}
}

//La de arriba carga la matriz con los bordes, dijimos que ibamos a cargar la matriz sin los bordes
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

double dameTempPtoCritico(vector<double> solucion, unsigned int alto, unsigned int ancho){
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
