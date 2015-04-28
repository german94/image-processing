#ifndef UTIL_H
#define UTIL_H

#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <vector>

using namespace std;


//Para no tener que usar stod
template<class T>
T string_to_type( const std::string& s )
{
   std::istringstream i(s);
   T x;
   if (!(i >> x))
		throw runtime_error("Se esperaba un tipo diferente de dato");
   return x;
}

template<class T>
string type_to_string( const T& s )
{
   std::stringstream ss;
   if (!(ss << s))
		throw runtime_error("Se esperaba imprimir otro tipo de dato");
   return ss.str();
}

vector<string> separar(const string& linea) {
    vector<string> res;

    string token;
    stringstream ss(linea);

    while (getline(ss, token, ' ')) {
        if (!token.empty()) {
            res.push_back(token);
        }
    }

    return res;
}

vector<string> separarComa(const string& linea) {
    vector<string> res;

    string token;
    stringstream ss(linea);

    while (getline(ss, token, ',')) {
        if (!token.empty()) {
            res.push_back(token);
        }
    }

    return res;
}

#endif /* end of include guard: UTIL_H */
