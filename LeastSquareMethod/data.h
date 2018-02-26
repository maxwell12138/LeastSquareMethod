#ifndef DATA_H
#define DATA_H
#include <vector>
#include <QPointF>
#include <QString>
#include "matrix.h"
class Data
{
public:
    Data();
    ~Data();
    void cal();
    QString &getExpression();
    void setOrder(int order);
    inline int getOrder(){return this->order;}
    double getMaxX();
    double getMaxY();
    double getMinX();
    double getMinY();
    inline std::vector<QPointF> &getPoints(){return points;}
    void generateMatrix();
    QPointF *createCurve(double start,double end,const int num);
private:
    Matrix a;
    Matrix x;
    Matrix y;
    int order;
    std::vector<QPointF> points;
};



#endif // DATA_H
