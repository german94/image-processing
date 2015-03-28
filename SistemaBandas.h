#include <iostream>
#include <vector>

using namespace std;

class SistemaBandas
{
public:
	SistemaBandas(unsigned int _filas, unsigned int _ancho);

	double Obtener(unsigned int i, unsigned int j) const;
	void Modificar(unsigned int i, unsigned int j, double valor);
	void Mostrar() const;
	void RestarFila(double coeficiente, unsigned int primera, unsigned int segunda, unsigned int desdeColumna);
	void EliminacionGaussiana();
	void CerosAIzquierda(unsigned int fila, unsigned int hastaColumna);

private:
	vector<vector<double> > matriz;
	pair<int, int> indexar(unsigned int i, unsigned int j) const;
	unsigned int filas, columnas, ancho;
};