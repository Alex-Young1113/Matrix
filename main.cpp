#include "Matrix.h"
#include <iostream>
using namespace std;

int main() {
    Mat mat(3, 3);
    mat.set(1,1,3);
    cout << mat.get(1, 1) << endl;
    mat.set(1,2,4);
    mat.set(0,0, 1);
    mat.set(2, 2, 5);
    cout << mat.get(2, 2) << endl << mat.get(1, 0);

    return 0;
}
