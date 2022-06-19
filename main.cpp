#include "Matrix.hpp"
#include <iostream>
#include <complex>


using namespace std;

int main() {
/*Use random matrix generator to test gauss elimination and inverse matrix calculation*/
    for (int i = 0; i < 4; ++i) {
        Mat<double> ranMat = ranMatGen(5, 5);
        ranMat.print();
        ranMat.gauss().print();
        ranMat.inverse().print();
        (ranMat * ranMat.inverse()).print();
    }

    Mat<double> ranMat = ranMatGen(5, 5);
    ranMat.print(false);
    (ranMat * ranMat).print(false);

    return 0;
}