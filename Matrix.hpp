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
//asd
template<class T>
class Mat {

    T getIndex(int x, int y); // return the offset of Mat[x][y]
public:
    std::shared_ptr<std::unordered_map<int, T>> pMap; // hashmap to store elements in sparse matrix
    std::shared_ptr<T[]> pData; // array to store elements in dense matrix
    long long row = 0; // number of rows
    long long col = 0; // number of columns
    long long step = 0; // used for computing the index of next row
    bool isSparse = false; // 0 for dense matrix and 1 for sparse matrix
    Mat() = default;;

    Mat(int row, int col, std::vector<T> *list = nullptr, bool isSparse = false); // construct an all zero matrix with x rows and y columns
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
    int rank();
    Mat<T> getsubmatrix(int colstart, int colend, int rowstart, int rowend);    // 获取子矩阵，也是自用
private:
    void QR(Mat<T>& Q, Mat<T>& R); // 利用施密特正交化进行QR分解，这个方法并不是很成熟，就不让外部调用了
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
    if(list != nullptr){
        int cnt = 0;
        for(int i = 1; i <= row; i ++){
            for(int j = 1; j <= col; j ++){
                if(cnt < (*list).size()){
                    this->set(i, j, (*list)[cnt]);
                    cnt ++;
                }
                else{
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
    T max = this->get(1,1);
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
    T min = this->get(1,1);
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
    T sum = this->get(1,1);
    for (int i = 1; i <= this->row; i++) {
        for (int j = 1; j <= this->col; j++) {
            if(i != 1 || j != 1) {
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
                        ans.set(i + 1, j + 1, ans.get(i+1, j+1) + (this->get(ii + 1, jj + 1)) * (kernel.get(m+1, n+1)));
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
Mat<T2> Mat<T2>::getsubmatrix(int colstart, int colend, int rowstart, int rowend){
    if(colend > colstart || rowend > rowstart ||
        colend > this->col || rowend > this->row ||
            colstart < 1 || rowend < 1)
        throw(InvalidCoordinatesException("Coordinate for submatrix is out of bound."));
    colstart --;
    colend --;
    rowstart --;
    rowend --;
    Mat<T2> ans;
    ans.col = colend - colstart + 1;
    ans.row = rowend - rowstart + 1;
    ans.step = this->step;
    ans.pData = std::shared_ptr<T2[]>(
            this->pData.get() + this->getIndex(rowstart, colstart));
    return ans;
}

template<class T2>
void Mat<T2>::QR(Mat<T2>& Q, Mat<T2>& R){
    

}

template<class T2>
int Mat<T2>::rank(){
    int re = 0;
    Mat<T2> temp = this->clone();
    for (int i = 1; i <= temp.row; i++){
        int row, col;
        for (col = i; col <= temp.col; col++){
            bool ok = false;
            for (row = i; row <= temp.row; row ++){
                if (fabs(temp.get(row, col) > 1e-10)) {
                    ok = true;
                    break;
                }
            }
            if (ok) break;
        }
    
        if (row <= temp.row && col <= temp.col) {
            for (int j = col; j <= temp.col; j++){
               T2 mid = temp.get(0, j);
               temp.set(0, j, temp.get(i, j));
               temp.set(i, j, temp.get(row, j));
               temp.set(row, j, temp.get(0, j));
            }
        }
        T2 a = 0;
        for(int j = i + 1; j <= temp.row; j++){
            a = -temp.get(j, col)/temp.get(i, col);
            for (int k = col; k <= temp.row; k++){
                T2 mid = temp.get(j, k);
                mid += a*temp.get(i, k);
                temp.set(j, k, mid);
            }
        }
        re++;
    }

    return re;
}

template<class T2>
Mat<T2> operator*(std::vector<T2> lhs, Mat<T2> &rhs) {
    if (rhs.row == 1 && rhs.col == lhs.size()) {
        Mat<T2> ans(rhs.col, rhs.col);
        std::vector<T2> list;
        for (int i = 0; i < rhs.col; ++i) {
            for (int j = 0; j < rhs.col; ++j) {
                list.template emplace_back(lhs[i] * rhs.get(1, j + 1));
            }
        }
        return Mat<T2>((int) rhs.col, (int) rhs.col, &list);
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

#endif //MATRIX_MATRIX_HPP