#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "matrix.h"
#include <vector>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    Matrix a(2,2);
//    a.output();
//    a.random();
//    a.setDataAt(1,1,10);
//    a.output();
//    std::cout<<a.determinant()<<std::endl;
//    Matrix b(3,2);
//    b.random();
//    std::cout<<b.determinant()<<std::endl;
//    Matrix c(3,3);
//    //double array[4]={4,4,3,23};
//    //c.input(array,4);
//    c.random();
//    c.output();
//    std::cout<<c.determinant()<<std::endl;
    data=new Data();
    data->setOrder(3);
    this->setFixedSize(750,400);
    ui->setupUi(this);
    ui->widget->autoMatch();
    ui->widget->addAttr(FIGURE::interactive);
}
MainWindow::~MainWindow()
{
    delete data;
    delete ui;
}


void MainWindow::dataInput()
{
    data->setOrder(ui->comboBox->currentText().toInt());
    int num=ui->tableWidget->dataInput(data->getPoints());
    if(data->getOrder()+1>num){
        QMessageBox::warning(this,tr("warning"),tr("sample must be more than order!"),QMessageBox::Ok);
        return;
    }
    ui->widget->clearPoints();
    ui->widget->reset();
    for(std::vector<QPointF>::iterator i=data->getPoints().begin();
        i!=data->getPoints().end();i++){
        ui->widget->inputPoint(*i);
    }
    data->cal();
    double start=data->getMinX();
    double end=data->getMaxX();
    QPointF *p=data->createCurve(start-(end-start)/10.0,end+(end-start)/10.0,50);
    ui->widget->inputCurve(p,50);
    delete p;
    p=NULL;
    ui->widget->autoScale();

    QString str("Order:");
    str+=QString::number(data->getOrder());
    ui->widget->setTitle(str);
    ui->textBrowser->setText(data->getExpression());
    ui->widget->repaint();
}


void MainWindow::dataInput(QString order)
{
    data->setOrder(order.toInt());
    int num=ui->tableWidget->dataInput(data->getPoints());
    if(data->getOrder()+1>num){
        QMessageBox::warning(this,tr("warning"),tr("sample must be more than order!"),QMessageBox::Ok);
        return;
    }
    ui->widget->clearPoints();
    ui->widget->reset();
    for(std::vector<QPointF>::iterator i=data->getPoints().begin();
        i!=data->getPoints().end();i++){
        ui->widget->inputPoint(*i);
    }
    data->cal();
    double start=data->getMinX();
    double end=data->getMaxX();
    QPointF *p=data->createCurve(start-(end-start)/10.0,end+(end-start)/10.0,50);
    ui->widget->inputCurve(p,50);
    delete p;
    std::cout<<" 1024 "<<endl;
    static bool flag=true;
    if(flag){
        ui->widget->autoScale();
        flag=false;
        std::cout<<"flag";
    }

    QString str("Order:");
    str+=QString::number(data->getOrder());
    ui->widget->setTitle(str);
    ui->textBrowser->setText(data->getExpression());
    ui->widget->repaint();
}
