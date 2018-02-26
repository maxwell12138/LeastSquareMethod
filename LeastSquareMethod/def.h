#ifndef DEF_H
#define DEF_H
bool minY(QPointF p1,QPointF p2){
    return p1.y()<p2.y();
}
bool minX(QPointF p1,QPointF p2){
    return p1.x()<p2.x();
}
const double threshold=10e-10;

#endif // DEF_H

