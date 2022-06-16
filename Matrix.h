#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include <unordered_map>
#include <memory>
#include <cstddef>
#include <vector>

template <class T> class data{

};
template <class T> class Mat{
    std::shared_ptr<std::unordered_map<int,T>> pMap; // hashmap to store elements in sparse matrix
    std::shared_ptr<T[]> pData; // array to store elements in dense matrix
    T getIndex(int x, int y); // return the offset of Mat[x][y]
public:
    long long row; // number of rows
    long long col; // number of columns
    long long step; // used for computing the index of next row
    bool isSparse;
    Mat(int row, int col, bool isSparse = false); // construct an all zero matrix with x rows and y columns
    void toDense(); // convert mat to dense matrix
    void toSparse(); // convert mat to dense matrix
    void set(int x, int y, T val); // set Mat[x][y] to val
    T get(int, int); // return Mat[x][y]
};
#endif //MATRIX_MATRIX_H