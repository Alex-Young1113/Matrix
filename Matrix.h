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

class Mat{
public:
    int col;
    int row;
    int step;
    int numOfElements;
    std::unordered_map<int,int> Map;
    int *pData;

    Mat(int, int);
    int getIndex(int x, int y);
    bool isSparse();
    void set(int, int, int);
    void toDense();
    int get(int, int);
};

#endif //MATRIX_MATRIX_H
