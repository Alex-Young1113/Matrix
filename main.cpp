#include "Matrix.hpp"
#include <iostream>
#include <complex>


using namespace std;

int main() {
    Mat<double> mat = ranMatGen(5, 5, -5, 5);
    mat.print(false);
    mat.gauss().print(false);
    return 0;
}