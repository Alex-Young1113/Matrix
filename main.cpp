#include "Matrix.h"
#include "Matrix.cpp"
#include <iostream>
using namespace std;

int main() {
    Mat<double> mat(60, 60, true);
    mat.set(3, 3, 6.5);
    Mat<double> &mat2 = mat;
    for(int i = 1; i < 50; i ++){
        for(int j = 1; j < 50; j ++){
            mat.set(i, j, i + j + 0.5);
        }
    }
    cout << mat2.get(3, 3) << endl << mat.isSparse << endl;
    mat.set(3, 3, 1919.81);
    cout << mat2.get(3, 3) << endl;
    mat.toDense();
    cout << mat2.get(3, 3) << endl << mat.isSparse << endl;
    mat.toSparse();
    cout << mat2.get(3, 3) << endl << mat.isSparse << endl;
    return 0;
}