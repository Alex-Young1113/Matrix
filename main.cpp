#include "Matrix.hpp"
#include <iostream>
#include <complex>


using namespace std;

int main() {
    Mat<double> mat = ranMatGen(5, 5, -5, 5);
    mat.print(false);
    mat.gauss().print(false);
    cout << "rank of the mat: " << mat.rank() << "\n";
    cout << "max of the mat: " << mat.max() << "\n";
    cout << "min of the mat: " << mat.min() << "\n";
    cout << "avg of the mat: " << mat.avg() << "\n";


    return 0;
}