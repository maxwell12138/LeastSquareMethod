#include "figure.h"
#include <QPainter>
#include <cstring>
#include <iostream>
Figure::Figure(QWidget *parent) : QWidget(parent)
{
    curve=0;
    points.clear();
    x_scale=1.0;
    y_scale=1.0;
    scale=1.0;

    offset.setX(0);
    offset.setY(0);

    font.setPixelSize(16);
    leftMargin=2*margin;
    rightMargin=margin;
    upMargin=margin;
    downMargin=std::max(font.pixelSize(),4*margin);

    //tracer=false;
    this->deleteAttr(FIGURE::tracer);
    //interactive=true;
    this->addAttr(FIGURE::interactive);

    popMenuInit();

    this->setMouseTracking(true);
    //mousePressed=false;
    this->deleteAttr(FIGURE::mousePressed);
    this->addAttr(FIGURE::coorVisible);
}

Figure::~Figure()
{
    if(curve!=0) delete curve;
    points.clear();
}

void Figure::clear()
{
    title.clear();
    traceIndex=0;
    if(timerFlag) this->killTimer(timerFlag);
    timerFlag=0;

    minx=0;
    maxx=100;
    miny=0;
    maxy=100;

    //curve=0;
    if(curve!=0) delete curve;
    points.clear();
    x_scale=1.0;
    y_scale=1.0;
    scale=1.0;

    offset.setX(0);
    offset.setY(0);

    font.setPixelSize(16);
    leftMargin=2*margin;
    rightMargin=margin;
    upMargin=margin;
    downMargin=std::max(font.pixelSize(),4*margin);

    font.setPixelSize(16);
    this->deleteAttr(FIGURE::tracer);
    //tracer=false;
}

void Figure::clearPoints()
{
    points.clear();
}

void Figure::clearCurve()
{
    if(curve!=0) delete curve;
    curve=0;
}

void Figure::inputPoint(QPointF point){
    points.push_back(point);
    autoScale();
}


void Figure::autoScale()
{
    std::cout<<"scale"<<std::endl;
    autoMatch();
    if(curve==0&&points.empty())    return;
    if(curve!=0){
        minx=curve[0].x();
        maxx=curve[0].x();
        miny=curve[0].y();
        maxy=curve[0].y();
        for(int i=0;i<this->num;i++){
            if(minx>curve[i].x())   minx=curve[i].x();
            if(miny>curve[i].y())   miny=curve[i].y();
            if(maxx<curve[i].x())   maxx=curve[i].x();
            if(maxy<curve[i].y())   maxy=curve[i].y();
        }
    }
    if(!points.empty()){
        if(curve==0){
            QPointF p=*points.begin();
            minx=p.x();
            maxx=p.x();
            miny=p.y();
            maxy=p.y();
        }
        for(vector<QPointF>::iterator i=points.begin()+1;i!=points.end();i++){
            if(minx>(*i).x())   minx=(*i).x();
            if(miny>(*i).y())   miny=(*i).y();
            if(maxx<(*i).x())   maxx=(*i).x();
            if(maxy<(*i).y())   maxy=(*i).y();
        }
    }
    if(minx==maxx){
        minx=(int)minx;
        maxx=minx+1;
    }
    if(miny==maxy){
        miny=(int)miny;
        maxy=miny+1;
    }
    x_scale=chart_width/(maxx-minx);
    y_scale=chart_height/(maxy-miny);

}

void Figure::inputCurve(QPointF *curve,const int num){
    if(this->curve==curve) return;
    if(this->curve!=0) delete []this->curve;
    this->curve=new QPointF[num]();
    memcpy(this->curve,curve,num*sizeof(QPointF));
    this->num=num;
    this->autoScale();
}

void Figure::paintEvent(QPaintEvent *event)
{
    if(curve==0&&points.empty()){
        initial();
        return;
    }
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(curveLine);
    painter.setPen(pen);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.translate(original+offset);
    painter.translate(-minx*(x_scale*scale),miny*(y_scale*scale));


    //draw x y coordinate
    if(this->getAttr()&FIGURE::coorVisible){
        double w=painter.fontMetrics().width(QString::number(cursorPos.x()));
        double h=painter.fontMetrics().height();
        painter.drawText(cursorPos.x()*(x_scale*scale)-w-5,-5-cursorPos.y()*(y_scale*scale),QString::number(cursorPos.x()));
        painter.drawText(cursorPos.x()*(x_scale*scale)+5,-5-cursorPos.y()*(y_scale*scale),QString::number(cursorPos.y()));
    }

    if(curve!=0){
        for(int i=0;i<num-1;i++){
            painter.drawLine(QPointF(curve[i].x()*(x_scale*scale),-curve[i].y()*(y_scale*scale))
                            ,QPointF(curve[i+1].x()*(x_scale*scale),-curve[i+1].y()*(y_scale*scale)));
        }

        if(this->getAttr()&FIGURE::tracer){
            pen.setWidth(2);
            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.drawEllipse(QPointF(curve[traceIndex].x()*(x_scale*scale),
                                        -curve[traceIndex].y()*(y_scale*scale)),5.0,5.0);
            pen.setWidth(5);
            painter.setPen(pen);
            painter.drawPoint(QPointF(curve[traceIndex].x()*(x_scale*scale),
                                      -curve[traceIndex].y()*(y_scale*scale)));
        }
    }
    if(!points.empty()){
        for(vector<QPointF>::iterator i=points.begin();i!=points.end();i++){
            painter.save();
            pen.setWidth(dickLine);
            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.translate((*i).x()*(x_scale*scale),-(*i).y()*(y_scale*scale));
            painter.rotate(45);
            painter.drawLine(-5,0,5,0);
            painter.drawLine(0,-5,0,5);
            painter.restore();
        }
    }

    painter.restore();
    painter.eraseRect(0,0,this->width(),upMargin-dickLine);
    painter.eraseRect(0,0,leftMargin-dickLine,this->height());
    painter.eraseRect(this->width()-rightMargin+dickLine,0,rightMargin,this->height());
    painter.eraseRect(0,this->height()+dickLine-downMargin,this->width(),downMargin);


    double x1,x2,y1,y2;
    x1=offset.x()/(x_scale*scale)+minx;
    x2=offset.x()/(x_scale*scale)+maxx;//(offset.x()+chart_width)/(x_scale*scale)+minx;
    y1=offset.y()/(y_scale*scale)+miny;
    y2=offset.x()/(x_scale*scale)+maxy;//(offset.y()+chart_height)/(y_scale*scale)+miny;
    painter.drawText(leftMargin,this->height()-downMargin/2,QString::number(x1));
    painter.drawText(this->width()-3*rightMargin,this->height()-downMargin/2,QString::number(x2));
    painter.drawText(0,upMargin+chart_height,QString::number(y1));
    painter.drawText(0,2*upMargin,QString::number(y2));

    initial();
}


void Figure::timerEvent(QTimerEvent *)
{
    repaint();
}

/***
 *  draw chart frame and title
 */
void Figure::initial()
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(thinLine);
    painter.setPen(pen);

    painter.drawRect(leftMargin-2*dickLine,upMargin-2*dickLine,
                     chart_width+4*dickLine,chart_height+4*dickLine);

//    //x
//    if(miny<0&&maxy>0){
//        double y=original.y()-scale*y_scale*miny;
//        painter.drawLine(original.x(),y,original.x()+chart_width,y);
//    }else{
//        //painter.drawLine(original.x(),original.y(),original.x()+chart_width,original.y());

//    }
//    //y
//    if(minx<0&&maxx>0){
//        double x=original.x()+scale*x_scale*minx;
//        painter.drawLine(x,original.y(),x,original.y()-chart_height);
//    }else{
//        //painter.drawLine(original.x(),original.y(),original.x(),original.y()-chart_height);

//    }
    if(title!=0)
    {
        painter.setFont(font);
        double w=painter.fontMetrics().width(title);
        double h=painter.fontMetrics().height();
        painter.drawText(this->width()/2.0-w/2.0,upMargin+chart_height+2*h,title);
    }
}

void Figure::popMenuInit()
{
    popMenu=new QMenu(this);
    QAction *reset=new QAction("reset",popMenu);
    popMenu->addAction(reset);
    connect(reset,SIGNAL(triggered()),this,SLOT(reset()));
    QAction *interactive=new QAction("interactive",popMenu);
    popMenu->addAction(interactive);
    connect(interactive,SIGNAL(triggered()),this,SLOT(setInteractive()));
    QAction *coor=new QAction("coorVisible",popMenu);
    popMenu->addAction(coor);
    connect(coor,SIGNAL(triggered()),this,SLOT(setCoorVisible()));
}

/***
 * show popmenu at cursor
 */
void Figure::contextMenuEvent(QContextMenuEvent *)
{
    popMenu->move(cursor().pos());
    popMenu->show();
}

void Figure::reset()
{
    offset.setX(0);
    offset.setY(0);
    scale=1;
    this->autoScale();
    repaint();
}

void Figure::setInteractive(){
    if(this->getAttr()&FIGURE::interactive){
        this->deleteAttr(FIGURE::interactive);
    }else{
        this->addAttr(FIGURE::interactive);
    }
}

void Figure::setCoorVisible()
{
    if(this->getAttr()&FIGURE::coorVisible){
        this->deleteAttr(FIGURE::coorVisible);
    }else{
        this->addAttr(FIGURE::coorVisible);
    }
}


/***
 * match chart frame according to widget margin
 ***/
void Figure::autoMatch()
{
    chart_width=int(this->width()-leftMargin-rightMargin);
    chart_height=int(this->height()-upMargin-downMargin);

    original.setX(leftMargin);
    original.setY(chart_height+upMargin);//+this->height()-downMargin);
    //repaint();
}

QPointF Figure::getCoordinateFromScreen(QPoint p)
{
    double x,y;
    x=(p.x()-original.x()-offset.x())/(x_scale*scale)+minx;
    y=-(p.y()-original.y()-offset.y())/(y_scale*scale)+miny;
    return QPointF(x,y);
}

void Figure::mousePressEvent(QMouseEvent *event)
{
    if(this->getAttr()&FIGURE::interactive){
        pressPoint=event->pos();
    }
    this->addAttr(FIGURE::mousePressed);
    //mousePressed=true;
}

void Figure::mouseMoveEvent(QMouseEvent *event)
{
    if(this->getAttr()&FIGURE::mousePressed){
        if(this->getAttr()&FIGURE::interactive){
            this->offset=this->offset+event->pos()-pressPoint;
            pressPoint=event->pos();
            repaint();
        }
    }else{
        cursorPos=getCoordinateFromScreen(event->pos());
        repaint();
    }
}

void Figure::wheelEvent(QWheelEvent *event)
{
    if(this->getAttr()&FIGURE::interactive){
        double delta=event->delta()/1000.0;
        if(this->scale+delta<=0)    return;
        this->scale+=delta;
        repaint();
    }
}

void Figure::mouseReleaseEvent(QMouseEvent *event)
{
    this->deleteAttr(FIGURE::mousePressed);
    //mousePressed=false;
}



void Figure::grid()
{

}

void Figure::setTitle(QString title)
{
    this->title=title;
}

