#ifndef MATRIX_MATRIX_HPP
#define MATRIX_MATRIX_HPP

#include <unordered_map>
#include <memory>
#include <cstddef>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "Exception.h"

template<class T>
class Mat {
    T getIndex(int x, int y) const; // return the offset of Mat[x][y]
public:
    std::shared_ptr<std::unordered_map<int, T>> pMap; // hashmap to store elements in sparse matrix
    std::shared_ptr<T[]> pData; // array to store elements in dense matrix
    long long row = 0; // number of rows
    long long col = 0; // number of columns
    long long step = 0; // used for computing the index of next row
    bool isSparse = false; // 0 for dense matrix and 1 for sparse matrix
    Mat() = default;

    Mat(int row, int col, std::vector<T> *list = nullptr,
        bool isSparse = false); // construct an all zero matrix with x rows and y columns
    void toDense(); // convert mat to dense matrix
    void toSparse(); // convert mat to dense matrix
    void set(int x, int y, T val); // set Mat[x][y] to val
    T get(int, int) const; // return Mat[x][y]

    T max();


    Mat<T> transpose();

    Mat<T> resize(int x, int y);

    Mat<T> conv(Mat<T> &kernel);

    template<class T2>
    friend Mat<T2> dotMuilt(Mat<T2> &lhs, Mat<T2> &rhs);

    template<class T2>
    friend Mat<T2> operator+(Mat<T2> &lhs, Mat<T2> &rhs);

    template<class T2>
    friend Mat<T2> operator-(Mat<T2> &lhs, Mat<T2> &rhs);

    template<class T2>
    friend Mat<T2> operator*(double lhs, Mat<T2> &rhs);

    template<class T2>
    friend Mat<T2> operator*(Mat<T2> &lhs, double rhs);

    template<class T2>
    friend Mat<T2> operator*(Mat<T2> lhs, Mat<T2> rhs);

    T min();

    T sum();

    T avg();

    void print(int width = 5); // print the matrix with specified width for each element
    Mat<T> clone(); // deep copy

    Mat<T> gauss();

    Mat<T> gauss(int &rowCount);

    int rank();

    Mat<T> getSubmatrix(int colstart, int colend, int rowstart, int rowend);    // 获取子矩阵，也是自用

    Mat<T> getCominor(int x, int y);


    std::vector<T> getRow(int l_row);

    std::vector<T> getCol(int l_col);

    T det();

    operator std::vector<std::vector<T>>() const {
        std::vector<std::vector<T>> ans;
        for (int i = 0; i < this->row; ++i) {
            std::vector<T> rowVec;
            for (int j = 0; j < this->col; ++j) {
                rowVec.template emplace_back(this->get(i + 1, j + 1));
            }
            ans.template emplace_back(rowVec);
        }
        return ans;
    }

    double trace();

private:
    void QR(Mat<T> &Q, Mat<T> &R); // 利用施密特正交化进行QR分解，这个方法并不是很成熟，就不让外部调用了



};

template<class T>
Mat<T>::Mat(int row, int col, std::vector<T> *list, bool isSparse) {
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
    if (list != nullptr) {
        int cnt = 0;
        for (int i = 1; i <= row; i++) {
            for (int j = 1; j <= col; j++) {
                if (cnt < (*list).size()) {
                    this->set(i, j, (*list)[cnt]);
                    cnt++;
                } else {
                    break;
                }
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
T Mat<T>::getIndex(int x, int y) const {
    return x * this->step + y;
}

template<class T>
T Mat<T>::get(int x, int y) const {
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
    for (int i = 1; i <= (long long) (w + 1) * (long long) (this->col); i++) {
        std::cout << "-";
    }
    std::cout << std::endl;
    for (int i = 1; i <= this->row; i++) {
        std::cout << std::setw(w - 1) << std::left << i << "|" << std::right;
        for (int j = 1; j <= this->col; j++) {
            std::cout << std::setw(w) << get(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template<class T>
Mat<T> Mat<T>::clone() {
    std::vector<T> tmp(this->row * this->col, 0);
    Mat<T> rt(this->row, this->col, &tmp, this->isSparse);
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
    T max = this->get(1, 1);
    for (int i = 1; i <= this->row; i++) {
        for (int j = 1; j <= this->col; j++) {
            if (this->get(i, j) > max) {
                max = this->get(i, j);
            }
        }
    }
    return max;
}

template<class T>
/*return the minimum value of the matrix */
T Mat<T>::min() {
    T min = this->get(1, 1);
    for (int i = 1; i <= this->row; i++) {
        for (int j = 1; j <= this->col; j++) {
            if (this->get(i, j) < min) {
                min = this->get(i, j);
            }
        }
    }
    return min;
}

template<class T>
T Mat<T>::sum() {
    T sum = this->get(1, 1);
    for (int i = 1; i <= this->row; i++) {
        for (int j = 1; j <= this->col; j++) {
            if (i != 1 || j != 1) {
                sum += this->get(i, j);
            }
        }
    }
    return sum;
}

template<class T>
T Mat<T>::avg() {
    return this->sum() / (this->row * this->col);
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
                        ans.set(i + 1, j + 1,
                                ans.get(i + 1, j + 1) + (this->get(ii + 1, jj + 1)) * (kernel.get(m + 1, n + 1)));
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

template<class T2>
Mat<T2> operator+(Mat<T2> &lhs, Mat<T2> &rhs) {
    if (lhs.col != rhs.col || lhs.row != lhs.col || rhs.col != rhs.row)
        throw (InvalidDimensionsException("Matrix not matched needs for addition"));

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
        throw (InvalidDimensionsException("Matrix not matched needs for addition"));

    Mat<T2> ans(lhs.row, lhs.col);
    for (int i = 1; i <= lhs.row; ++i) {
        for (int j = 1; j <= lhs.col; ++j) {
            ans.set(i, j, lhs.get(i, j) - rhs.get(i, j));
        }
    }

    return ans;
}

template<class T2>
Mat<T2> operator*(double lhs, Mat<T2> &rhs) {
    Mat<T2> ans(rhs.row, rhs.col);
    for (int i = 1; i <= rhs.row; ++i) {
        for (int j = 1; j <= rhs.col; ++j) {
            ans.set(i, j, rhs.get(i, j) * lhs);
        }
    }

    return ans;
}

template<class T2>
Mat<T2> operator*(Mat<T2> &rhs, double lhs) {
    Mat<T2> ans(rhs.row, rhs.col);
    for (int i = 1; i <= rhs.row; ++i) {
        for (int j = 1; j <= rhs.col; ++j) {
            ans.set(i, j, rhs.get(i, j) * lhs);
        }
    }

    return ans;
}

template<class T2>
Mat<T2> Mat<T2>::getSubmatrix(int rowstart, int rowend, int colstart, int colend) {
    if (colend > colstart || rowend > rowstart ||
        colend > this->col || rowend > this->row ||
        colstart < 1 || rowend < 1)
        throw (InvalidCoordinatesException("Coordinate for submatrix is out of bound."));
    colstart--;
    colend--;
    rowstart--;
    rowend--;
    Mat<T2> ans;
    ans.col = colend - colstart + 1;
    ans.row = rowend - rowstart + 1;
    ans.step = this->step;
    ans.pData = std::shared_ptr<T2[]>(
            this->pData.get() + this->getIndex(rowstart, colstart));
    return ans;
}

template<class T2>
void Mat<T2>::QR(Mat<T2> &Q, Mat<T2> &R) {
}


template<class T2>
Mat<T2> operator*(std::vector<T2> &lhs, Mat<T2> &rhs) {
    if (rhs.row == 1 && rhs.col == lhs.size()) {
        Mat<T2> ans(rhs.col, rhs.col);
        std::vector<T2> list;
        for (int i = 0; i < rhs.col; ++i) {
            for (int j = 0; j < rhs.col; ++j) {
                list.template emplace_back(lhs[i] * rhs.get(1, j + 1));
            }
        }
        return Mat<T2>(rhs.col, rhs.col, &list);
    } else if (rhs.row != 1 && rhs.row == lhs.size()) {
        std::vector<T2> list;
        for (int i = 0; i < rhs.col; ++i) {
            T2 t = 0;
            for (int j = 0; j < rhs.row; ++j) {
                t += lhs[j] * rhs.get(j + 1, i + 1);
            }
            list.template emplace_back(t);
        }
        return Mat<T2>(1, rhs.col, &list);
    } else {
        std::cerr << "Dimension not matched for multiply" << "\n";
        throw (Multiply_DimensionsNotMatched(""));
    }
}

template<class T2>
Mat<T2> operator*(Mat<T2> lhs, Mat<T2> rhs) {
    if (lhs.col != rhs.row) {
        throw InvalidDimensionsException("");
    }
    Mat<T2> ans(lhs.row, rhs.col);
    for (int i = 1; i <= ans.row; i++) {
        for (int j = 1; j <= ans.col; j++) {
            T2 sum = 0;
            for (int k = 1; k <= lhs.col; k++) {
                sum += lhs.get(i, k) * rhs.get(k, j);
            }
            ans.set(i, j, sum);
        }
    }
    return ans;
}

template<class T>
Mat<T> Mat<T>::resize(int x, int y) {
    Mat<T> ans(x, y);
    int i_ = 1;
    int j_ = 1;
    for (int i = 1; i <= x; i++) {
        for (int j = 1; j <= y; j++) {
            if (i_ <= this->row && j_ <= this->col) {
                ans.set(i, j, this->get(i_, j_));
            } else {
                ans.set(i, j, 0);
            }
            j_++;
            if (j_ == this->col) {
                j_ = 0;
                i_++;
            }
        }
    }
    return ans;
}

template<class T2>
Mat<T2> operator*(Mat<T2> &lhs, std::vector<T2> &rhs) {
    if (lhs.col == 1 && lhs.col == rhs.size()) {
        Mat<T2> rhsMat(1, rhs.size(), &rhs);
        return lhs * rhsMat;
    } else if (lhs.col != 1 && lhs.col == rhs.size()) {
        Mat<T2> rhsMat(rhs.size(), 1, &rhs);
        return lhs * rhsMat;
    } else {
        std::cerr << "Dimension not matched for multiply" << "\n";
        throw (Multiply_DimensionsNotMatched(""));
    }
}

template<class T>
Mat<T> dotMuilt(Mat<T> &lhs, Mat<T> &rhs) {
    if (lhs.row != rhs.row || lhs.col != rhs.col)
        throw (Multiply_DimensionsNotMatched(""));
    std::vector<T> list;
    for (int i = 0; i < lhs.row; ++i) {
        for (int j = 0; j < lhs.col; ++j) {
            list.template emplace_back(lhs.get(i + 1, j + 1) * rhs.get(i + 1, j + 1));
        }
    }
    return Mat<T>(rhs.row, rhs.col, &list);
}

template<class T2>
Mat<T2> Mat<T2>::gauss(int &cnt) {
    Mat<T2> out = this->clone();
    int i = 1;
    int pivot = 1;
    int l_cnt = 0;
    //out.print();

    for (; i <= out.row && pivot <= out.col; i++, pivot++) {
        if (i > out.col) {
            break;
        }

        for (int k = i + 1; k <= out.row; k++) {
            if (out.get(i, pivot) != 0) {
                break;
            } else if (out.get(k, pivot) != 0) {
                l_cnt++;
                for (int count = 1; count <= out.col; count++) {
                    T2 mid = out.get(i, count);
                    out.set(i, count, out.get(k, count));
                    out.set(k, count, mid);
                }
                break;
            }
        }
    }
    //out.print();

    for (i = 1; i <= out.row && out.col; i++) {
        int max = i;
        for (int k = i; k <= out.row; k++) {
            if (out.get(k, i) > out.get(max, i)) max = k;
        }
        if (fabs(out.get(max, i)) < 1e-10) continue;
        if (max != i) {
            l_cnt++;
            for (int j = 1; j <= out.col; j++) {
                T2 mid = out.get(i, j);
                out.set(i, j, out.get(max, j));
                out.set(max, j, mid);
            }
        }
        //std::cout << "Test" << std::endl;
        for (int k = i + 1; k <= out.row; k++) {
            if (fabs(out.get(i, i)) < 1e-10) {
                break;
            }
            T2 a = -out.get(k, i) / out.get(i, i);
            l_cnt++;
            for (int count = 1; count <= out.col; count++) {
                out.set(k, count, a * out.get(i, count) + out.get(k, count));
            }
        }
    }
    cnt = l_cnt;
    return out;
}


template<class T2>
Mat<T2> Mat<T2>::gauss() {
    Mat<T2> out = this->clone();
    int i = 1;
    int pivot = 1;
    //out.print();

    for (; i <= out.row && pivot <= out.col; i++, pivot++) {
        if (i > out.col) {
            break;
        }

        for (int k = i + 1; k <= out.row; k++) {
            if (out.get(i, pivot) != 0) {
                break;
            } else if (out.get(k, pivot) != 0) {
                for (int count = 1; count <= out.col; count++) {
                    T2 mid = out.get(i, count);
                    out.set(i, count, out.get(k, count));
                    out.set(k, count, mid);
                }
                break;
            }
        }
    }
    //out.print();

    for (i = 1; i <= out.row && out.col; i++) {
        int max = i;
        for (int k = i; k <= out.row; k++) {
            if (out.get(k, i) > out.get(max, i)) max = k;
        }
        if (fabs(out.get(max, i)) < 1e-10) continue;
        if (max != i) {
            for (int j = 1; j <= out.col; j++) {
                T2 mid = out.get(i, j);
                out.set(i, j, out.get(max, j));
                out.set(max, j, mid);
            }
        }
        //std::cout << "Test" << std::endl;
        for (int k = i + 1; k <= out.row; k++) {
            if (fabs(out.get(i, i)) < 1e-10) {
                break;
            }
            T2 a = -out.get(k, i) / out.get(i, i);
            for (int count = 1; count <= out.col; count++) {
                out.set(k, count, a * out.get(i, count) + out.get(k, count));
            }
        }
    }

    return out;
}


template<class T>
int Mat<T>::rank() {
    int res{};
    Mat<T> resMat = this->gauss();
    for (int i = 1; i <= this->row; ++i) {
        if (resMat.get(i, this->col) != 0) res++;
    }
    return res;
}

template<class T>
std::vector<T> Mat<T>::getRow(int l_row) {
    std::vector<T> res;
    for (int i = 1; i <= this->col; ++i) {
        res.template emplace_back(this->get(l_row, i));
    }
    return res;
}

template<class T>
std::vector<T> Mat<T>::getCol(int l_col) {
    std::vector<T> res;
    for (int i = 1; i <= this->row; ++i) {
        res.template emplace_back(this->get(i, l_col));
    }
    return res;
}

template<class T>
//获取代数余子式
Mat<T> Mat<T>::getCominor(int x, int y) {
    if (x > this->row || y > this->col) throw (Cominor_CoordinateExceedsBounds(""));
    std::vector<T> res;
    for (int i = 1; i <= this->row; ++i) {
        for (int j = 1; j <= this->col; ++j) {
            if (i != x || j != y) res.template emplace_back(this->get(i, j));
        }
    }
    return Mat<T>(this->row - 1, this->col - 1, &res);
}

template<class T>
T Mat<T>::det() {
    if (this->col != this->row) throw (Determinant_NotSquareMatirx(""));
    int cnt{};
    T ans = 1;
    Mat<T> res = this->gauss(cnt);
    for (int i = 1; i <= this->col; ++i) {
        ans *= res.get(i, i);
    }
    return pow(-1, cnt) * ans;
}

template<class T>
double Mat<T>::trace() {
    double ans{};
    if (this->col != this->row) throw (Trace_NotSquareMatrix(""));
    else {
        for (int i = 0; i < this->row; ++i) {
            ans += this->get(i+1,i+1);
        }
    }
    return ans;
}

#endif //MATRIX_MATRIX_HPP