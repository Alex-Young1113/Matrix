#include "Matrix.hpp"
#include <iostream>


using namespace std;

int main() {
    int l1[9]{1, 2, 1, 0, 0, 0, -1, -2, -1};
    Mat<int> mat1(3, 3, l1, 9);
    int l2[16]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    Mat<int> mat2(4, 4, l2, 16);
    cout << "mat1" << endl;
    mat1.print();
    cout << "mat2" << endl;

    mat2.print();

    Mat<int> tmp = mat2.transpose();
    cout << "tmp:" << endl;
    tmp.print();
    return 0;
}