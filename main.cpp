#include "Matrix.hpp"
#include <iostream>
#include <complex>


using namespace std;

int main() {
    vector<double> l2{1,1,1,
                      2,2,2,
                      3,3,3};
    Mat<double> mat2(3, 3, &l2, false);
    vector<double> l1{1.1, 2.5, 1.2,-3.44,
                      4.2, 9.6, 1.8,8.11,
                      -9.8,18.6,1.7,2.2,
                      3,4,5,6};
    Mat<double> mat1(4, 4, &l1, false);
    mat1.gauss().print();
    mat1.inverse().print();
    (mat1.inverse() * mat1).print();

    cout << mat1.det();

    return 0;
}