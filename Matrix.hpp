#ifndef MATRIX_MATRIX_HPP
#define MATRIX_MATRIX_HPP

#include <cstdlib>
#include <ctime>

#define random(a, b) (rand()%(b-a)+a)

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
    long long getIndex(int x, int y) const; // return the offset of Mat[x][y]
    double EPS = 1e-9;
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


    Mat<T> transpose();

    Mat<T> resize(int x, int y);

    Mat<T> conv(Mat<T> &kernel);

    template<class T2>
    friend Mat<T2> dotMuilt(Mat<T2> const &lhs, Mat<T2> const &rhs);

    template<class T2>
    friend Mat<T2> operator+(Mat<T2> const &lhs, Mat<T2> const &rhs);

    template<class T2>
    friend Mat<T2> operator-(Mat<T2> const &lhs, Mat<T2> const &rhs);

    template<class T2>
    friend Mat<T2> operator*(double lhs, Mat<T2> const &rhs);

    template<class T2>
    friend Mat<T2> operator*(Mat<T2> const &lhs, double rhs);

    template<class T2>
    friend Mat<T2> operator*(Mat<T2> const &lhs, Mat<T2> const &rhs);

    template<class T2>
    friend Mat<T2> operator*(std::vector<T> const &lhs, Mat<T2> const &rhs);

    template<class T2>
    friend Mat<T2> operator*(Mat<T2> const &lhs, std::vector<T> const &rhs);

    T min();

    T sum();

    T avg();

    T max();

    T minRow(int r);

    T sumRow(int r);

    T maxRow(int r);

    T minCol(int c);

    T sumCol(int c);

    T maxCol(int c);

    void print(bool hasIndex = true, int width = 5); // print the matrix with specified width for each element

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

    Mat<T> inverse();

    void QR(Mat<T> &Q, Mat<T> &R); // 利用施密特正交化进行QR分解，这个方法并不是很成熟，就不让外部调用了

    void setZero();

    Mat<T> unitMatGen(int x);

    void eigen(Mat<T> &value, Mat<T> &vector);

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
long long Mat<T>::getIndex(int x, int y) const {
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
void Mat<T>::print(bool hasIndex, int w) {
    if (hasIndex) {
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
    } else {
        std::cout << " \n " << '[' << std::right;
        for (int i = 1; i <= this->row; i++) {

            for (int j = 1; j <= this->col; j++) {
                std::cout << std::setw(w) << get(i, j) << " ";
            }

            if (i == this->row) { std::cout << ']' << std::right; }
            else { std::cout << ',' << std::right; }
            std::cout << std::endl;
        }

    }
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
        throw (Addition_DimensionNotMatched("Matrix not matched needs for addition"));

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
        throw (Addition_DimensionNotMatched("Matrix not matched needs for addition"));

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
    ans.setZero();
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

    ans.setZero();
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
        throw (Multiply_DimensionsNotMatched(""));
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
    ans.setZero();
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

//template <class T>
//cv::Mat Mat<T>::toOpencv() {
//    cv::Mat ans(this->row, this->col, CV_64F);
//    for (int i = 1; i <= this->row; i++) {
//        for (int j = 1; j <= this->col; j++) {
//            ans.at<double>(i - 1, j - 1) = this->get(i, j);
//        }
//    }
//    return Mat<T>(rhs.row, rhs.col, &list);
//}

template<class T>
int Mat<T>::rank() {
    int res{};
    Mat<T> resMat = this->gauss();
    for (int i = 1; i <= this->row; ++i) {
        if (resMat.get(i, this->col) != 0) res++;
    }
    return res;
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
            if (fabs(out.get(k, i)) > fabs(out.get(max, i))) max = k;
        }
        if (fabs(out.get(max, i)) < EPS) continue;
        if (max != i) {
            for (int j = 1; j <= out.col; j++) {
                T2 mid = out.get(i, j);
                out.set(i, j, out.get(max, j));
                out.set(max, j, mid);
            }
        }
        //std::cout << "Test" << std::endl;
        for (int k = i + 1; k <= out.row; k++) {
            if (fabs(out.get(i, i)) < EPS) {
                break;
            }
            T2 a = -out.get(k, i) / out.get(i, i);
            for (int count = 1; count <= out.col; count++) {
                out.set(k, count, a * out.get(i, count) + out.get(k, count));
            }
        }
    }
    out.setZero();
    return out;
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
            if (fabs(out.get(k, i)) > fabs(out.get(max, i))) max = k;
        }
        if (fabs(out.get(max, i)) < EPS) continue;
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
            if (fabs(out.get(i, i)) < EPS) {
                break;
            }
            T2 a = -out.get(k, i) / out.get(i, i);
            l_cnt++;
            for (int count = 1; count <= out.col; count++) {
                out.set(k, count, a * out.get(i, count) + out.get(k, count));
            }
        }
    }
    out.setZero();
    cnt = l_cnt;
    return out;
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
    if (this->col != this->row) throw (Determinant_NotSquareMatrix(""));
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
            ans += this->get(i + 1, i + 1);
        }
    }
    return ans;
}

template<class T>
Mat<T> Mat<T>::inverse() {
    if (this->row != this->col) {
        throw Inverse_NotSquareMatrix("error: calculate the inverse of a non-square matrix");
    }
    int n = this->row;
    Mat<T> a(n, 2 * n);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            a.set(i, j, this->get(i, j));
        }
    }
    /* Augmenting Identity Matrix of Order n */
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i == j) {
                a.set(i, j + n, 1);
            } else {
                a.set(i, j + n, 0);
            }
        }
    }

    /* Applying Gauss Jordan Elimination */
    for (int i = 1; i <= n; i++) {
        if (a.get(i, i) == 0) {
            std::cout << "Mathematical Error!";
            throw (Inverse_NotInvertible(""));
        }
        for (int j = 1; j <= n; j++) {
            if (i != j) {
                double ratio = a.get(j, i) / a.get(i, i);
                for (int k = 1; k <= 2 * n; k++) {
                    a.set(j, k, a.get(j, k) - ratio * a.get(i, k));
                }
            }
        }
    }
    /* Row Operation to Make Principal Diagonal to 1 */
    for (int i = 1; i <= n; i++) {
        for (int j = n + 1; j <= 2 * n; j++) {
            a.set(i, j, a.get(i, j) / a.get(i, i));
        }
    }

    Mat<T> rt(n, n);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            rt.set(i, j, a.get(i, j + n));
        }
    }
    return rt;
}

template<class T>
void Mat<T>::setZero() {
    for (int i = 1; i <= row; ++i) {
        for (int j = 1; j <= col; ++j) {
            if (fabs(this->get(i, j)) < EPS) this->set(i, j, 0.0);
        }
    }
}

Mat<double> ranMatGen(int x, int y, int start = 1, int end = 10) {
    std::vector<double> list;
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            list.emplace_back(random(start, end));
        }
    }
    return Mat<double>(x, y, &list);
}

template<class T>
Mat<T> unitMatGen(int x) {
    std::vector<T> list;
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < x; ++j) {
            if (i != j) list.template emplace_back(0);
            else list.template emplace_back(1);
        }
    }
    return Mat<T>(x, x, &list);
}

template<class T2>
void Mat<T2>::QR(Mat<T2> &Q, Mat<T2> &R) {
    Mat<T2> temp = this->clone();
    if (Q.col != Q.row || R.col != R.row || Q.col != R.col)
        throw (InvalidDimensionsException("Q or R size mismatch!"));

    int i, j, k, m;
    T2 u, alpha, w, t;
    for (i = 1; i <= Q.col; i++) {
        for (j = 1; j <= Q.row; j++) {
            Q.set(i, j, 0);
            if (i == j) Q.set(i, j, 1);
        }
    }

    for (k = 1; k <= Q.col - 1; k++) {

        T2 u = 0;
        for (i = k; i <= Q.col; i++) {
            w = fabs(temp.get(i, k));
            if (w > u) u = w;
        }
        alpha = 0;

        for (i = k; i <= Q.col; i++) {
            t = temp.get(i, k) / u;
            alpha = alpha + t * t;
        }
        if (temp.get(k, k) > 0) u = -u;
        alpha = u * sqrt(alpha);
        if (fabs(alpha) + 1.0 == 1.0) {
            throw (InvalidCoordinatesException("QR分解失败!"));
        }

        u = sqrt(2 * alpha * (alpha - temp.get(k, k)));
        if ((u + 1) != 1) {
            temp.set(k, k, (temp.get(k, k) - alpha) / u);
            for (i = k + 1; i <= Q.col; i++) temp.set(i, k, temp.get(i, k) / u);

            for (j = 1; j <= Q.col; j++) {
                t = 0;
                for (m = k; m <= Q.col; m++)
                    t = t + temp.get(m, k) * Q.get(m, j);
                for (i = k; i <= Q.col; i++)
                    Q.set(i, j, Q.get(i, j) - 2 * t * temp.get(i, k));
            }

            for (j = k + 1; j <= Q.col; j++) {
                t = 0;
                for (m = k; m <= Q.col; m++)
                    t = t + temp.get(m, k) * temp.get(m, j);
                for (i = k; i <= Q.col; i++)
                    temp.set(i, j, temp.get(i, j) - 2 * t * temp.get(i, k));
            }

            temp.set(k, k, alpha);
            for (i = k + 1; i <= Q.col; i++) temp.set(i, k, 0);
        }
    }

    for (i = 1; i <= Q.col - 1; i++) {
        for (j = i + 1; j <= Q.col; j++) {
            t = Q.get(i, j);
            Q.set(i, j, Q.get(j, i));
            Q.set(j, i, t);
        }
    }

    R = temp.clone();
}

template<class T2>
void Mat<T2>::eigen(Mat<T2> &value, Mat<T2> &vector) {
    Mat<T2> temp = this->clone();
    if (temp.col != temp.row) {
        throw (InvalidDimensionsException("Only square matrices have eigenvalues and eigenvectors."));
    } else if (value.col != this->col || value.row != 1) {
        throw (InvalidDimensionsException("Size of matrices for eigenvalue should be 1 X N."));
    } else if (vector.col != vector.row || vector.row != this->row) {
        throw (InvalidDimensionsException("Size for eigenvetor container mismatch"));
    }

    Mat<T2> Q(temp.row, temp.col);
    Mat<T2> R(temp.row, temp.col);
    for (int count = 1; count <= 50; count++) {
        temp.QR(Q, R);
        temp = R * Q;
    }

    for (int i = 1; i <= value.col; i++)
        value.set(1, i, temp.get(i, i));

    T2 evalue;
    for (int i = 1; i <= value.col; i++) {
        evalue = value.get(1, i);
        temp = this->clone();
        //temp.print();
        for (int j = 1; j <= temp.col; j++)
            temp.set(j, j, temp.get(j, j) - evalue);

        //temp.print();
        temp = temp.gauss();
        //std::cout << "After gauss" << std::endl;
        //temp.print();
        for (int j = temp.col; j >= 1; j--) {
            if (temp.get(j, j) != 0) {
                for (int k = 1; k <= j - 1; k++) {
                    T2 ratio = -temp.get(k, j) / temp.get(j, j);
                    for (int count = 1; count <= temp.col; count++) {
                        temp.set(k, count, temp.get(k, count) + ratio * temp.get(j, count));
                    }
                }
            }
        }
        //std::cout << "Guass again" << std::endl;
        //temp.print();
        for (int j = 1; j <= vector.row; j++) {
            vector.set(j, i, temp.get(j, j));
        }
    }

}

template<class T>
T Mat<T>::minRow(int r) {
    T min = this->get(1, 1);
    for (int i = 1; i <= col; ++i) {
        if (this->get(r, i) < min) min = this->get(r, i);
    }
    return min;
}

template<class T>
T Mat<T>::minCol(int c) {
    T min = this->get(1, 1);
    for (int i = 1; i <= row; ++i) {
        if (this->get(i, c) < min) min = this->get(i, c);
    }
    return min;
}

template<class T>
T Mat<T>::maxCol(int c) {
    T max = this->get(1, 1);
    for (int i = 1; i <= row; ++i) {
        if (this->get(i, c) > max) max = this->get(i, c);
    }
    return max;
}

template<class T>
T Mat<T>::maxRow(int r) {
    T max = this->get(1, 1);
    for (int i = 1; i <= col; ++i) {
        if (this->get(r, i) > max) max = this->get(r, i);
    }
    return max;
}

#endif //MATRIX_MATRIX_HPP