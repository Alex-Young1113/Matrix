#include "Matrix.hpp"
#include <iostream>


using namespace std;

int main() {
    //asdasd
    vector<int> l1{1, 2, 1, 0, 0, 0};
    Mat<int> mat1(3, 2, &l1, false);
    vector<int> l2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    Mat<int> mat2(5, 4, &l2, false);
    std::vector<int> vv{1, 1, 1};
    Mat<int> mat3 = vv * mat1;

    mat3.print();

    return 0;
}