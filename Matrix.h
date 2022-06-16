#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include <unordered_map>
#include <memory>
#include <cstddef>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Exception.h"

template <class T> class Mat{
    std::shared_ptr<std::unordered_map<int,T>> pMap; // hashmap to store elements in sparse matrix
    std::shared_ptr<T[]> pData; // array to store elements in dense matrix
    T getIndex(int x, int y); // return the offset of Mat[x][y]
public:
    long long row = 0; // number of rows
    long long col = 0; // number of columns
    long long step = 0; // used for computing the index of next row
    bool isSparse = 0; // 0 for dense matrix and 1 for sparse matrix
    Mat(){};
    Mat(int row, int col, bool isSparse = false); // construct an all zero matrix with x rows and y columns
    void toDense(); // convert mat to dense matrix
    void toSparse(); // convert mat to dense matrix
    void set(int x, int y, T val); // set Mat[x][y] to val
    T get(int, int); // return Mat[x][y]
    void print(int width = 5); // print the matrix with specified width for each element
    Mat<T> clone(); // deep copy
};
#endif //MATRIX_MATRIX_H