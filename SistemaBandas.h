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
	void RestarFila0(double coeficiente, unsigned int primera, unsigned int segunda, unsigned int desdeColumna);
	void EliminacionGaussiana0();//restarfila0 y eg0 son las funciones standards 
	void RestarFila1(double coeficiente, unsigned int primera, unsigned int segunda, unsigned int desdeColumna);
	void EliminacionGaussiana1(); //restarfila1 y eg1 aprovecha que es bandas, por lo que no resta toda una fila, ni para triangular debe descender hasta la última fila
	void CerosAIzquierda(unsigned int fila, unsigned int hastaColumna);
	vector<double> BackWardSubstitution();

private:
	vector<vector<double> > matriz;
	pair<int, int> indexar(unsigned int i, unsigned int j) const;
	unsigned int filas, columnas, ancho;
};
