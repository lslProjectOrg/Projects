#ifndef __CLASS__TEST_MVC__MAINWINDOW_H__
#define __CLASS__TEST_MVC__MAINWINDOW_H__

#include <QMainWindow>
#include <QModelIndex>

#include "ui_TestMVCMainWindow.h"

class QAction;
class QTreeView;
class QWidget;



class TestMVCMainWindow : public QMainWindow, private Ui::UITestMVCMainWindow
{
    Q_OBJECT

public:
    TestMVCMainWindow(QWidget *parent = 0);

public slots:
    void updateActions();

private slots:
    void insertChild();
    bool insertColumn(const QModelIndex &parent = QModelIndex());
    void insertRow();
    bool removeColumn(const QModelIndex &parent = QModelIndex());
    void removeRow();
};



#endif//__CLASS__SMARTTRADER_QTMVC_MAINWINDOW_H__
