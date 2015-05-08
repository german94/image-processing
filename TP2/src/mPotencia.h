#ifndef MPOTENCIA_H_INCLUDED
#define MPOTENCIA_H_INCLUDED

double norma2(Matriz<double>& v);

double normaInf(Matriz<double>& v);

void copiarMatriz(Matriz<double>& m1, Matriz<double>& m2);

bool haypocadif(Matriz<double>&v0, Matriz<double>&v1);

double ObtenerAutovalorMayor(Matriz<double> &A, Matriz<double>& v0);

vector<double> metodoPotencias(Matriz<double>& A, unsigned int alpha, Matriz<double>& P, Matriz<double>& v);







#endif // MPOTENCIA_H_INCLUDED
