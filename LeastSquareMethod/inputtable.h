#ifndef MYTABLE_H
#define MYTABLE_H
#include <QTableWidget>
#include <QMenu>
#include <vector>
class InputTable : public QTableWidget
{
    Q_OBJECT

public:
    InputTable(QWidget *parent = 0);
    virtual ~InputTable();
    void contextMenuEvent(QContextMenuEvent *);
    int dataInput(std::vector<QPointF> &points);
private:

    QMenu *popMenu;
    void tableInitial();
    void popMenuInitial();
private slots:
    void addRow();
    void deleteRow();
    void deleteThisRow();
    void insertUpRow();
    void insertDownRow();
};

#endif // MYTABLE_H
