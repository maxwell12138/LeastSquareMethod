#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>

class Matrix
{
public:
    Matrix();
    ~Matrix();
    Matrix(const int row,const int column);
    Matrix &operator *(const double multiplier);
    Matrix &operator /(const double denominator);
    Matrix &operator *(const Matrix &denominator);
    void operator =(Matrix &operand);
    void deepCopy(const Matrix &operand);

    void exchangeRow(const int row1,const int row2);
    //void addRow(const int to,const int add);
    Matrix &inverse();
    Matrix &transform();

    double determinant();
    void clear();
    void random(const int base=10);
    void eye();
    void output();
    void create(const int row,const int column);

    void input(const double *in, int num);
    void addRowTo(const int to,const int from,const double scale=1.0);
    void addColumnTo(const int to,const int from,const double scale=1.0);
    inline double getDataAt(int i,int j)const    {return data[i*column+j];}
    inline void setDataAt(int i,int j,double data) { this->data[i*column+j]=data;}
    inline void resize(int row,int column){this->row=row;this->column=column;}
    inline const int getColumn()const {return column;}
    inline const int getRow()const {return row;}
    inline double *getData()const {return data;}
private:
    double *data;
    int row;
    int column;
};

#endif // MATRIX_H
