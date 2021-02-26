#include "simplematrix.h"

template <typename T>
SimpleMatrix<T>::SimpleMatrix(){ }

template <typename T>
SimpleMatrix<T>::SimpleMatrix(int _n, int _m){
    n = _n;
    m = _m;
    matrix = new T*[_n];
    for(int i=0;i<_n;i++){
        matrix[i] = new T[_m];
        for(int j=0;j<_m;j++){
            matrix[i][j]=0;
        }
    }
}

template <typename T>
SimpleMatrix<T>::~SimpleMatrix(){
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            delete matrix[i][j];
        }
        delete matrix[i];
    }
    delete matrix;
}

template <typename T>
int SimpleMatrix<T>::lines(){
    return n;
}

template <typename T>
int SimpleMatrix<T>::columns(){
    return m;
}

template <typename T>
T SimpleMatrix<T>::value(int i, int j){
    return matrix[i][j];
}

