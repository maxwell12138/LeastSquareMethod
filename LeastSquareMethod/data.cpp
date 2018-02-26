#include "data.h"
#include "math.h"
#include <algorithm>
#include "def.h"
Data::Data()
{
    this->order=1;
}
Data::~Data(){
    points.clear();
}
void Data::cal()
{
    this->generateMatrix();

    Matrix temp=a.transform();
    Matrix temp1=temp*a;
    if(temp1.determinant()<threshold&&temp1.determinant()>-threshold)    return;
    std::cout<<"determinant"<<temp1.determinant()<<std::endl;
    Matrix temp2=temp1.inverse();
    temp1=temp2*temp;
    x=temp1*y;
}

QString &Data::getExpression()
{
    QString *title=new QString("y=");
    if(x.getDataAt(0,0)>threshold||x.getDataAt(0,0)<-threshold){
        *title+=QString::number(x.getDataAt(0,0));
    }
    if(x.getDataAt(1,0)<-threshold){
        *title+=QString::number(x.getDataAt(1,0));
        *title+="*x";
    }else if(x.getDataAt(1,0)>threshold){
        *title+="+";
        *title+=QString::number(x.getDataAt(1,0));
        *title+="*x";
    }


    for(int i=2;i<order+1;i++){
        if(x.getDataAt(i,0)<-threshold){
            *title+=QString::number(x.getDataAt(i,0));
            *title+="*x^";
            *title+=QString::number(i);
        }else if(x.getDataAt(i,0)>threshold){
            *title+="+";
            *title+=QString::number(x.getDataAt(i,0));
            *title+="*x^";
            *title+=QString::number(i);
        }
    }
    return *title;
}

void Data::setOrder(int order)
{
    if(order<0) return;
    this->order=order;
}

double Data::getMaxX()
{
    std::vector<QPointF>::iterator p=std::max_element(points.begin(),points.end(),minX);
    return (*p).x();
}

double Data::getMaxY()
{
    std::vector<QPointF>::iterator p=std::max_element(points.begin(),points.end(),minY);
    return (*p).x();
}

double Data::getMinX()
{
    std::vector<QPointF>::iterator p=std::min_element(points.begin(),points.end(),minX);
    return (*p).x();
}

double Data::getMinY()
{
    std::vector<QPointF>::iterator p=std::min_element(points.begin(),points.end(),minY);
    return (*p).x();
}
void Data::generateMatrix()
{
    if(points.empty()) return;
    a.create(points.size(),order+1);
    y.create(points.size(),1);
    for(int i=0;i<points.size();i++){
        QPointF p=points[i];
        y.setDataAt(i,0,p.y());
        a.setDataAt(i,0,1);
        for(int j=1;j<order+1;j++){
            a.setDataAt(i,j,pow(p.x(),j));
        }
    }
    a.output();
}

QPointF *Data::createCurve(double start,double end,const int num)
{
    if(end==start) return 0;
    double interval=(end-start)/(double)(num-1);
    QPointF *p=new QPointF[num]();
    for(int i=0;i<num;i++){
        p[i].setX(start+interval*i);
        double y=0;
        for(int j=0;j<order+1;j++){
            y+=x.getDataAt(j,0)*pow(p[i].x(),j);
        }
        p[i].setY(y);
    }
    return p;
}


