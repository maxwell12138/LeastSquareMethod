#include "InputTable.h"
#include <QAction>
#include <queue>
#include <QTableWidget>
#include <QMessageBox>
#include <QHeaderView>
#include <QCheckBox>
using namespace std;
InputTable::InputTable(QWidget *parent):QTableWidget(parent)
{
    tableInitial();
    popMenuInitial();
}

InputTable::~InputTable()
{
    delete popMenu;
}

//show popmenu at cursor
void InputTable::contextMenuEvent(QContextMenuEvent *)
{
    popMenu->move(cursor().pos());
    popMenu->show();
}

int InputTable::dataInput(std::vector<QPointF> &points)
{
    points.clear();
    QPointF p;
    for(int i=0;i<this->rowCount();i++){
            //if item is empty,return a null pointer and a error
        if(this->item(i,0)==0||this->item(i,1)==0){
            continue;
        }
        if(this->item(i,0)->text().isEmpty()||this->item(i,1)->text().isEmpty()){
            continue;
        }
        double num=this->item(i,0)->text().toDouble();
        p.setX(num);
        num=this->item(i,1)->text().toDouble();
        p.setY(num);
        points.push_back(p);
    }
    return points.size();
}

void InputTable::tableInitial()
{
    this->resize(190,300);
    this->setRowCount(7);
    this->setColumnCount(2);
    QStringList header;

    header<<"x"<<"y";
    this->horizontalHeader()->resizeSection(1,80);
    this->horizontalHeader()->resizeSection(0,80);

    //this->horizontalHeader()->setStretchLastSection(true);
    this->setHorizontalHeaderLabels(header);
    this->setItem(0,0,new QTableWidgetItem("2"));
    this->setItem(0,1,new QTableWidgetItem("4"));

    this->setItem(1,0,new QTableWidgetItem("5"));
    this->setItem(1,1,new QTableWidgetItem("13"));

    this->setItem(2,0,new QTableWidgetItem("8"));
    this->setItem(2,1,new QTableWidgetItem("30"));

    this->setItem(3,1,new QTableWidgetItem("20"));
    this->setItem(3,0,new QTableWidgetItem("10"));

    this->setItem(4,0,new QTableWidgetItem("100"));
    this->setItem(4,1,new QTableWidgetItem("34"));

    this->setItem(5,0,new QTableWidgetItem("50"));
    this->setItem(5,1,new QTableWidgetItem("30"));

    this->setItem(6,0,new QTableWidgetItem("20"));
    this->setItem(6,1,new QTableWidgetItem("70"));

//    this->setCellWidget(0,0,new QCheckBox(this));
//    this->setItem(0,1,new QTableWidgetItem("0"));
//    this->setItem(0,2,new QTableWidgetItem("30"));
//    this->setItem(0,3,new QTableWidgetItem("10-10*cos(x)"));//("sin(x)"));
//    for(int i=0;i<this->rowCount();i++){
//        for(int j=1;j<this->columnCount();j++){
//            //this->item(i,j)->setTextAlignment(Qt::AlignCenter);
//        }
//    }
}

void InputTable::popMenuInitial()
{
    popMenu=new QMenu(this);
    QAction *actionAdd=new QAction("add",popMenu);
    QAction *actionDelete=new QAction("delete",popMenu);
    popMenu->addAction(actionAdd);
    popMenu->addAction(actionDelete);
    QAction *actionDeleteThisRow=new QAction("deleteThis",popMenu);
    popMenu->addAction(actionDeleteThisRow);
    QAction *actionInsertUpRow=new QAction("insertUp",popMenu);
    popMenu->addAction(actionInsertUpRow);
    QAction *actionInsertDownRow=new QAction("insertDown",popMenu);
    popMenu->addAction(actionInsertDownRow);
    connect(actionAdd,SIGNAL(triggered()),this,SLOT(addRow()));
    connect(actionDelete,SIGNAL(triggered()),this,SLOT(deleteRow()));
    connect(actionDeleteThisRow,SIGNAL(triggered()),this,SLOT(deleteThisRow()));
    connect(actionInsertUpRow,SIGNAL(triggered()),this,SLOT(insertUpRow()));
    connect(actionInsertDownRow,SIGNAL(triggered()),this,SLOT(insertDownRow()));
}

void InputTable::addRow()
{
    int row = this->rowCount();
    this->setRowCount(row+1);
}

void InputTable::deleteRow()
{
    int row = this->rowCount();
    if(row<=0) return;
    this->setRowCount(row-1);
}

void InputTable::deleteThisRow()
{
    int rowIndex = this->currentRow();
    this->removeRow(rowIndex);
}

void InputTable::insertUpRow()
{
    int rowIndex = this->currentRow();
    this->insertRow(rowIndex);
}

void InputTable::insertDownRow()
{
    int rowIndex = this->currentRow();
    this->insertRow(rowIndex+1);
}

