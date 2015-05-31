#include <iostream>

using namespace std;


/*
function：image reverse
*/
//#include "stdafx.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>
//#include <cv.h>
//#include <highgui.h>

#define FILE1 "img1.bmp"
#define FILE2 "img2.bmp"
#define FILE3 "img3.bmp"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "bmp.h"

int main(int argc, char *argv[])
{

    cout << "Hello world!" << endl;


  /* ======================================================================= */
  /* === EJ 1 : crear un bmp de 24 bits de 100x100 y dibujar un patron bayer */

  // creo el header de una imagen de 100x100 de 24 bits
  BMPIH* imgh1 = get_BMPIH(100,100);

  // crea una imagen bmp inicializada
  BMP* bmp1 = bmp_create(imgh1,1);

  // obtengo la data y dibujo el patron bayer
  uint8_t* data1 = bmp_get_data(bmp1);
  int i,j;
  for(j=0;j<100;j=j+2) {
    for(i=0;i<100;i=i+2) {
      data1[j*300+3*i+1] = 0xff;
      data1[j*300+3*i+3] = 0xff;
    }
    for(i=0;i<100;i=i+2) {
      data1[j*300+300+3*i+2] = 0xff;
      data1[j*300+300+3*i+4] = 0xff;
    }
  }

  // guardo la imagen
  bmp_save(FILE1, bmp1);

  // borrar bmp
  bmp_delete(bmp1);

  /* ======================================================================= */
  /* === EJ 2 : crear un bmp de 32 bits de 100x100 y fondo blanco en degrade */

  // creo el encavezado de una imagen de 640x480 de 32 bits
  BMPV5H* imgh2 = get_BMPV5H(100,100);

  // crea una imagen bmp no inicializada
  BMP* bmp2 = bmp_create(imgh2,0);

  // obtengo la data y dibujo el degrade
  uint8_t* data2 = bmp_get_data(bmp2);
  for(j=0;j<100;j++) {
    for(i=0;i<100;i++) {
      data2[j*400+i*4+0] = (uint8_t)(((float)(i+j))*(256.0/200.0));
      data2[j*400+i*4+1] = 0xff;
      data2[j*400+i*4+2] = 0xff;
      data2[j*400+i*4+3] = 0xff;
    }
  }

  // guardo la imagen
  bmp_save(FILE2, bmp2);

  // borrar bmp
  bmp_delete(bmp2);

  /* ======================================================================= */
  /* === EJ 3 : crear un bmp con el mapa de bits de bmp1 y el alpha de bmp2  */

  // Abro los dos archivos
  BMP* bmp1n = bmp_read(FILE1);
  BMP* bmp2n = bmp_read(FILE2);

  // copio la imagen con transparecia sin datos
  BMP* bmpNEW = bmp_copy(bmp2n, 0);

  // obtengo datos de new y las combino
  uint8_t* data1n = bmp_get_data(bmp1n);
  uint8_t* data2n = bmp_get_data(bmp2n);
  uint8_t* dataNEW = bmp_get_data(bmpNEW);
  for(j=0;j<100;j++) {
    for(i=0;i<100;i++) {
      dataNEW[j*400+i*4+0] = data2n[j*400+i*4+0];
      dataNEW[j*400+i*4+1] = data1n[j*300+i*3+0];
      dataNEW[j*400+i*4+2] = data1n[j*300+i*3+1];
      dataNEW[j*400+i*4+3] = data1n[j*300+i*3+2];
    }
  }

  // guardo la imagen
  bmp_save(FILE3, bmpNEW);

  // borrar bmp
  bmp_delete(bmp1n);
  bmp_delete(bmp2n);
  bmp_delete(bmpNEW);

  cout<<"hasta aca llega\n";

  return 0;




}
