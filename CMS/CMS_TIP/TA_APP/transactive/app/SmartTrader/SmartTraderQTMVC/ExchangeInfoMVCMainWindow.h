#ifndef __CLASS__EXCHANGE_INFO_MVC__MAINWINDOW_H__
#define __CLASS__EXCHANGE_INFO_MVC__MAINWINDOW_H__

#include <QMainWindow>
#include <QModelIndex>

#include "ui_ExchangeInfoMVCMainWindow.h"

class QAction;
class QTreeView;
class QWidget;



class ExchangeInfoMVCMainWindow : public QMainWindow, private Ui::UIExchangeInfoMVCMainWindow
{
    Q_OBJECT

public:
    ExchangeInfoMVCMainWindow(QWidget *parent = 0);
private slots:
	void slotTreeViewDoubleClick(const QModelIndex & index);
};



#endif//__CLASS__EXCHANGE_INFO_MVC__MAINWINDOW_H__
