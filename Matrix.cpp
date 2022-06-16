#include "Matrix.h"

template <class T>
Mat<T>::Mat(int row, int col, bool isSparse){
    this->row = row;
    this->col = col;
    this->step = col;
    this->isSparse = isSparse;
    if(this->isSparse) {
        this->pMap = std::shared_ptr<std::unordered_map<int, T>>(new std::unordered_map<int, T>);
    }
    else{
        this->pData = std::shared_ptr<T[]> (new T [this->row * this->col]);
        for(int i = 0; i < this-> row; i ++){
            for(int j = 0; j < this->col; j ++){
                this->pData[getIndex(i, j)] = 0;
            }
        }
    }
}

template <class T>
void Mat<T>::set(int x, int y, T val) {
    x--;
    y--;
    if(this->isSparse){
        (*this->pMap)[this->getIndex(x, y)] = val;
    }
    else{
        this->pData[this->getIndex(x, y)] = val;
    }
}

template <class T>
void Mat<T>::toDense() {
    if(this->isSparse) {
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

template <class T>
void Mat<T>::toSparse() {
    if(!this->isSparse) {
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

template <class T>
T Mat<T>::getIndex(int x, int y) {
    return x * this->step + y;
}

template <class T>
T Mat<T>::get(int x, int y){
    x--;
    y--;
    if(this->isSparse){
        if((*this->pMap).find(this->getIndex(x, y)) == (*this->pMap).end()){
            return 0;
        }
        else{
            return (*this->pMap)[this->getIndex(x, y)];
        }
    }
    else{
        return this->pData[this->getIndex(x, y)];
    }
}


