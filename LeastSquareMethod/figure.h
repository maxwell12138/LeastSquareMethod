#ifndef FIGURE_H
#define FIGURE_H
#include <vector>
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMenu>
#include <QAction>
using namespace std;
const int dickLine=3;
const int thinLine=1;
const int dotLine=1;
const int margin=12;
const int tick=5;
const int curveLine=2;
namespace FIGURE {
    enum attrType{clear=0x0000,tracer=0x0001,interactive=0x0002,mousePressed=0x0004,
                  coorVisible=0x0008};
}

class Figure : public QWidget
{
    Q_OBJECT
public:
    Figure(QWidget *parent = 0);
    ~Figure();
    void clear();
    void clearPoints();
    void clearCurve();

    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *);
    void grid();
    void setTitle(QString title);
    void autoMatch();

    QPointF getCoordinateFromScreen(QPoint p);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    inline void setTraceIndex(int index){this->traceIndex=index;}

    void inputCurve(QPointF *curve,const int num);
    void inputPoint(QPointF point);
    void autoScale();
    void initial();
    void contextMenuEvent(QContextMenuEvent *);

    //Attribution operation funcitons
    inline void addAttr(FIGURE::attrType type){this->attr=this->attr|type;}
    inline const int getAttr(){return this->attr;}
    inline void deleteAttr(const FIGURE::attrType type) {this->attr=this->attr&(~type);}
    inline void clearAttr(){this->attr=FIGURE::clear;}
private:
    QMenu *popMenu;
    void popMenuInit();
    QString title;
    int traceIndex;
    int timerFlag;

    double minx,maxx,miny,maxy;
    int chart_width,chart_height;
    double x_scale,y_scale,scale;
    QPoint offset;
    int leftMargin,rightMargin,upMargin,downMargin;
    int x_num,y_num;

    QPointF *curve;
    int num;
    vector<QPointF> points;
private:
    QFont font;
    QPoint original;
    QPoint pressPoint;
    QPointF cursorPos;

//    bool tracer;
//    bool interactive;
//    bool mousePressed;

    int attr;
public slots:
    void reset();
    void setInteractive();
    void setCoorVisible();
    //inline void setInteractive(bool interactive){this->interactive=interactive;repaint();}
};


#endif // FIGURE_H
