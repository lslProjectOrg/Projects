/********************************************************************************
** Form generated from reading UI file 'TestMVCMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTMVCMAINWINDOW_H
#define UI_TESTMVCMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UITestMVCMainWindow
{
public:
    QAction *exitAction;
    QAction *insertRowAction;
    QAction *removeRowAction;
    QAction *insertColumnAction;
    QAction *removeColumnAction;
    QAction *insertChildAction;
    QWidget *centralwidget;
    QVBoxLayout *vboxLayout;
    QTreeView *view;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *actionsMenu;
    QMenu *menuTestMVC;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 600);
        exitAction = new QAction(MainWindow);
        exitAction->setObjectName(QString::fromUtf8("exitAction"));
        insertRowAction = new QAction(MainWindow);
        insertRowAction->setObjectName(QString::fromUtf8("insertRowAction"));
        removeRowAction = new QAction(MainWindow);
        removeRowAction->setObjectName(QString::fromUtf8("removeRowAction"));
        insertColumnAction = new QAction(MainWindow);
        insertColumnAction->setObjectName(QString::fromUtf8("insertColumnAction"));
        removeColumnAction = new QAction(MainWindow);
        removeColumnAction->setObjectName(QString::fromUtf8("removeColumnAction"));
        insertChildAction = new QAction(MainWindow);
        insertChildAction->setObjectName(QString::fromUtf8("insertChildAction"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        vboxLayout = new QVBoxLayout(centralwidget);
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        view = new QTreeView(centralwidget);
        view->setObjectName(QString::fromUtf8("view"));
        view->setAlternatingRowColors(true);
        view->setSelectionBehavior(QAbstractItemView::SelectItems);
        view->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        view->setAnimated(false);
        view->setAllColumnsShowFocus(true);

        vboxLayout->addWidget(view);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 573, 21));
        fileMenu = new QMenu(menubar);
        fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
        actionsMenu = new QMenu(menubar);
        actionsMenu->setObjectName(QString::fromUtf8("actionsMenu"));
        menuTestMVC = new QMenu(menubar);
        menuTestMVC->setObjectName(QString::fromUtf8("menuTestMVC"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(fileMenu->menuAction());
        menubar->addAction(actionsMenu->menuAction());
        menubar->addAction(menuTestMVC->menuAction());
        fileMenu->addAction(exitAction);
        actionsMenu->addAction(insertRowAction);
        actionsMenu->addAction(insertColumnAction);
        actionsMenu->addSeparator();
        actionsMenu->addAction(removeRowAction);
        actionsMenu->addAction(removeColumnAction);
        actionsMenu->addSeparator();
        actionsMenu->addAction(insertChildAction);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("UITestMVCMainWindow", "Editable Tree Model", 0, QApplication::UnicodeUTF8));
        exitAction->setText(QApplication::translate("UITestMVCMainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
        exitAction->setShortcut(QApplication::translate("UITestMVCMainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        insertRowAction->setText(QApplication::translate("UITestMVCMainWindow", "Insert Row", 0, QApplication::UnicodeUTF8));
        insertRowAction->setShortcut(QApplication::translate("UITestMVCMainWindow", "Ctrl+I, R", 0, QApplication::UnicodeUTF8));
        removeRowAction->setText(QApplication::translate("UITestMVCMainWindow", "Remove Row", 0, QApplication::UnicodeUTF8));
        removeRowAction->setShortcut(QApplication::translate("UITestMVCMainWindow", "Ctrl+R, R", 0, QApplication::UnicodeUTF8));
        insertColumnAction->setText(QApplication::translate("UITestMVCMainWindow", "Insert Column", 0, QApplication::UnicodeUTF8));
        insertColumnAction->setShortcut(QApplication::translate("UITestMVCMainWindow", "Ctrl+I, C", 0, QApplication::UnicodeUTF8));
        removeColumnAction->setText(QApplication::translate("UITestMVCMainWindow", "Remove Column", 0, QApplication::UnicodeUTF8));
        removeColumnAction->setShortcut(QApplication::translate("UITestMVCMainWindow", "Ctrl+R, C", 0, QApplication::UnicodeUTF8));
        insertChildAction->setText(QApplication::translate("UITestMVCMainWindow", "Insert Child", 0, QApplication::UnicodeUTF8));
        insertChildAction->setShortcut(QApplication::translate("UITestMVCMainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        fileMenu->setTitle(QApplication::translate("UITestMVCMainWindow", "&File", 0, QApplication::UnicodeUTF8));
        actionsMenu->setTitle(QApplication::translate("UITestMVCMainWindow", "&Actions", 0, QApplication::UnicodeUTF8));
        menuTestMVC->setTitle(QApplication::translate("UITestMVCMainWindow", "TestMVC", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UITestMVCMainWindow: public Ui_UITestMVCMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTMVCMAINWINDOW_H
