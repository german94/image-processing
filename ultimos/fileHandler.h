#include <string>
#include <fstream>
#include <vector>

using namespace std;


class FileHandler {
public:

	//Constructor
	FileHandler (string path);

	//Reads leech positions and returns them as a vector of doubles ([x,y,x2,y2,x3,y3])
	vector<vector<double> > readLeeches(int leechCount);

	//Reads the general parameters and stores them in a vector of doubles
	void readParameters(vector<double>& paramList);

	//Clears the file
	void clearFile(string path);

private:
	string fileName;

};
