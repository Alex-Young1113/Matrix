#include "Matrix.h"
#include "Matrix.cpp"
#include <iostream>


using namespace std;

int main() {
    Mat<double> mat(20, 20, false);
    //mat.set(33, 33, 6.5);
    Mat<double> &mat2 = mat;
    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            mat.set(i, j, i + j + 0.5);
        }
    }
    Mat<double> mat3 = mat.clone();
    mat.print(5);
    mat[90][2] = 1;
    mat.print(5);
    return 0;
}