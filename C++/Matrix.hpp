//
//  Matrix.h
//  TurbinesSimulator
//
//  Created by Sylvain Cusat-Blanc on 04/12/12.
//  Copyright (c) 2012 Sylvain Cusat-Blanc. All rights reserved.
//

#ifndef __TurbinesSimulator__Matrix__
#define __TurbinesSimulator__Matrix__

#include <iostream>
#include <vector>
#include <sstream>


template<class T> class Matrix {
public:
    unsigned int rows;
    unsigned int cols;

    //Matrix () {};
    Matrix (unsigned int rows=1, unsigned int cols=1);
    Matrix (const Matrix<T>* m);
    Matrix (unsigned int rows, unsigned int cols, const T* elements);
    Matrix (unsigned int rows, unsigned int cols, const std::vector<T>& elements);
    ~Matrix() {};

    inline T& operator()(unsigned int i, unsigned int j) {return elements[i*cols+j];};
    inline const T& operator()(unsigned int i, unsigned int j) const {return elements[i*cols+j];};
    inline const T& get(unsigned int i, unsigned int j) const {return elements[i*cols+j];};
    inline void set(unsigned int i, unsigned int j, const T& value);
    inline void set(unsigned int i, unsigned int j, T& value);

    bool operator==(const Matrix<T>* m) const;
    void operator=(const Matrix<T>* m);
    Matrix<T>* operator+(const Matrix<T>* m) const;
    Matrix<T>* operator-(const Matrix<T>* m) const;
    bool isZero() const;
    Matrix<T>* transpose() const;

    std::string toString() const;


    //Matrix<T>& addRow(unsigned int index, const Matrix<T>& row);
    //Matrix<T>& addCol(unsigned int index, const Matrix<T>& row);

    Matrix<T>* getCol(unsigned int index) const;
    Matrix<T>* getRow(unsigned int index) const;

private:
    std::vector<T> elements;
};

template<class T>
Matrix<T>::Matrix(unsigned int r, unsigned int c):
rows(r),
cols(c),
elements(r*c,T(0.0))
{
}

template<class T>
Matrix<T>::Matrix(const Matrix<T>* m):
rows(m->rows),
cols(m->cols),
elements(/*m.rows*m.cols, */m->elements) {
/*    for (unsigned i=0; i<m.rows*m.cols;i++) {
        this->elements[i]=m.elements[i];
    }*/
}

template<class T>
Matrix<T>::Matrix(unsigned int rows, unsigned int cols, const T* elements):
rows(rows),
cols(cols),
elements(rows*cols) {
    for (unsigned int i=0; i<rows*cols; i++) {
        this->elements[i]=elements[i];
    }
}

template<class T>
Matrix<T>::Matrix(unsigned int rows, unsigned int cols, const std::vector<T>& elements):
rows(rows),
cols(cols),
elements(elements) {
}

template<class T>
void Matrix<T>::set(unsigned int i, unsigned int j, const T& value) {
    elements[i*cols+j]=value;
}

template<class T>
void Matrix<T>::set(unsigned int i, unsigned int j, T& value) {
    elements[i*cols+j]=value;
}

template<class T>
void Matrix<T>::operator=(const Matrix<T>* m) {
    this->rows=m->rows;
    this->cols=m->cols;
    elements=std::vector<T>(rows*cols);
    for (unsigned int i=0; i<rows*cols; i++) {
        elements[i]=m->elements[i];
    }
}

template<class T>
bool Matrix<T>::operator==(const Matrix<T>* m) const {
    if (rows==m->rows && cols==m->cols) {
        for (unsigned int i=0; i<rows*cols; i++) {
            if (elements[i]!=m->elements[i]) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

template<class T>
bool Matrix<T>::isZero() const {
    for (unsigned int i=0; i<rows*cols; i++) {
        if (elements[i]!=0) {
            return false;
        }
    }
    return true;
}

/*template<class T>
Matrix<T>& Matrix<T>::addRow(unsigned int index, const Matrix<T> &row) {
    if (row.cols == cols) {
        if (index<rows) {
            for (unsigned int i=0; i<row.rows*row.cols; i++) {
                elements.insert(elements.begin()+index*cols, row.elements);
            }
            rows+=row.rows;
        } else {
            for (unsigned int i=0; i<row.rows*row.cols; i++) {
                elements.push_back(row.elements[i]);
            }
            rows+=row.rows;
        }
    }
    return *this;
}*/

template<class T>
Matrix<T>* Matrix<T>::getCol(unsigned int index) const {
    Matrix<T>* res=new Matrix<T>(rows, 1);
    for (unsigned int r=0; r<rows; r++) {
        res->set(r,0,get(r, index));;
    }
    return res;
}

template<class T>
Matrix<T>* Matrix<T>::getRow(unsigned int index) const {
    Matrix<T>* res = new Matrix<double>(1, cols);
    for (unsigned int c=0; c<cols; c++) {
        res->set(0,c,get(index, c));;
    }
    return res;
}

template<class T>
Matrix<T>* Matrix<T>::operator+(const Matrix<T>* m) const {
    Matrix<T>* res=new Matrix<T>(rows, cols);
    for (unsigned int i=0; i<rows; i++) {
        for (unsigned int j=0; j<cols; j++) {
            res->set(i,j,get(i,j)+m->get(i,j));
        }
    }
    return res;
}

template<class T>
Matrix<T>* Matrix<T>::operator-(const Matrix<T>* m) const {
    Matrix<T>* res = new Matrix<T>(rows, cols);
    for (unsigned int i=0; i<rows; i++) {
        for (unsigned int j=0; j<cols; j++) {
            res->set(i,j,get(i,j)-m->get(i,j));
        }
    }
    return res;
}

template<class T>
Matrix<T>* Matrix<T>::transpose() const {
    Matrix<T>* res = new Matrix<double>(cols, rows);
    for (unsigned int i=0; i<rows; i++) {
        for (unsigned int j=0; j<cols; j++) {
            res->set(i,j,get(j,i));
        }
    }
    return res;
}

template<class T>
std::string Matrix<T>::toString() const {
    std::stringstream ss;
    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            ss << this->get(i,j) << "\t";
        }
        ss << std::endl;
    }
    return ss.str();
}

#endif /* defined(__TurbinesSimulator__Matrix__) */
