#ifndef __CLASS__SMARTTRADER_QTMVC_MAINWINDOW_H__
#define __CLASS__SMARTTRADER_QTMVC_MAINWINDOW_H__

#include <QMainWindow>
#include <QModelIndex>

#include "ui_SmartTraderQTMVCMainWindow.h"

class QAction;
class QTreeView;
class QWidget;



class SmartTraderQTMVCMainWindow : public QMainWindow, private Ui::UISmartTraderQTMVCMainWindow
{
    Q_OBJECT

public:
    SmartTraderQTMVCMainWindow(QWidget *parent = 0);

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
