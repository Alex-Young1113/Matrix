#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include <unordered_map>
#include <memory>
#include <cstddef>
template <class T>
class Mat{
public:
    long long row;
    long long col;
    long long step;
    long long numOfElements;
    std::unordered_map<int,T> map;
    std::shared_ptr<T[]> pData;

    Mat(int, int);
    T getIndex(int x, int y);
    bool isSparse();
    void set(int, int, T);
    void toDense();
    T get(int, int);
};

#endif //MATRIX_MATRIX_H