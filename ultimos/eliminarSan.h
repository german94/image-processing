#include <iostream>
#include <vector>

using namespace std;

vector<double> obtenerTemperaturas(SistemaBandas& sistema, unsigned int matrixSize, unsigned int discWidth, unsigned int discrHeight, double discrInterval, vector<vector<double> >& sanguijuelasInfo, char metodo);

void cargarValoresSinBordes(SistemaBandas& unSistema, unsigned int alturaSistema, unsigned int anchoDiscrSinBordes, unsigned int altoDiscrSinBordes);

void cargarSanguijuelas(SistemaBandas& unSistema, unsigned int alturaSistema, unsigned int discrHeight, unsigned int discrWidth, double discrInterval, vector<vector<double> >& sanguijuelasInfo);

bool enCirculo(unsigned int posX, unsigned int posY, double discr, double circlX, double circlY, double radio);

void puntosSanguijuela(unsigned int filas, unsigned int columnas, double discr, double sangX, double sangY, double radio, vector<pair<unsigned int, unsigned int> >& resultado);

int eliminarSanguijuelaModo2(SistemaBandas& unSistema, char metodo, vector<vector<double> > &sanguijuelasInfo, unsigned int matrixSize, unsigned int discrWidth, unsigned int discrHeight, double discrInterval);

int eliminarSanguijuelaModo3(SistemaBandas& unSistema, char metodo, vector<vector<double> > &sanguijuelasInfo, unsigned int matrixSize, unsigned int discrWidth, unsigned int discrHeight, double discrInterval);

vector<double> eliminar1sang(SistemaBandas& unSistema, int f_v, unsigned int matrixSize, unsigned int discrWidth, vector<vector<double> > &L, vector<vector<double> > &U);

vector<double>  Sherman_Morrison(vector<double> &nuevoB, vector<vector<double> > &L, vector<vector<double> > &U, vector<double> &vt, vector<double> &u, int f_v, unsigned int discrWidth);

vector<double> BackWardSubstitution2(vector<double> &y, vector<vector<double> > &U);

vector<double> ForWardSubstitution(vector<double> &b, vector<vector<double> > &L, unsigned int discrWidth);

vector<vector<double> > filtrar(vector<vector<double> > &sanguijuelasInfo, int i);

double dameTempPtoCritico(vector<double> &solucion, unsigned int alto, unsigned int ancho);
