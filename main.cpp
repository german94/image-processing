#include "myMatrix.h"
#include "fileHandler.h"
#include <iostream>
#include <vector>


void loadValues(myMatrix& aMatrix, unsigned int panelWidth);
void loadValuesBand(myMatrix& aMatrix, unsigned int panelWidth);

int main(int argc, char* argv[]) {
	if(argc == 1){
		bool test = argv[0] == argv[0];
		if(test == true){
			test = false;
		}
	}

	FileHandler myFile("./test1.in");
	vector<double> params;
	params.resize(4);
	myFile.readParameters(params);

	double width = params[0];
	double heigth = params[1];
	double discrInterval = params[2];
	//unsigned int nLeeches = params[3];

	unsigned int discrHeigth = (unsigned int)heigth/discrInterval;
	unsigned int discrWidth = (unsigned int)width/discrInterval;
	unsigned int matrixSize = discrHeigth*discrWidth;

	//return 0;
	myMatrix aa(matrixSize,2*discrWidth+1);
	loadValuesBand(aa, discrWidth);
	
	//myMatrix aa(matrixSize,matrixSize);
	//loadValues(aa, discrWidth);
	//aa.gaussianElimination();
	//aa[0][0] = 1.32423;
	//aa.swapRows(0,9);
	//aa.swapRows(9,5);
	//cout << aa[0][9] << endl;
	cout << aa.getNRows() << " X " << aa.getNCols() << endl;
	cout << aa.toString();
}


void loadValues(myMatrix& aMatrix, unsigned int panelWidth){
	for(unsigned int currentSpot = 0; currentSpot < aMatrix.getNRows(); currentSpot++){
		if(currentSpot < panelWidth || currentSpot > (aMatrix.getNRows()-panelWidth) || (currentSpot+1) % panelWidth == 0 || (currentSpot+1) % panelWidth == 1){
			//Test values
			aMatrix[currentSpot][currentSpot] = 1;
		}else{
			//Test values
			aMatrix[currentSpot][currentSpot] = 4;
			aMatrix[currentSpot][currentSpot-1] = 2;
			aMatrix[currentSpot][currentSpot+1] = 3;
			aMatrix[currentSpot][currentSpot+panelWidth] = 4;
			aMatrix[currentSpot][currentSpot-panelWidth] = 5;
		}

	}
}

//Solucion parcial:
void loadValuesBand(myMatrix& aMatrix, unsigned int panelWidth){
	//unsigned int middleIndex =
	for(unsigned int currentSpot = 0; currentSpot < aMatrix.getNRows(); currentSpot++){
		if(currentSpot < panelWidth || currentSpot > (aMatrix.getNRows()-panelWidth) || (currentSpot+1) % panelWidth == 0 || (currentSpot+1) % panelWidth == 1){
			aMatrix[currentSpot][panelWidth] = 1;
		}else{
			aMatrix[currentSpot][panelWidth] = 4;
			aMatrix[currentSpot][panelWidth-1] = 2;
			aMatrix[currentSpot][panelWidth+1] = 3;
			aMatrix[currentSpot][panelWidth + panelWidth] = 4;
			aMatrix[currentSpot][0] = 5;
		}
	}
}
