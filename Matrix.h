#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include <unordered_map>
template <class T>
class Mat{
public:
    int row{};
    int col{};
    int step{};
    int numOfElements{};
    std::unordered_map<int,int> Map;
    T *pData;

    Mat(int, int);
    T getIndex(int x, int y);
    bool isSparse();
    void set(int, int, T);
    void toDense();
    T get(int, int);
};

#endif //MATRIX_MATRIX_H
