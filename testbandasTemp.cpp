#include "SistemaBandas.h"
#include "fileHandler.h"
#include <sstream>
#include <math.h>

void cargarValores(SistemaBandas& unSistema,unsigned int alturaSistema, unsigned int ancho);
bool enCirculo(unsigned int posX, unsigned int posY, double discr, unsigned int circlX, unsigned int circlY, double radio);
void puntosSanguijuela(unsigned int filas, unsigned int columnas, unsigned int punX, unsigned int punY, double discr, unsigned int sangX, unsigned int sangY, double radio, vector<pair<unsigned int, unsigned int> >& resultado);

int main()
{

	FileHandler myFile("./test2.in");
	vector<double> params;
	params.resize(4);
	myFile.readParameters(params);

	double width = params[0];
	double heigth = params[1];
	double discrInterval = params[2];
	unsigned int nLeeches = params[3];

	unsigned int discrHeigth = (unsigned int)heigth/discrInterval;
	unsigned int discrWidth = (unsigned int)width/discrInterval;
	unsigned int matrixSize = discrHeigth*discrWidth;

	vector<double> sanguijuelas = myFile.readLeeches(nLeeches);


	SistemaBandas sistema(matrixSize, 2*discrWidth+1);

	for(unsigned int i = 0; i < sanguijuelas.size(); i=i+4){

		unsigned int sanguijuelaX = (unsigned int)sanguijuelas[i]/discrInterval;
		unsigned int sanguijuelaY = (unsigned int)sanguijuelas[i+1]/discrInterval;
		double sanguijuelaR = sanguijuelas[i+2];
		int sanguijuelaT = sanguijuelas[i+3];

		vector<pair<unsigned int, unsigned int> > punSang;
		cout << "NUEVO" << endl;
		cout << sanguijuelaX << " " << sanguijuelaY << " " << sanguijuelaR << endl;
		puntosSanguijuela(discrHeigth, discrWidth, sanguijuelaX, sanguijuelaY, discrInterval, sanguijuelaX, sanguijuelaY, sanguijuelaR, punSang);

		for(unsigned int i = 0; i < punSang.size(); i++){
			cout << "PUNTO:" << endl << punSang[i].first << "," << punSang[i].second << endl;
		}
		//sistema.Modificar()
	}
	return 0;
	cargarValores(sistema, matrixSize, discrWidth);


	sistema.Mostrar();

	sistema.EliminacionGaussiana();
	cout << endl;
	sistema.Mostrar();

	vector <double> res = sistema.BackWardSubstitution();

	return 0;
}


void cargarValores(SistemaBandas& unSistema, unsigned int alturaSistema, unsigned int ancho){
	//unsigned int middleIndex =
	for(unsigned int posicion = 0; posicion < alturaSistema; posicion++){
		if(posicion < ancho || posicion > (alturaSistema-ancho) || (posicion+1) % ancho == 0 || (posicion+1) % ancho == 1){
			unSistema.Modificar(posicion, posicion, 1);
			unSistema.Modificar(posicion, posicion+ancho+1, -100);

		}else{
			unSistema.Modificar(posicion,posicion, 4);
			unSistema.Modificar(posicion,posicion-1, 2);
			unSistema.Modificar(posicion,posicion+1, 3);
			unSistema.Modificar(posicion,posicion+ancho, 4);
			unSistema.Modificar(posicion,posicion-ancho, 5);
		}
	}
}

bool enCirculo(unsigned int posX, unsigned int posY, double discr, unsigned int circlX, unsigned int circlY, double radio){
	return ( pow((posX*discr - circlX*discr),2) + pow((posY*discr - circlY*discr),2) <= pow(radio,2) );
}

void puntosSanguijuela(unsigned int filas, unsigned int columnas, unsigned int punX, unsigned int punY, double discr, unsigned int sangX, unsigned int sangY, double radio, vector<pair<unsigned int, unsigned int> >& resultado){

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
