#include "Matrix.h"
template <class T>
Mat<T>::Mat(int row, int col){
    this->row = row;
    this->col = col;
    this->step = col;
    this->numOfElements = 0;
    this->pData = nullptr;
}

template <class T>
bool Mat<T>::isSparse() {
    if(this->numOfElements < std::max(this->row, this->col)){
        return true;
    }
    else{
        return false;
    }
}

template <class T>
void Mat<T>::set(int x, int y, T val) {
    if(this->isSparse()){
        this->numOfElements ++;
        this->Map[this->getIndex(x, y)] = val;
        if(!this->isSparse()){
            this->toDense();
        }
        if(!this->isSparse()){
            this->toDense();
        }
    }
    else{
        this->numOfElements ++;
        this->pData[x * this->step + y] = val;
    }
}
template <class T>
void Mat<T>::toDense() {
    this->pData = new int[this->row * this->col];
    for(int i = 0; i < this-> row; i ++){
        for(int j = 0; j < this->col; j ++){
            this->pData[getIndex(i, j)] = 0;
        }
    }
    for(auto kv : this->Map){
        this->pData[kv.first] = kv.second;
    }
}
template <class T>
T Mat<T>::getIndex(int x, int y) {
    return x * this->step + y;
}
template <class T>
T Mat<T>::get(int x, int y){
    if(this->isSparse()){
        if(this->Map.find(this->getIndex(x, y)) == this->Map.end()){
            return 0;
        }
        else{
            return this->Map[this->getIndex(x, y)];
        }
    }
    else{
        return this->pData[this->getIndex(x, y)];
    }
}
