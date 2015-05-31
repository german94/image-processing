#define FILE1 "img1.bmp"
#define FILE2 "img2.bmp"
#define FILE3 "img3.bmp"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "bmp.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/time.h>
#include "util.h"
#include "Matriz.h"
#include <math.h>
#include <cstring>

using namespace std;

timeval start, endTime;
//////////////////////////////////////////////////////////////
void init_time()
{
      gettimeofday(&start,NULL);
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
double get_time()
{
      gettimeofday(&endTime,NULL);
      return
(1000000*(endTime.tv_sec-start.tv_sec)+(endTime.tv_usec-start.tv_usec))/1000000.0;
}
//////////////////////////////////////////////////////////////

enum Metodo {VECINO_MAS_CERCANO = 0, INTERPOLACION_BILINEAL= 1, INTERPOLACION_POR_SPLINES=2};


void usage() { cout << "./tp <input_image> <output_image> <metodo> <k>" << endl; }


int main(int argc, char *argv[]){



	if(argc != 6) {
        usage();
        return 0;
	}

	//char* imagenEntrada=argv[1];
	//char* imagenSalida=argv[2];

	Metodo metodo = (Metodo) string_to_type<int>(argv[3]);
	unsigned int k = string_to_type<unsigned int>(argv[4]);


    cout << "hello world!" << endl;


    switch(metodo){

        case VECINO_MAS_CERCANO:{

            cout<<"zoom comun\n";

            cout<<" k es "<<k<<endl;
              // Abro los dos archivos
              BMP* bmp1n = bmp_read(argv[1]);
              BMP* bmp2n = bmp_read(argv[2]);

              // copio la imagen con transparecia sin datos
              BMP* bmpNEW = bmp_copy(bmp2n, 0);

              // obtengo datos de new y las combino
              uint8_t* data1n = bmp_get_data(bmp1n);
              uint8_t* data2n = bmp_get_data(bmp2n);
              uint8_t* dataNEW = bmp_get_data(bmpNEW);
              for(int j=0;j<100;j++) {
                for(int i=0;i<100;i++) {
                  dataNEW[j*400+i*4+0] = data2n[j*400+i*4+0];
                  dataNEW[j*400+i*4+1] = data1n[j*300+i*3+0];
                  dataNEW[j*400+i*4+2] = data1n[j*300+i*3+1];
                  dataNEW[j*400+i*4+3] = data1n[j*300+i*3+2];
                }
              }

              // guardo la imagen
              bmp_save(argv[5], bmpNEW);

              // borrar bmp
              bmp_delete(bmp1n);
              bmp_delete(bmp2n);
              bmp_delete(bmpNEW);

            cout<<"termino metodo\n";

            break;
        }

        case INTERPOLACION_BILINEAL:{

            //metodo




            break;
        }

        case INTERPOLACION_POR_SPLINES:{

            //metodo




            break;
        }

        default:
            throw runtime_error("No existe ese metodo");
    }


  return 0;

}
