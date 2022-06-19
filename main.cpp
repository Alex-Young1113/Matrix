#include "Matrix.hpp"
#include <iostream>


using namespace std;

int main() {
    vector<double> l1{1, 4, -1, 4, 0,
                      -7, 6, -5, 0, -14,
                      7, -5, 0, -12, 12,
                      -10, 11, 8, 7, 6,
                      7, 9, 10, -8, 3};
    Mat<double> mat1(5, 5, &l1, false);
    mat1.print();
    mat1.getCominor(2, 2).print();
    mat1.gauss().print();
    cout << mat1.rank() << "\n";
    int i{};
    mat1.gauss(i);
    cout << i << "\n";
    cout << mat1.det() << "\n" << mat1.trace();
    vector<vector<double>> vec = (vector<vector<double>>) mat1;

    return 0;
}