#include <iostream>
#include <fstream>
#include <iomanip>
#include "fileHandler.h"

FileHandler::FileHandler (string path){
	fileName = path;

}


void FileHandler::readParameters(vector<double>& paramList){
	ifstream myFile(fileName.c_str());	//Paso a c.str() porque el seniorito ifstream solo acepta cstrings!
	if (myFile.is_open()){
		myFile >> setprecision(5) >> paramList[0];
		myFile >> setprecision(5) >> paramList[1];
		myFile >> setprecision(5) >> paramList[2];
		myFile >> paramList[3];
		myFile.close();
	};

}

//NOT TESTED
vector<vector<double> > FileHandler::readLeeches(int leechCount){
	vector<vector<double> > retVector;

	ifstream myFile(fileName.c_str());
	if (myFile.is_open()){

		string dummyLine;
		getline(myFile, dummyLine);	//Ignores first line

		//Read values
		double readValue;
		for (int i = 0 ; i < leechCount*4; i= i+4)
		{
			vector<double> nuevo;
			nuevo.resize(5);
			for(int k = 0; k < 4; k++) {myFile >> readValue; nuevo[k] = readValue;}
			retVector.push_back(nuevo);
		}
	myFile.close();
	}
	return retVector;
}

//NOT TESTED
void FileHandler::saveMatrix(string path, myMatrix& aMatrix, unsigned int panelWidth){
	ofstream myFile(path.c_str(), ofstream::app);
	if (myFile.is_open()){
		for (unsigned int i = 0; i < aMatrix.getNRows(); i++)
		{
			myFile << (i / (panelWidth)) << "\t";	//ROW
			myFile << (i % panelWidth)	<< "\t"; //COL
			myFile << fixed << setprecision(5) << showpoint << aMatrix[i][0];
			myFile << endl;
		}
		myFile.close();
	}

}


void FileHandler::clearFile(string path){
	ofstream myFile(path.c_str(), ofstream::trunc);
	myFile.close();
}
