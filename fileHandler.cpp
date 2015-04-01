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
vector<double> FileHandler::readLeeches(int leechCount){
	vector<double> retVector;
	retVector.resize(leechCount*4);

	ifstream myFile(fileName.c_str());
	if (myFile.is_open()){

		string dummyLine;
		getline(myFile, dummyLine);	//Ignores first line


		//Read values
		double readValue;
		for (int i = 0; i < leechCount*4; i++)
		{
			myFile >> readValue;
			retVector[i] = readValue;
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
