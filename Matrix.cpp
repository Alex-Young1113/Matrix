#include "Matrix.h"

template<typename T>
Mat<T>::Mat(int row, int col){
    this->row = row;
    this->col = col;
    this->step = col;
    this->numOfElements = 0;
    this->pData = nullptr;
}

template<typename T>
bool Mat<T>::isSparse() {
    if(this->numOfElements < std::min(this->row, this->col)){
        return true;
    }
    else{
        return false;
    }
}

template<typename T>
void Mat<T>::set(int row, int col, T val) {
    if(this->isSparse()){
        this->Map.insert(Point(row, col), val);
        this->numOfElements ++;
        if(!this->isSparse()){
            this->toDense();
        }
    }
    else{
        this->pData[row * this->step + col] = val;
    }
}

template<typename T>
void Mat<T>::toDense() {
    this->pData = new T[this->row * this->col];
    for(auto kv : this->Map){
        this->set(kv.first.x, kv.first.y, kv.second);
    }
}
