#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include <unordered_map>
class Point{
public:
    int x;
    int y;
    Point(int x, int y){
        this->x = x;
        this->y = y;
    }
};

template <typename T>
class Mat{
public:
    int col;
    int row;
    int step;
    int numOfElements;
    std::unordered_map<Point,T> *Map;
    T *pData;

    Mat();
    Mat(int, int);
    bool isSparse();
    void set(int, int, T);
    void toDense();
};

#endif //MATRIX_MATRIX_H
