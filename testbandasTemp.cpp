#include "SistemaBandas.h"
#include "fileHandler.h"
#include <sstream>
#include <math.h>

void cargarValores(SistemaBandas& unSistema,unsigned int alturaSistema, unsigned int ancho);
void cargarValoresSinBordes(SistemaBandas& unSistema, unsigned int alturaSistema, unsigned int anchoDiscrSinBordes, unsigned int altoDiscrSinBordes);
void cargarSanguijuelas(SistemaBandas& unSistema, unsigned int alturaSistema, unsigned int discrHeight, unsigned int discrWidth, double discrInterval, vector<double> sanguijuelasInput);
bool enCirculo(unsigned int posX, unsigned int posY, double discr, double circlX, double circlY, double radio);
void puntosSanguijuela(unsigned int filas, unsigned int columnas, unsigned int punX, unsigned int punY, double discr, double sangX, double sangY, double radio, vector<pair<unsigned int, unsigned int> >& resultado);

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

	unsigned int discrHeight = (unsigned int)heigth/discrInterval;
	unsigned int discrWidth = (unsigned int)width/discrInterval;
	unsigned int matrixSize = (discrHeight - 1)*(discrWidth - 1);		//no hay que contar los bordes, si ponemos discrHeight*discWidth estamos contando un borde de cada dimension

	SistemaBandas sistema(matrixSize, discrWidth - 1);

	vector<double> sanguijuelas = myFile.readLeeches(nLeeches);

	cargarValoresSinBordes(sistema, matrixSize, discrWidth - 1, discrHeight - 1);		//primero armo el sistema como si no tuviera ninguna sanguijuela
	cargarSanguijuelas(sistema, matrixSize, discrHeight, discrWidth, discrInterval, sanguijuelas);		//despues piso las filas necesarias con los datos cuando cargo las sanguijuelas

	// for(int x =0 ; x < matrixSize; x++)
	// {
	// 	for(int y =0; y < matrixSize + 1; y++)
	// 		cout<<sistema.Obtener(x, y)<<" ";

	// 	cout<<endl;
	// }

	sistema.EliminacionGaussiana();

	vector <double> res = sistema.BackWardSubstitution();
	for(unsigned int i = 0; i < discrWidth + 1; i++)
	{
		for(unsigned int j = 0; j < discrHeight + 1; j++)
		{
			double showValue = 0;
			if(i == 0 || j == 0 || i == discrWidth || j == discrWidth)
				showValue = -100;
			else
				showValue = res[(i - 1) * (discrWidth - 1) + j];	// (i - 1) porque discrWidth va de 0 a matrixSize - 1 y matrixSize no cuenta bordes

			cout << i<<"\t"<<j<<"\t"<<showValue << endl;
		}
	}

	return 0;
}


void cargarSanguijuelas(SistemaBandas& unSistema, unsigned int alturaSistema, unsigned int discrHeight, unsigned int discrWidth, double discrInterval, vector<double> sanguijuelasInput){
	for(unsigned int i = 0; i < sanguijuelasInput.size(); i=i+4){

		//Las sanguijuelas pueden no caer en la discretizacion (es decir, que sanguijuelaX y sanguijuelaY no sean enteros). 
		//Si no caen en la discretizacion no hay que "hacer que caigan" porque estariamos moviendo la sanguijuela de lugar y podrian cambiar los puntos de la discretizacion afectados.
		double sanguijuelaX = sanguijuelasInput[i]/discrInterval;
		double sanguijuelaY = sanguijuelasInput[i+1]/discrInterval;

		double sanguijuelaR = sanguijuelasInput[i+2];
		double sanguijuelaT = sanguijuelasInput[i+3];

		vector<pair<unsigned int, unsigned int> > punSang;

		//Si la sanguijuela esta situada justo en un punto de la discretizacion entonces todo bien, pero si no, el primer punto sobre el cual va a actuar
		//(el primero en la recursion de puntosSanguijuela) va a ser el punto dado por la parte entera de ambas coordenadas y despues se ira moviendo a partir
		//de ese punto.
		puntosSanguijuela(discrHeight - 1, discrWidth - 1, floor(sanguijuelaX), floor(sanguijuelaY), discrInterval, sanguijuelaX, sanguijuelaY, sanguijuelaR, punSang);

		for(unsigned int j = 0; j < punSang.size(); j++){

			unsigned int posicionEnMatriz = punSang[j].first + punSang[j].second*(discrWidth - 1); //modificar


			for(int i = 0; i < alturaSistema; i++)
				unSistema.Modificar(posicionEnMatriz, i, 0);
			
			unSistema.Modificar(posicionEnMatriz, posicionEnMatriz, 1);
			unSistema.Modificar(posicionEnMatriz, alturaSistema, sanguijuelaT);
		}
	}
}


void cargarValores(SistemaBandas& unSistema, unsigned int alturaSistema, unsigned int ancho){
	for(unsigned int posicion = 0; posicion < alturaSistema; posicion++){
		if(unSistema.Obtener(posicion, posicion) == 0){

			if(posicion < ancho || posicion > (alturaSistema-ancho) || (posicion+1) % ancho == 0 || (posicion+1) % ancho == 1){
				unSistema.Modificar(posicion, posicion, 1);
				unSistema.Modificar(posicion, alturaSistema, -100);

			}else{
				unSistema.Modificar(posicion,posicion, -4);
				unSistema.Modificar(posicion,posicion-1, 1);
				unSistema.Modificar(posicion,posicion+1, 1);
				unSistema.Modificar(posicion,posicion+ancho, 1);
				unSistema.Modificar(posicion,posicion-ancho, 1);
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

		if(abajo.second >= alturaSistema)
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

//circlX y circlY representan la posicion de la sanguijuela, deben ser double por si no cae dentro de la discretizacion
bool enCirculo(unsigned int posX, unsigned int posY, double discr, double circlX, double circlY, double radio){
	return ( pow((posX*discr - circlX*discr),2) + pow((posY*discr - circlY*discr),2) <= pow(radio,2) );
}

//lo mismo, sangX y sangY pueden no caer en la discretizacion, no deben ser int
void puntosSanguijuela(unsigned int filas, unsigned int columnas, unsigned int punX, unsigned int punY, double discr, double sangX, double sangY, double radio, vector<pair<unsigned int, unsigned int> >& resultado){

	if(enCirculo(punX, punY, discr, sangX, sangY, radio) && punX < columnas -1 && punY < filas -1 && punX > 1 && punY > 1 ){
		for(unsigned int i=0; i < resultado.size(); i++){
			if(resultado[i].first == punX && resultado[i].second == punY){
				return;
			}
		}
		pair<unsigned int, unsigned int> punNuevo (punX, punY);

		resultado.push_back(punNuevo);
		puntosSanguijuela(filas, columnas, punX-1, punY, discr, sangX, sangY, radio, resultado);
		puntosSanguijuela(filas, columnas, punX+1, punY, discr, sangX, sangY, radio, resultado);
		puntosSanguijuela(filas, columnas, punX, punY-1, discr, sangX, sangY, radio, resultado);
		puntosSanguijuela(filas, columnas, punX, punY+1, discr, sangX, sangY, radio, resultado);
	}
	return;
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
