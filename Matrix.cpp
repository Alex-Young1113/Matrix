#include "Matrix.h"

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
/* Not recommended way to visit elements, because there is no col size check. i.e.,
 * mat[row][col] will not throw exceptions even if col is beyond limit
 * Use mat[row][col] to visit or change the row-th row and col-th col element of the matrix.
 * */
template<class T>
T *Mat<T>::operator[](int n) {
    T *rt = new T[this->col];
    if (n <= 0 || n > this->row) {
        throw (InvalidCoordinatesException("Index out of range"));
    }
    rt = (this->pData.get()) + (n - 1) * this->row - 1;
    return rt;
}
