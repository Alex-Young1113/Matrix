#include "Matrix.hpp"
#include <iostream>


using namespace std;

int main() {
    vector<int> l1{1, 2, 1, 0, 0, 0, -1, -2, -1};
    Mat<int> mat1(3, 3, &l1, false);
    vector<int> l2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    Mat<int> mat2(5, 4, &l2, false);
    cout << "mat1" << endl;
    mat1.print();
    cout << "mat2" << endl;

    mat2.print();

    Mat<int> mat3 = mat2.transpose();
    cout << "mat3:" << endl;
    mat3.print();
    Mat<int> mat4 = 4.7 * mat2;
    cout << "mat4:" << endl;
    mat4.print();
    return 0;
}