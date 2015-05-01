#ifndef MATRIZ_H
#define MATRIZ_H
#include <vector>


template<class T>
class Matriz
{
    public:
        Matriz(int filas, int cols);
        ~Matriz();
        std::vector<T >&  operator[](int i);
        Matriz<T> operator*(Matriz<T>& B);
        Matriz<T> operator*(T l);
        Matriz<T> operator-(Matriz<T>& B);
        Matriz<T> operator+(Matriz<T>& B);
        bool operator==(Matriz<T>& B);
        Matriz<T> traspuesta();
        int filas();
        int columnas();
        void display();
    protected:
    private:
        std::vector<std::vector <T> > _matrix;
        int _filas;
        int _columnas;

};


template<class T>
Matriz<T>::~Matriz(){
}

template<class T>
Matriz<T>::Matriz(int filas, int cols){
    _filas = filas;
    _columnas = cols;
    _matrix.resize(filas);
    for (unsigned i=0; i<_matrix.size(); i++) {
        _matrix[i].resize(cols,0);
    }
}

template<class T>
int Matriz<T>::filas(){
    return _filas;
}

template<class T>
int Matriz<T>::columnas(){
    return _columnas;
}


template<class T>
std::vector<T >& Matriz<T>::operator[](int i){
    return this->_matrix[i];
}

template<class T>
Matriz<T> Matriz<T>::operator*(Matriz<T>& B){
    // A(this): n x m, B: m x k
    int n = this->filas();
    int m = this->columnas();
    int k = B.columnas();

    Matriz<T> result(n,k);
    for(int i = 0; i < n; i++){
        for(int j = 0; j< k ;j++){
            T sum = _matrix[i][0] * B._matrix[0][j];
            for(int r = 1; r < m; r++){
               sum += this->_matrix[i][r] * B._matrix[r][j];
            }
            result[i][j] = sum;
        }
    }
    return result;

}

template<class T>
Matriz<T> Matriz<T>::operator+(Matriz<T>& B){
    // A(this): n x m, B: m x k
    int n = this->filas();
    int m = this->columnas();
    int k = B.columnas();

    Matriz<T> result(n,k);
    for(int i = 0; i < n; i++){
        for(int j = 0; j< m ;j++){
            result[i][j] = this->_matrix[i][j] + B._matrix[i][j];
        }
    }
    return result;
}

template<class T>
Matriz<T> Matriz<T>::operator-(Matriz<T>& B){
    // A(this): n x m, B: m x k
    int n = this->filas();
    int m = this->columnas();
    int k = B.columnas();

    Matriz<T> result(n,k);
    for(int i = 0; i < n; i++){
        for(int j = 0; j< m ;j++){
            result[i][j] = this->_matrix[i][j] - B._matrix[i][j];
        }
    }
    return result;

}

template<class T>
Matriz<T> Matriz<T>::operator*(T l){
    // A(this): n x m, B: m x k
    int n = this->filas();
    int m = this->columnas();

    Matriz<T> result(n,m);
    for(int i = 0; i < n; i++){
        for(int j = 0; j< m ;j++){
            result[i][j] =  this->_matrix[i][j] * l ;
        }
    }
    return result;
}




template<class T>
bool Matriz<T>::operator==(Matriz<T>& B){
    // A(this): n x m, B: m x k
    int n = this->filas();
    int m = this->columnas();
    bool iguales = true;
    Matriz<T> result(n,m);
    int i = 0;
    int j = 0;
    while(i<n && iguales){
        while(j<m && iguales){
            iguales = (this->_matrix[i][j] == B._matrix[i][j] );
            j++;
        }
        i++;
    }
    return iguales;
}

template<class T>
Matriz<T> Matriz<T>::traspuesta(){
    int n = this->filas();
    int m = this->columnas();

    Matriz<T> result(n,m);
    for(int i = 0; i < n; i++){
        for(int j = 0; j< m ;j++){
            result[i][j] =  this->_matrix[j][i] ;
        }
    }
    return result;
}



#endif // MATRIZ_H
