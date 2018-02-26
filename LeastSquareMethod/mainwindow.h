#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "data.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void dataInput();
    void dataInput(QString order);
private:
    Data *data;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
