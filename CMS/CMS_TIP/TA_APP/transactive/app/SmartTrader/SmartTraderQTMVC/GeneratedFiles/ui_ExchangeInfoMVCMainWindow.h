/********************************************************************************
** Form generated from reading UI file 'ExchangeInfoMVCMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXCHANGEINFOMVCMAINWINDOW_H
#define UI_EXCHANGEINFOMVCMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UIExchangeInfoMVCMainWindow
{
public:
    QWidget *WidgetExchangeInfo;
    QVBoxLayout *vboxLayout;
    QTreeView *ViewExchangeInfo;

    void setupUi(QMainWindow *UIExchangeInfoMVCMainWindow)
    {
        if (UIExchangeInfoMVCMainWindow->objectName().isEmpty())
            UIExchangeInfoMVCMainWindow->setObjectName(QString::fromUtf8("UIExchangeInfoMVCMainWindow"));
        UIExchangeInfoMVCMainWindow->resize(600, 600);
        WidgetExchangeInfo = new QWidget(UIExchangeInfoMVCMainWindow);
        WidgetExchangeInfo->setObjectName(QString::fromUtf8("WidgetExchangeInfo"));
        vboxLayout = new QVBoxLayout(WidgetExchangeInfo);
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        ViewExchangeInfo = new QTreeView(WidgetExchangeInfo);
        ViewExchangeInfo->setObjectName(QString::fromUtf8("ViewExchangeInfo"));
        ViewExchangeInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ViewExchangeInfo->setAlternatingRowColors(true);
        ViewExchangeInfo->setSelectionBehavior(QAbstractItemView::SelectItems);
        ViewExchangeInfo->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        ViewExchangeInfo->setAnimated(false);
        ViewExchangeInfo->setAllColumnsShowFocus(true);
        ViewExchangeInfo->header()->setVisible(false);
        ViewExchangeInfo->header()->setStretchLastSection(false);

        vboxLayout->addWidget(ViewExchangeInfo);

        UIExchangeInfoMVCMainWindow->setCentralWidget(WidgetExchangeInfo);

        retranslateUi(UIExchangeInfoMVCMainWindow);

        QMetaObject::connectSlotsByName(UIExchangeInfoMVCMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *UIExchangeInfoMVCMainWindow)
    {
        UIExchangeInfoMVCMainWindow->setWindowTitle(QApplication::translate("UIExchangeInfoMVCMainWindow", "windeowTitleExchangeInfoMainWindow", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UIExchangeInfoMVCMainWindow: public Ui_UIExchangeInfoMVCMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXCHANGEINFOMVCMAINWINDOW_H
