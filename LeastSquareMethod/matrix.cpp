#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <set>

Matrix::Matrix()
{
    column=0;
    row=0;
    data=0;
}

Matrix::~Matrix()
{
    if(NULL!=data)  free(data);
}

Matrix::Matrix(const int row,const int column):row(row),column(column)
{
    void *p;
    do{
        p=malloc(row*column*sizeof(double));
    }while(p==NULL);
    data=(double *)p;
    clear();
}

Matrix &Matrix::operator *(const double multiplier)
{
    Matrix *a=new Matrix(this->row,this->column);
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++){
            a->setDataAt(i,j,this->getDataAt(i,j)*multiplier);
        }
    }
    return *a;
}

Matrix &Matrix::operator /(const double denominator)
{
    Matrix *a=new Matrix(this->row,this->column);
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++){
            a->setDataAt(i,j,this->getDataAt(i,j)/denominator);
        }
    }
    return *a;
}

Matrix &Matrix::operator *(const Matrix &multiplier)
{
    Matrix *result;
    if(this->column!=multiplier.row) return *result;
    result=new Matrix(this->row,multiplier.column);
    //result->create(this->row,multiplier.column);
    for(int i=0;i<row;i++){
        for(int j=0;j<multiplier.column;j++){
            double temp=0;
            for(int k=0;k<column;k++){
                temp+=(this->getDataAt(i,k))*(multiplier.getDataAt(k,j));
            }
            result->setDataAt(i,j,temp);
        }
    }
    return *result;
}

void Matrix::operator =(Matrix &operand)
{
    if(data!=0) delete []data;
    this->data=operand.getData();
    this->column=operand.getColumn();
    this->row=operand.getRow();
}

void Matrix::deepCopy(const Matrix &operand)
{
    this->create(operand.getRow(),operand.getColumn());
    memcpy(this->data,operand.getData(),row*column*sizeof(double));
}

void Matrix::exchangeRow(const int row1,const int row2)
{
    if(row1<0||row1>row) return;
    if(row2<0||row2>row) return;
    double temp;
    for(int j=0;j<row;j++){
        temp=this->getDataAt(row1,j);
        this->setDataAt(row1,j,getDataAt(row2,j));
        this->setDataAt(row2,j,temp);
    }
}

//void Matrix::addRow(const int to,const int add)
//{
//    if(add<0||add>row) return;
//    if(to<0||to>row) return;
//    for(int j=0;j<row;j++){
//        this->setDataAt(to,j,getDataAt(add,j)+getDataAt(to,j));
//    }
//}

Matrix &Matrix::inverse()
{
    Matrix *result,original;
    if(row!=column) return *result;
    original.deepCopy(*this);
    result=new Matrix(row,row);
    result->eye();
    for(int k=0;k<row;k++){
        double scale=original.getDataAt(k,k);
        if(scale==0){
            for(int i=0;i<row;i++){
                if(original.getDataAt(i,k)!=0){
                    original.addRowTo(k,i,1);
                    result->addRowTo(k,i,1);
                    scale=original.getDataAt(k,k);
                    break;
                }
            }
        }

        if(scale==0){
            result->clear();
            return *result;
        }

        for(int j=0;j<row;j++){
            original.setDataAt(k,j,original.getDataAt(k,j)/scale);
            result->setDataAt(k,j,result->getDataAt(k,j)/scale);
        }
        for(int i=0;i<row;i++){
            if(i==k) continue;
            scale=original.getDataAt(i,k);
            for(int j=0;j<row;j++){
                original.setDataAt(i,j,original.getDataAt(i,j)-scale*original.getDataAt(k,j));
                result->setDataAt(i,j,result->getDataAt(i,j)-scale*result->getDataAt(k,j));
            }
        }
    }
    return *result;
}


Matrix &Matrix::transform()
{
//    Matrix result(column,row);
//    for(int i=0;i<column;i++){
//        for(int j=0;j<row;j++){
//            result.setDataAt(i,j,this->getDataAt(j,i));
//        }
//    }
//    return result;

    Matrix *result=new Matrix(column,row);
    for(int i=0;i<column;i++){
        for(int j=0;j<row;j++){
            double a=this->getDataAt(j,i);
            result->setDataAt(i,j,this->getDataAt(j,i));
        }
    }
    return *result;
}


double Matrix::determinant()
{
    double result=1;
    if(row!=column) return 0;
    if(row==1)  return getDataAt(0,0);
    Matrix original;
    original.deepCopy(*this);
    for(int j=0;j<row-1;j++){
        double scale=0;
        for(int i=row-1;i>j;i--){
            if(original.getDataAt(i,j)==0) continue;
            if(original.getDataAt(i-1,j)!=0){
                scale=-original.getDataAt(i,j)/original.getDataAt(i-1,j);
                original.addRowTo(i,i-1,scale);
            }else{
                original.exchangeRow(i,i-1);
                result=-1*result;
            }
        }
    }
    for(int i=0;i<row;i++){
        result=result*original.getDataAt(i,i);
    }
    return result;
}


void Matrix::clear()
{
    for(int i=0;i<column*row;i++){
        data[i]=0;
    }
}


void Matrix::random(const int base)
{
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++){
            data[i*column+j]=rand()%base;
        }
    }
}


void Matrix::eye()
{
    if(!data||row!=column) return;
    for(int i=0;i<row;i++){
        for(int j=0;j<row;j++){
            if(i==j){
                setDataAt(i,j,1);
            }else{
                setDataAt(i,j,0);
            }
        }
    }
}


void Matrix::output()
{
    if(data==0) return;
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++){
            std::cout<<data[i*column+j]<<" ";
        }
        std::cout<<std::endl;
    }
}


void Matrix::create(const int row, const int column)
{
    if(this->data!=0){
        if(this->column*this->row==row*column){
            this->column=column;
            this->row=row;
            this->clear();
            return;
        }else{
            free(data);
        }
    }
    this->row=row;
    this->column=column;

    void *p;
    do{
        p=malloc(row*column*sizeof(double));
    }while(p==NULL);
    data=(double *)p;
    clear();
}

void Matrix::input(const double *in,int num)
{
    if(row==0||column==0) return;
    if(data==0) this->create(row,column);

    if(num>column*row){
        memcpy(this->data,in,column*row*sizeof(double));
    }else{
        memcpy(this->data,in,num*sizeof(double));
        if(num<column*row){
            for(int i=num;i<column*row;i++){
                data[i]=0;
            }
        }
    }
}

void Matrix::addRowTo(const int to, const  int from, const double scale)
{
    if(to<0||from<0||to>=row||from>=row) return;
    for(int j=0;j<column;j++){
        double temp=getDataAt(to,j)+getDataAt(from,j)*scale;
        setDataAt(to,j,temp);
    }
}

void Matrix::addColumnTo(const int to, const int from, const double scale)
{
    if(to<0||from<0||to>=column||from>=column) return;
    for(int i=0;i<row;i++){
        double temp=getDataAt(i,to)+getDataAt(i,from)*scale;
        setDataAt(i,to,temp);
    }
}

