# Matrix
1) It supports all matrix sizes, from small fixed-size matrices to arbitrarily large dense matrices, and even sparse matrices (Add: try to use efficient ways to store the sparse matrices). (10 points)
2) It supports all standard numeric types, including std::complex, integers, and is easily extensible to custom numeric types. (10 points)
3) It supports matrix and vector arithmetic, including addition, subtraction, scalar multiplication, scalar division, transposition, conjugation, element-wise multiplication, matrix-matrix multiplication, matrix-vector multiplication, dot product and cross product. (20 points)
4) It supports basic arithmetic reduction operations, including finding the maximum value, finding the minimum value, summing all items, calculating the average value (all supporting axis-specific and all items). (10 points)
5) It supports computing eigenvalues and eigenvectors, calculating traces, computing inverse and computing determinant. (10 points)
6) It supports the operations of reshape and slicing. (10 points)
7) It supports convolutional operations of two matrices. (10 points)
8) It supports to transfer the matrix from OpenCV to the matrix of this library and vice versa. (10 points)
9) It should process likely exceptions as much as possible. (10 points)

目前完成1， 2， 9完成一半，需要你们自己在用到新exception的时候到Exception.h里添加。

y:4(finished) 7(finished) 5(determinant,not yet)