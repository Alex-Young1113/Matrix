#include "Matrix.hpp"
#include <iostream>


using namespace std;

int main() {
    int l1[9]{1, 2, 1, 0, 0, 0, -1, -2, -1};
    Mat<int> mat1(3, 3, l1, 9);
    int l2[16]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    Mat<int> mat2(3, 3, l2, 9);
    mat1.print();
    mat2.print();

    Mat<int> add = mat1 + mat2;
    Mat<int> scalar = mat1 * (3.123);

    add.print();
    scalar.print();
    return 0;
}

