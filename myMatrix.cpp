#include "myMatrix.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>

myMatrix::myMatrix(unsigned int nRows, unsigned int nCols){
	this->internalGrid = new double* [nRows]();
	for(unsigned int i = 0; i < nRows; i++){
		this->internalGrid[i] = new double [nCols]();
	}
	this->nCols = nCols;
	this->nRows = nRows;
}

myMatrix::myMatrix(){
	this->nCols = 0;
	this->nRows = 0;
}

myMatrix::~myMatrix(){
	for(unsigned int i = 0; i < this->nRows; i++){
		delete[] this->internalGrid[i];
	}
	delete[] this->internalGrid;
}

unsigned int myMatrix::getNCols(){
	return this->nCols;
}

unsigned int myMatrix::getNRows(){
	return this->nRows;
}


void myMatrix::swapRows(unsigned int firstRow, unsigned int secondRow){
	if(firstRow < this->nRows && secondRow < this->nRows){
		double * auxiliaryRow = this->internalGrid[firstRow];
		this->internalGrid[firstRow] = this->internalGrid[secondRow];
		this->internalGrid[secondRow] = auxiliaryRow;
	}
}

void myMatrix::substractRow(double coefficient, unsigned int firstRow, unsigned int secondRow, unsigned int fromColumn){
	for(unsigned int i = fromColumn; i < this->nCols; i++){
		this->internalGrid[secondRow][i] = this->internalGrid[secondRow][i] - (coefficient*this->internalGrid[firstRow][i]);
	}
}

void myMatrix::substractRowBand(double coefficient, unsigned int firstRow, unsigned int secondRow){
	unsigned int padding = secondRow-firstRow;
	for (unsigned int i = 0; i+padding < this->nCols; i++){
		this->internalGrid[secondRow][i] = this->internalGrid[secondRow][i] - (coefficient*this->internalGrid[firstRow][i+padding]);
	}
}

void myMatrix::zeroesLeft(unsigned int aRow, unsigned int toColumn){
	for(unsigned int i = 0; i < toColumn; i++){
		this->internalGrid[aRow][i] = 0;
	}
}

void myMatrix::gaussianElimination(){
	//cout << "START" << endl;
	for(unsigned int j = 0; j < this->nCols; j++){
		for (unsigned int i = j+1; i < this->nRows; i++)
		{
			double coefficient = (internalGrid[i][j]/internalGrid[j][j]);
			//cout << "F" << i << "=" << "F" << i << "-" << coefficient << "*F" << j << endl;
			this->substractRow(coefficient, j, i, j+1);
			//cout << this->toString() << endl;
		}
		this->zeroesLeft(j,j);


	}
}

void myMatrix::gaussianEliminationBand(unsigned int panelWidth){
	//Ya no puedo confiar en nCols porque ahora el valor es menor
	//Como la matriz verdadera siempre es cuadrada me es indiferente usar nRows en vez de nCols
	for(unsigned int j = 0; j < this->nRows; j++){
		for(unsigned int i = j+1; i < this->nRows && i < j+panelWidth+1; i++){	//Ahi aprovecho la ventaja de que si i >= j+panelWidth+1 entonces el coeficiente da (teoricamente) 0
			double mainDiagonalValue = internalGrid[j][panelWidth];	//panelWidth == la columna del medio de nuestra matriz == la diagonal principal
			//panelWidth == medio; (i-j) == 1,2,3..etc => medio-1, medio-2... medio-etc
			//Es decir que a medida que bajo en la matriz (incrementa i), el valor que tengo que utilizar se encuentra 1 a la izquierda del anterior
			//Por eso hago la resta
			double coefficient = internalGrid[i][panelWidth-(i-j)] / mainDiagonalValue;;
			this->substractRowBand(coefficient, j, i);
		}
		this->zeroesLeft(j, panelWidth);
	}
}

double* myMatrix::operator[](int i){
	return this->internalGrid[i];
}


string myMatrix::toString(){
	stringstream strOutput;
	for(unsigned int i = 0; i < this->nRows; i++){
		for(unsigned int j = 0; j < this->nCols; j++){
			strOutput << "|" << this->internalGrid[i][j] << "|";
		}
		strOutput << endl;
	}
	return strOutput.str();
}


string myMatrix::toString(char notZero){
	stringstream strOutput;
	for(unsigned int i = 0; i < this->nRows; i++){
		for(unsigned int j = 0; j < this->nCols; j++){
			strOutput << "|";
			if(abs(this->internalGrid[i][j]) > 0.0000001){
				strOutput << notZero;
			}else{
				strOutput << 0;
			}
			strOutput << "|";
		}
		strOutput << endl;
	}
	return strOutput.str();
}
