#include "util.h"

struct Spline
{
	vector<double> a, b, c, d;

	Spline(vector<double> _a, vector<double> _b, vector<double> _c, vector<double> _d) : a(_a), b(_b), c(_c), d(_d) {  }
};

struct Bloque
{
	int desdei;
	int desdej;
	int tam;

	Bloque(int i, int j, int t) : desdei(i), desdej(j), tam(t) {  }
};

//implementacion del algoritmo del burden pagina 149/150
Spline dame_Spline(vector<int>& x, vector<double>& a, unsigned int k)
{
	unsigned int n = x.size() - 1;
	double h_i = k + 1;
	vector<double> alpha(n + 1, 0);

	for(int i = 1; i < n; i++)
		alpha[i] = ((double)3 / h_i)*(a[i + 1] - 2 * a[i] + a[i - 1]);

	vector<double> l(n + 1, 0), u(n + 1, 0), z(n + 1, 0);
	l[0] = 1;

	for(int i = 1; i < n; i++)
	{
		l[i] = h_i * ((double)4 - u[i - 1]);
		u[i] = h_i / l[i];
		z[i] = (alpha[i] - h_i*z[i - 1]) / l[i];//antes era a[i]
	}

	vector<double> c(n + 1, 0), b(n, 0), d(n, 0);

	l[n] = 1;
	z[n] = 0;
	c[n] = 0;

	for(int j = n - 1; j >= 0; j--)
	{
		c[j] = z[j] - u[j]*c[j + 1];
		b[j] = (a[j + 1] - a[j]) / h_i - h_i * (c[j + 1] + (double)2 * c[j]) / (double)3;
		d[j] = (c[j + 1] - c[j]) / ((double)3 * h_i);
	}

	a.pop_back();

	Spline s(a, b, c, d);

	return s;
}

void MetodoSpline(Bloque b, vector<vector<int> >& imagen, int k)
{
	vector<Spline> splinesPorFila, splinesPorColumna;

	//me armo un spline por cada fila que contiene pixeles originales (es decir las filas que tienen todos -1 no las uso)
    for(int i = b.desdei; i < b.desdei + b.tam; i += k + 1)
    {
    	vector<int> x;
    	vector<double> a;

    	for(int j = b.desdej; j < b.desdej + b.tam; j += k + 1)
    	{
    		x.push_back(j);
    		a.push_back(imagen[i][j]);
    	}
 
    	splinesPorFila.push_back(dame_Spline(x, a, k));
    }

    //recorro cada una de las filas que tienen pixeles originales para calcular los pixeles que hay entre medio de cada par de
    //pixeles originales (es decir los que tienen -1)
    for(int i = b.desdei; i < b.desdei + b.tam; i += k + 1)		//salto de a k + 1 filas para no tocar las que tienen todos -1
    {
    	int p = 0;			//p va a ser el punto "anterior" al punto que quiero calcular usando interpolacion, es decir si quiero calcular S(x) (S es el spline) con x el punto con el -1, p seria el punto correspondiente al pixel original anterior a x

    	for(int j = b.desdej + 1; j < b.desdej + b.tam; j += k + 1, p++)	//salto de a k + 1 columnas para no tocar los pixeles originales, con el proximo for voy rellenando los pixeles nuevos entre cada par de pixeles originales
    	{
    		//como voy saltando d a k + 1 filas y para calcular los splines por fila hicimos lo mismo, divido por k + 1 para obtener el spline correspondiente a la fila con la que estoy trabajando
    		double a_j = splinesPorFila[(i - b.desdei) / (k + 1)].a[p];
    		double b_j = splinesPorFila[(i - b.desdei) / (k + 1)].b[p];
    		double c_j = splinesPorFila[(i - b.desdei) / (k + 1)].c[p];
    		double d_j = splinesPorFila[(i - b.desdei) / (k + 1)].d[p];

    		for(int t = 0; t < k; t++)
    		{
	    		//Lo siguiente es evaluar el polinomio en el punto, en realidad esta simplificando porque:
	    		//t + 1 = j + t - p = j + t - j + 1 = t + 1, donde j + t es el punto que quiero calcular y p es el punto
	    		//anterior. Se calcula asi porque el algoritmo para hallar el spline te lo devuelve expresado de determinada manera,
	    		//que se puede ver en el burden en la pagina 149 donde dice "Note" entre parentesis debajo de OUTPUT
	    		int interpolacion = a_j + b_j * (t + 1) + c_j * (t + 1) * (t + 1) + d_j * (t + 1) * (t + 1) * (t + 1);

	    		if(interpolacion > 255)
	    			interpolacion = 255;
	    		if(interpolacion < 0)
	  				interpolacion = 0;

	    		imagen[i][j + t] = interpolacion;
    		}
    	}
    }

    //Ahora es simetrico, como por cada fila donde habia pixeles originales ahora estan calculados todos los pixeles nuevos
    //entre cada par de pixeles originales, faltan calcular los pixeles de las filas que estan entre cada par de filas de
    //pixeles originales. O sea las filas que tienen todos -1.
    //Para eso me armo un spline por columna y despues el recorrido y demas es todo simetrico al paso anterior.
    for(int j = b.desdej; j < b.desdej + b.tam; j++)
    {
    	vector<int> x;
    	vector<double> a;

    	for(int i = b.desdei; i < b.desdei + b.tam; i += k + 1)
    	{
    		x.push_back(i);
    		a.push_back(imagen[i][j]);
    	}
 
    	splinesPorColumna.push_back(dame_Spline(x, a, k));
    }

    for(int j = b.desdej; j < b.desdej + b.tam; j++)
    {
    	int p = 0;
    	
    	for(int i = b.desdei + 1; i < b.desdei + b.tam; i += k + 1, p++)
    	{
			double a_i = splinesPorColumna[j - b.desdej].a[p];
    		double b_i = splinesPorColumna[j - b.desdej].b[p];
    		double c_i = splinesPorColumna[j - b.desdej].c[p];
    		double d_i = splinesPorColumna[j - b.desdej].d[p];

    		for(int t = 0; t < k; t++)
    		{		    				
	    		int interpolacion = a_i + b_i * (t + 1) + c_i * (t + 1) * (t + 1) + d_i * (t + 1) * (t + 1) * (t + 1);

	    		if(interpolacion > 255)
	    			interpolacion = 255;
	    		if(interpolacion < 0)
	    			interpolacion = 0;

	    		imagen[i + t][j] = interpolacion;
    		}
    	}
    }
}