#include <string>
#include <fstream>
#include <vector>
#include "myMatrix.h"

using namespace std;


class FileHandler {
public:

	//Constructor
	FileHandler (string path);

	//Reads leech positions and returns them as a vector of doubles ([x,y,x2,y2,x3,y3])
	vector<double> readLeeches(int leechCount);

	//Reads the general parameters and stores them in a vector of doubles
	void readParameters(vector<double>& paramList);

	//Saves the matrix in a file NOT TESTED
	void saveMatrix (string path, myMatrix& aMatrix, unsigned int panelWidth);

	//Clears the file
	void clearFile(string path);

private:
	string fileName;

};
