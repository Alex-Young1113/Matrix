#include "Matrix.hpp"
#include <iostream>


using namespace std;

int main() {
    vector<int> l1{1, 2, 1, 0, 0, 0};
    Mat<int> mat1(2, 3, &l1, false);
    vector<int> l2{1, 2, 3, 4, 5, 6};
    Mat<int> mat2(2, 3, &l2, false);
    vector<int> vv{1, 2, 1};
    Mat<int> mm = mat1 * vv;

    (dotMuilt(mat1,mat2)).print();
    return 0;
}