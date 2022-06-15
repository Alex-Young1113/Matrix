#include "Matrix.h"
#include "Matrix.cpp"
#include <iostream>
using namespace std;

int main() {
    Mat<int> mat(300, 300);
    for(int i = 0; i < 50; i ++){
        for(int j = 0; j < 50; j ++){
            mat.set(i, j, i + j);
        }
    }
    Mat<int> mat2 = mat;
    cout << mat.pData.use_count();
    return 0;
}