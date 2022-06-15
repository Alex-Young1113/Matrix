#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include <unordered_map>

class Mat{
public:
    int row;
    int col;
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
