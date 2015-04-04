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
	void RestarFila(double coeficiente, unsigned int primera, unsigned int segunda, unsigned int desdeColumna, char metodo);
	void EliminacionGaussiana0(char metodo); // eg0 aprovecha que es bandas, por lo que no resta toda una fila, ni para triangular debe descender hasta la última fila
	void LU1(char metodo); // se utiliza con LU 
	void CerosAIzquierda(unsigned int fila, unsigned int hastaColumna);
	vector<double> BackWardSubstitution();
	vector<vector<double> > ObtenerL();
	vector<vector<double> > ObtenerU();
	vector<double> ObtenerTempModo1();

private:
	vector<vector<double> > matriz;
	pair<int, int> indexar(unsigned int i, unsigned int j) const;
	unsigned int filas, columnas, ancho;
};
