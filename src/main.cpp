#include "splines.h"
#include "bilineal.h"
#include "vecino.h"
#include "aux.h"

using namespace std;

int main(int argc, char *argv[])
{
	string matlabCommand1 = "nohup matlab -nodisplay -nosplash -nojvm -r \"Image2Csv('";
	matlabCommand1 += argv[1];
	matlabCommand1 += "');quit\"";

	system(matlabCommand1.c_str());

	string newFile = "";
	newFile += argv[1];
	newFile += ".csv";


	int k = string_to_type<unsigned int>(argv[2]);

	Metodo metodo = (Metodo)string_to_type<int>(argv[3]);


	vector<vector<int> > expandida;
	init_time();
	preprocesarImagen(newFile, k, expandida);


    switch(metodo){

    	case VECINO:
    	{
            if(argc != 5) {
                usageVecino();
                return 0;
                }

 	cout<<"Vecino \n";

            SubMetodo submetodo = (SubMetodo) string_to_type<int>(argv[4]);
    		switch(submetodo){

				case ORGINALES:
			    {
			    	cout<<"Original\n";
			    	origin(expandida, k);
			    	break;
				}

				case CALCULADOSF:
				{
					cout<<"Fila\n";
					dinamico(expandida,k);
					break;
				}
				case PROM:
				{
					cout<<"Promedio\n";
					vecinoProm(expandida);
					break;
				}
			}

	        grabarImagen(argv[1], expandida);

	        break;
		}

		case SPLINE:
    	{
    		  if(argc != 5) {
                usageSpline();
                return 0;
                }

    		int tamBloque = string_to_type<int>(argv[4]);
    		int tam = tamBloque + (tamBloque - 1) * k;

    		for(int i = 0; i < expandida.size(); i += tam - 1)
    		{
    			bool terminarImagen = false;
    			if(i + tam > expandida.size())
				{
					int dif = i + tam - 1 - expandida.size();
					i -= dif + 1;
					terminarImagen = true;
					if(i + tam > expandida.size())
						break;
				}

    			for(int j = 0; j < expandida[0].size(); j += tam - 1)
    			{
    				bool terminarFila = false;
    				if(j + tam > expandida[0].size())
    				{
						int dif = j + tam - 1 - expandida[0].size();
						j -= dif + 1;
						terminarFila = true;
						if(j + tam > expandida.size())
							break;
					}

    				Bloque b(i, j, tam);
    				MetodoSpline(b, expandida, k);

    				if(terminarFila)
    				{
    					terminarFila = false;
    					break;
    				}
    			}

    			if(terminarImagen)
    				break;
    		}

	        grabarImagen(argv[1], expandida);

	        break;
		}

		case SPLINEVARIABLE:
    	{

            if(argc != 5) {
                usage();
                return 0;
                }
            int alpha = string_to_type<int>(argv[4]);

    		//me armo un spline por cada fila que contiene pixeles originales (es decir las filas que tienen todos -1 no las uso)
		    for(int i = 0; i < expandida.size(); i = i + k + 1)//i recorre las filar de la imagen expandida
		    {
		    	vector <vector<int> > x;
		    	vector<int> subs_x;
		    	x.push_back(subs_x);

		    	vector <vector<double> > a;
		    	vector<double> subs_a;
		    	a.push_back(subs_a);

		    	int particion = 0;
		    	for(int j = 0; j < expandida[i].size() - 1; j += k + 1)//j recorre las columnas para determinar hasta donde voy a formar los splines
		    	{//de una misma fila
		    		a[particion].push_back(expandida[i][j]);
		    		x[particion].push_back(j); //ahora por cada fila voy a formar distintos splines, es como aplicar varias veces para una misma
		    		// fila la funcion dame_splines
		    		if (abs (expandida[i][j] - expandida[i][j + k + 1]) >= alpha) 
		    		{
		    			vector<double> nueva_sub_a;
		    			a.push_back(nueva_sub_a);
		    			vector<int> nueva_sub_x;
		    			x.push_back(nueva_sub_x);
		    			particion++;
		    		}
		    	}

		    	a[particion].push_back(expandida[i][expandida[i].size() -1]); //el ultimo elemento no queda pusheado en el for anterior
		 		//deberia quedar un vector con los splines que quiero formar
		    	x[particion].push_back(expandida[i].size() -1);

		    	for(int t = 0 ; t < a.size(); t++)
		    	{		//t va a recorrer cada particion que arme en la fila_i y voy a formar a.size() splines por filas
		    		if(a[t].size() > 1) // si solo tengo un elemento no hago nada
		    		{
			    		Spline sub_spline = dame_Spline(x[t], a[t], k);

				    	int p = 0;			//p va a ser el punto "anterior" al punto que quiero calcular usando interpolacion, es decir si quiero calcular S(x) (S es el spline) con x el punto con el -1, p seria el punto correspondiente al pixel original anterior a x
				    	for(int j = x[t][0] + 1; j < x[t][x[t].size() - 1]; j += k + 1, p++)	// este j recorre todas las columnas de cada particion de a o x
				    	{
				    		double a_j = sub_spline.a[p];
				    		double b_j = sub_spline.b[p];
				    		double c_j = sub_spline.c[p];
				    		double d_j = sub_spline.d[p];

				    		for(int x_t = 0; x_t < k; x_t++)// en cada vector dentro de x tengo los indices desde donde quiero que empiece y termine cada spline
				    		{
					    		int interpolacion = a_j + b_j * (j + x_t - x[t][p]) + c_j * (j + x_t - x[t][p]) * (j + x_t - x[t][p]) + d_j * (j + x_t - x[t][p]) * (j + x_t - x[t][p]) * (j + x_t - x[t][p]);

					    		if(interpolacion > 255)
					    			interpolacion = 255;

					    		expandida[i][j + x_t] = interpolacion;
				    		}
				    	}
					}
					if(t != a.size() - 1)
					{
						int desde = x[t][x[t].size() -1] +1;
						int hasta = x[t+1][0] -1;
						for(int r = desde ; r <= (desde + hasta)/2 ; r++) {expandida[i][r] = expandida[i][desde -1 ];}
						for(int s = (desde + hasta)/2 + 1 ; s <= hasta ; s++) {expandida[i][s] = expandida[i][hasta + 1];}
					}
					//ahora voy a rellenar entre la posicion donde termina un spline con donde comienza el otro con algo como metodo mas cercano
				}
			}
		    //Ahora es simetrico,

		    for(int i = 0; i < expandida[0].size(); i++)//i recorre las cols de la imagen expandida
		    {
		    	vector <vector<int> > x;
		    	vector<int> subs_x;
		    	x.push_back(subs_x);

		    	vector <vector<double> > a;
		    	vector<double> subs_a;
		    	a.push_back(subs_a);

		    	int particion = 0;
		    	for(int j = 0; j < expandida.size() - 1; j += k + 1)//j recorre las filas para determinar hasta donde voy a formar los splines
		    	{//de una misma fila
		    		a[particion].push_back(expandida[j][i]);
		    		x[particion].push_back(j); //ahora por cada fila voy a formar distintos splines, es como aplicar varias veces para una misma
		    		// fila la funcion dame_splines
		    		if (abs (expandida[j][i] - expandida[j + k + 1][i]) >= alpha)
		    		{
		    			vector<double> nueva_sub_a;
		    			a.push_back(nueva_sub_a);
		    			vector<int> nueva_sub_x;
		    			x.push_back(nueva_sub_x);
		    			particion++;
		    		}
		    	}
		    	a[particion].push_back(expandida[expandida.size() -1][i]); //el ultimo elemento no queda pusheado en el for anterior
		 		//deberia quedar un vector con los splines que quiero formar
		    	x[particion].push_back(expandida.size() -1);

		    	for(int t = 0 ; t < a.size(); t++)
		    	{		//t va a recorrer cada particion que arme en la fila_i y voy a formar a.size() splines por filas
		    		if(a[t].size() > 1) // si solo tengo un elemento no hago nada
		    		{
			    		Spline sub_spline = dame_Spline(x[t], a[t], k);

				    	int p = 0;			//p va a ser el punto "anterior" al punto que quiero calcular usando interpolacion, es decir si quiero calcular S(x) (S es el spline) con x el punto con el -1, p seria el punto correspondiente al pixel original anterior a x
				    	for(int j = x[t][0] + 1; j < x[t][x[t].size() - 1]; j += k + 1, p++)	// este j recorre todas las filas de cada particion de a o x
				    	{
				    		double a_j = sub_spline.a[p];
				    		double b_j = sub_spline.b[p];
				    		double c_j = sub_spline.c[p];
				    		double d_j = sub_spline.d[p];

				    		for(int x_t = 0; x_t < k; x_t++)// en cada vector dentro de x tengo los indices desde donde quiero que empiece y termine cada spline
				    		{
					    		int interpolacion = a_j + b_j * (j + x_t - x[t][p]) + c_j * (j + x_t - x[t][p]) * (j + x_t - x[t][p]) + d_j * (j + x_t - x[t][p]) * (j + x_t - x[t][p]) * (j + x_t - x[t][p]);

					    		if(interpolacion > 255)
					    			interpolacion = 255;

					    		expandida[j + x_t][i] = interpolacion;
				    		}
				    	}
					}
					if(t != a.size() - 1)
					{
						int desde = x[t][x[t].size() -1] +1;
						int hasta = x[t+1][0] -1;
						for(int r = desde ; r <= (desde + hasta)/2 ; r++) {expandida[r][i] = expandida[desde -1][i];}
						for(int s = (desde + hasta)/2 + 1 ; s <= hasta ; s++) {expandida[s][i] = expandida[hasta + 1][i];}
					}
					//ahora voy a rellenar entre la posicion donde termina un spline con donde comienza el otro con algo como metodo mas cercano
				}
			}
		 //    ///////////muestro
			grabarImagen(argv[1], expandida);


	        break;
		}

		case BILINEAL:
    	{
		     if(argc != 4) {
                usage();
                return 0;
                }

		    int dato_f, dato_c;
		    for(int i = 0; i < expandida.size(); i++)
		    {
		    	dato_c = 0;
		    	for(int j = 0; j < expandida[i].size() ; j++)
		    	{
		    		if(j == dato_c + k + 1 && j != expandida[i].size() -1) {dato_c = j;}
		    		if(expandida[i][j] != -1)
		    		{
		    			if(i != expandida.size() - 1)  {dato_f = i;}
		    		}
		    		else
		    		{
		    			expandida[i][j] = calculo_bilineal(k, dato_c, dato_f, i, j, expandida);
		    		}
		    	}
		    }

		    grabarImagen(argv[1], expandida);

	        break;
		}

		case BILINEALBIS:
		{

			if(argc != 5)
			{
                usage();
                return 0;
            }

            SubMetodoDeBilineal submetodo = (SubMetodoDeBilineal) string_to_type<int>(argv[4]);

    		switch(submetodo){

				case ORGINALEs:
			    {
			    	originBilineal(expandida, k);
			    	break;
				}
			    case EXPANDIDO:
				{
					expandidoBilineal(expandida, k);
					//Aca no se si habria que pasarle un parametro mas, diciendo de a cuantos pixeles voy a hacer la interpolacion
					break;
				}
				case PORDIAGONALES:
				{
					cout<<"bilineal por diagonales\n";
					porDiagBilineal(expandida, k);
					break;
				}
				case PORBLOQUES:
				{

					break;
				}
			}

		    grabarImagen(argv[1], expandida);


			break;
		}



		default:
            throw runtime_error("No existe ese metodo");
    }

    string s = "rm ";
    s += newFile;
    system(s.c_str());

    return  0;

}

