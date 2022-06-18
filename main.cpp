#include "Matrix.hpp"
#include <iostream>


using namespace std;

int main() {
    vector<double> l1{1,2,3,4,5,6,7,8,9};
    Mat<double> mat1(3, 3, &l1, false);
    mat1.gauss().print();
    cout<< mat1.rank();
    return 0;
}