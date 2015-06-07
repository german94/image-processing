#include "splines.h"
#include "bilineal.h"
#include "vecino.h"
#include "aux.h"

using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 5) { //archivo_input ; k; metodo
		usage();
		return 0;
	}

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

	preprocesarImagen(newFile, k, expandida);

	cout<<expandida.size()<<endl;

    switch(metodo){

    	case VECINO:
    	{

		    int dato_f, dato_c;
		    for(int i = 0; i < expandida.size(); i++)
		    {
		    	dato_c = 0;
		    	for(int j = 0; j < expandida[i].size() ; j++)
		    	{

		    		if(expandida[i][j] != -1) {dato_f = i; dato_c = j;}
		    		else
		    		{
		    			if(j == dato_c + k + 1) {dato_c = j;}
		    			expandida[i][j] = mas_cercano(k, dato_c, dato_f, i, j, expandida);
		    		}
		    	}
		    }

	        grabarImagen(argv[1], expandida);

	        break;
		}

		case SPLINE:
    	{
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

		case BILINEAL:
    	{
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

			ofstream salida("salida1.csv",std::ofstream::out);// genero este nuevo archivo de salda con el formato pero el vector tiene valores cero

	        for(int i = 0; i < expandida.size(); i++)
	        {
	        	for(int j = 0; j < expandida[i].size(); j++)
	        	{
	        		if(j != expandida[i].size() - 1) {salida << expandida[i][j] << " ";}
	        		else {salida << expandida[i][j];}
	        	}
	        	if(i != expandida.size() -1) {salida << endl;}
	        }
	        salida.close();
	        
	        grabarImagen(argv[1]);

	        break;
		}

		case BILINEALBIS:
		{
			//Primero interpolo por filas, de a k filas
			int dato_c;
		    for(int i = 0; i < expandida.size(); i = i+k+1)
		    {
		    	dato_c = 0;
		    	for(int j = 0; j < expandida[i].size() ; j++)
		    	{
		    		if(expandida[i][j] != -1 && i != expandida.size() - 1) {dato_c = j;}
		    		else
		    		{
		    			if(j == dato_c + k + 1 && j != expandida[i].size() -1) {dato_c = j;}
		    			expandida[i][j] = calculo_bilineal_por_filas(k, dato_c, i, j, expandida);
		    		}
		    	}
		    }

		   	//Ahora interpolo por columnas, de a una fila
			int dato_f;
		    for(int j = 0; j < expandida[0].size(); j++)
		    {
		    	dato_f = 0;
		    	for(int i = 0; i < expandida.size() ; i++)
		    	{
		    		if(expandida[i][j] != -1 && j != expandida[0].size() - 1) {dato_f = i;}
		    		else
		    		{
		    			if(i == dato_f + k + 1 && i != expandida.size() -1) {dato_f = i;}
		    			expandida[i][j] = calculo_bilineal_por_columnas(k, dato_f, i, j, expandida);
		    		}
		    	}
		    }

		    grabarImagen(argv[1], expandida);


			break;
		}

		default:
            throw runtime_error("No existe ese metodo");
    }
    return  0;

}

