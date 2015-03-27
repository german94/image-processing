#ifndef MYMATRIX_H
#define MYMATRIX_H

#include <string>

using namespace std;

class myMatrix {
	public:

		//Constructors:
		myMatrix(unsigned int nCols, unsigned int nRows);
		myMatrix();

		//Destructor:
		~myMatrix();

		//Swap 2 rows in the internal grid
		//Mathematically: row1<=>row2
		void swapRows(unsigned int firstRow, unsigned int secondRow);

		//Similar to the mathematic operation: row2 => row2 - (X * row1)
		//With the additional option to specify the column from where to start substracting
		//E.G: if fromColumn = 5 then the first 6 columns of secondRow will be unchanged (fromColumn = 0 => normal operation)
		void substractRow(double coefficient, unsigned int firstRow, unsigned int secondRow, unsigned int fromColumn);

		//Use in conjunction with gaussianElimination and substractRow:
		//After a row substraction we KNOW we have zeroes up to X column
		//So, instead of substracting and possibly having 0.000000000000001 we assign the zeroes manually
		void zeroesLeft(unsigned int aRow, unsigned int toColumn);

		//Gaussian elimination for normal matrixes
		void gaussianElimination();

		//Lets the user access the internal matrix structure with the standart operator
		//E.G: myMatrix[i][j]
		//WARNING: Parameters i and j are NOT validated, may cause a segmentation fault
		double * operator [](int i);

		//Gets number of columns
		unsigned int getNCols();

		//Gets number of rows
		unsigned int getNRows();

		//Represents the matrix as a string
		string toString();

		//Represents the matrix as a string.
		//Any value different than 0 is represented by the special character **notZero**
		string toString(char notZero);

	private:
		//The actual matrix is represented by an array of arrays of doubles
		double** internalGrid;

		//Information about the matrix's size
		unsigned int nCols;
		unsigned int nRows;

};

#endif /* MYMATRIX_H */
