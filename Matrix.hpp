#ifndef MATRIX_MATRIX_HPP
#define MATRIX_MATRIX_HPP

#include <unordered_map>
#include <memory>
#include <cstddef>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Exception.h"

template<class T>
class Mat {
    std::shared_ptr<std::unordered_map<int, T>> pMap; // hashmap to store elements in sparse matrix
    std::shared_ptr<T[]> pData; // array to store elements in dense matrix
    T getIndex(int x, int y); // return the offset of Mat[x][y]
public:
    long long row = 0; // number of rows
    long long col = 0; // number of columns
    long long step = 0; // used for computing the index of next row
    bool isSparse = false; // 0 for dense matrix and 1 for sparse matrix
    Mat() = default;;

    Mat(int row, int col, T *list, int n);

    Mat(int row, int col, bool isSparse = false); // construct an all zero matrix with x rows and y columns
    void toDense(); // convert mat to dense matrix
    void toSparse(); // convert mat to dense matrix
    void set(int x, int y, T val); // set Mat[x][y] to val
    T get(int, int); // return Mat[x][y]

    T max();

    Mat<T> transpose();

    Mat<T> conv(Mat<T> &kernel);

    template<class T2>
    friend Mat<T2> operator+(Mat<T2> &lhs, Mat<T2> &rhs);

    template<class T2>
    friend Mat<T2> operator-(Mat<T2> &lhs, Mat<T2> &rhs);

    template<class T2>
    friend Mat<T2> operator*(double lhs, Mat<T2> &rhs);

    template<class T2>
    friend Mat<T2> operator*(Mat<T2> &lhs, double rhs);

    T min();

    T sum();

    T avg();

    void print(int width = 5); // print the matrix with specified width for each element
    Mat<T> clone(); // deep copy
};

template<class T>
Mat<T>::Mat(int row, int col, bool isSparse) {
    this->row = row;
    this->col = col;
    this->step = col;
    this->isSparse = isSparse;
    if (this->isSparse) {
        this->pMap = std::shared_ptr<std::unordered_map<int, T>>(new std::unordered_map<int, T>);
    } else {
        this->pData = std::shared_ptr<T[]>(new T[this->row * this->col]);
        for (int i = 0; i < this->row; i++) {
            for (int j = 0; j < this->col; j++) {
                this->pData[getIndex(i, j)] = 0;
            }
        }
    }
}

template<class T>
void Mat<T>::set(int x, int y, T val) {
    if (x > this->row || y > this->col) {
        std::cerr << "index " << "(" << x << "," << y << ") " << "out of range " << "1-" << this->row << "," << "1-"
                  << this->col << std::endl;
        throw InvalidCoordinatesException("Index out of range");
    }
    x--;
    y--;
    if (this->isSparse) {
        (*this->pMap)[this->getIndex(x, y)] = val;
    } else {
        this->pData[this->getIndex(x, y)] = val;
    }
}

template<class T>
void Mat<T>::toDense() {
    if (this->isSparse) {
        this->isSparse = false;
        this->pData = std::shared_ptr<T[]>(new T[this->row * this->col]);
        for (int i = 0; i < this->row; i++) {
            for (int j = 0; j < this->col; j++) {
                this->pData[getIndex(i, j)] = 0;
            }
        }
        for (auto kv: (*this->pMap)) {
            this->pData[kv.first] = kv.second;
        }
        this->pMap = nullptr;
    }
}

template<class T>
void Mat<T>::toSparse() {
    if (!this->isSparse) {
        this->isSparse = true;
        this->pMap = std::shared_ptr<std::unordered_map<int, T>>(new std::unordered_map<int, T>);
        for (int i = 0; i < this->row; i++) {
            for (int j = 0; j < this->col; j++) {
                if (this->pData[getIndex(i, j)] != 0) {
                    (*this->pMap)[getIndex(i, j)] = this->pData[getIndex(i, j)];
                }
            }
        }
        this->pData = nullptr;
    }
}

template<class T>
T Mat<T>::getIndex(int x, int y) {
    return x * this->step + y;
}

template<class T>
T Mat<T>::get(int x, int y) {
    if (x > this->row || y > this->col) {
        std::cerr << "index " << "(" << x << "," << y << ") " << "out of range " << "1-" << this->row << "," << "1-"
                  << this->col << std::endl;
        throw InvalidCoordinatesException("Index out of range");
    }
    x--;
    y--;
    if (this->isSparse) {
        if ((*this->pMap).find(this->getIndex(x, y)) == (*this->pMap).end()) {
            return 0;
        } else {
            return (*this->pMap)[this->getIndex(x, y)];
        }
    } else {
        return this->pData[this->getIndex(x, y)];
    }
}

template<class T>
void Mat<T>::print(int w) {
    for (int i = 1; i <= w; i++) {
        std::cout << " ";
    }
    for (int i = 1; i <= this->col; i++) {
        std::cout << std::setw(w) << i << " ";
    }
    std::cout << std::endl;
    for (int i = 1; i <= w; i++) {
        std::cout << " ";
    }
    for (int i = 1; i <= (double) (w + 1) * (double) (this->col); i++) {
        std::cout << "-";
    }
    std::cout << std::endl;
    for (int i = 1; i <= this->col; i++) {
        std::cout << std::setw(w - 1) << std::left << i << "|" << std::right;
        for (int j = 1; j <= this->row; j++) {
            std::cout << std::setw(w) << get(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

template<class T>
Mat<T> Mat<T>::clone() {
    Mat<T> rt(this->row, this->col, this->isSparse);
    rt.step = this->step;
    if (this->isSparse) {
        for (auto kv: (*this->pMap)) {
            (*rt.pMap)[kv.first] = kv.second;
        }
    } else {
        for (int i = 1; i <= this->row; i++) {
            for (int j = 1; j <= this->col; j++) {
                rt.set(i, j, this->get(i, j));
            }
        }
    }
    return rt;
}

template<class T>
/* return the maximum element of the matrix */
T Mat<T>::max() {
    if (this->isSparse) {
        T max = 0;
        for (auto kv: (*this->pMap)) {
            if (kv.second > max) {
                max = kv.second;
            }
        }
        return max;
    } else {
        T max = 0;
        for (int i = 1; i <= this->row; i++) {
            for (int j = 1; j <= this->col; j++) {
                if (this->get(i, j) > max) {
                    max = this->get(i, j);
                }
            }
        }
        return max;
    }
}

template<class T>
/*return the minimum value of the matrix */
T Mat<T>::min() {
    if (this->isSparse) {
        T min = 0;
        for (auto kv: (*this->pMap)) {
            if (kv.second < min) {
                min = kv.second;
            }
        }
        return min;
    } else {
        T min = 0;
        for (int i = 1; i <= this->row; i++) {
            for (int j = 1; j <= this->col; j++) {
                if (this->get(i, j) < min) {
                    min = this->get(i, j);
                }
            }
        }
        return min;
    }
}

template<class T>
T Mat<T>::sum() {
    if (this->isSparse) {
        T sum = 0;
        for (auto kv: (*this->pMap)) {
            sum += kv.second;
        }
        return sum;
    } else {
        T sum = 0;
        for (int i = 1; i <= this->row; i++) {
            for (int j = 1; j <= this->col; j++) {
                sum += this->get(i, j);
            }
        }
        return sum;
    }
}

template<class T>
T Mat<T>::avg() {
    if (this->isSparse) {
        T sum = 0;
        for (auto kv: (*this->pMap)) {
            sum += kv.second;
        }
        return sum / (*this->pMap).size();
    } else {
        T sum = 0;
        for (int i = 1; i <= this->row; i++) {
            for (int j = 1; j <= this->col; j++) {
                sum += this->get(i, j);
            }
        }
        return sum / (this->row * this->col);
    }
}

template<class T>
Mat<T> Mat<T>::conv(Mat<T> &kernel) {
    kernel = kernel.transpose().transpose(); // 翻转180度
    // center
    int x = kernel.row / 2;
    int y = kernel.col / 2;
    Mat<T> ans(row, col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            for (int m = 0; m < kernel.row; m++) // kernel rows
            {
                for (int n = 0; n < kernel.col; n++) // kernel columns
                {
                    int ii = i + (m - x);
                    int jj = j + (n - y);
                    // ignore input samples which are out of bound
                    if (ii >= 0 && ii < row && jj >= 0 && jj < col) {
                        ans.set(i + 1, j + 1, ans[i + 1][j + 1] + (this->get(ii + 1, jj + 1)) * (kernel[m + 1][n + 1]));
                    }
                }
            }
        }
    }
    return ans;
}

template<class T>
Mat<T> Mat<T>::transpose() {
    Mat<T> answer(col, row);
    for (int i = 1; i <= col; i++) {
        for (int j = 1; j <= row; j++) {
            answer.set(i, j, this->get(j, i));
        }
    }
    return answer;
}

template<class T>
Mat<T>::Mat(int row, int col, T *list, int n) {
    this->row = row;
    this->col = col;
    this->step = col;
    this->isSparse = false;
    if (n > row * col) throw (InvalidDimensionsException("InvalidDimensions"));
    this->pData = std::shared_ptr<T[]>(new T[this->row * this->col]);
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            this->pData[getIndex(i, j)] = 0;
        }
    }
    for (int i = 1; i <= row; ++i) {
        for (int j = 1; j <= col; ++j) {
            this->set(j, i, list[i - 1 + (j - 1) * this->step]);
        }
    }
}

template<class T2>
Mat<T2> operator+(Mat<T2> &lhs, Mat<T2> &rhs) {
    if (lhs.col != rhs.col || lhs.row != lhs.col || rhs.col != rhs.row)
        throw (OperatorAddition_MatrixNotMatched("Matrix not matched needs for addition"));

    Mat<T2> ans(lhs.row, lhs.row);
    for (int i = 1; i <= lhs.row; ++i) {
        for (int j = 1; j <= lhs.col; ++j) {
            ans.set(i, j, lhs.get(i, j) + rhs.get(i, j));
        }
    }

    return ans;
}

template<class T2>
Mat<T2> operator-(Mat<T2> &lhs, Mat<T2> &rhs) {
    if (lhs.col != rhs.col || lhs.row != lhs.col || rhs.col != rhs.row)
        throw (OperatorAddition_MatrixNotMatched("Matrix not matched needs for addition"));

    Mat<T2> ans(lhs.row, lhs.row);
    for (int i = 1; i <= lhs.row; ++i) {
        for (int j = 1; j <= lhs.col; ++j) {
            ans.set(i, j, lhs.get(i, j) - rhs.get(i, j));
        }
    }

    return ans;
}

template<class T2>
Mat<T2> operator*(double lhs, Mat<T2> &rhs) {
    Mat<T2> ans(rhs.row, rhs.row);
    for (int i = 1; i <= rhs.row; ++i) {
        for (int j = 1; j <= rhs.col; ++j) {
            ans.set(i, j, rhs.get(i, j) * lhs);
        }
    }

    return ans;
}

template<class T2>
Mat<T2> operator*(Mat<T2> &rhs, double lhs) {
    Mat<T2> ans(rhs.row, rhs.row);
    for (int i = 1; i <= rhs.row; ++i) {
        for (int j = 1; j <= rhs.col; ++j) {
            ans.set(i, j, rhs.get(i, j) * lhs);
        }
    }

    return ans;
}


#endif //MATRIX_MATRIX_HPP