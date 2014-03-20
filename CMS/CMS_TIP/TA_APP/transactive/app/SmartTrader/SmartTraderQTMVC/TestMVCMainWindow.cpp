#include <QtGui>

#include "TestMVCMainWindow.h"
#include "TreeItem.h"
#include "TreeModel.h"


using namespace TA_App_App;


TestMVCMainWindow::TestMVCMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

#ifdef Q_WS_MAEMO_5
    // Alternating row colors look bad on Maemo
    view->setAlternatingRowColors(false);
#endif

	//Data
	TreeItem* pRoot = NULL;
	TreeItem* pRootColumn = NULL;
	TreeItem* pItemNewTmp = NULL;
	TreeItem* pItemParentTmp = NULL;

	QList<QVariant> dataTmp;
	dataTmp.clear();
	dataTmp.push_back("root");
	pRoot = new TreeItem(dataTmp, NULL);


	dataTmp.clear();
	dataTmp.push_back("root_column");
	pRootColumn = new TreeItem(dataTmp, pRoot);
	pRoot->appendChild(pRootColumn);


	dataTmp.clear();
	dataTmp.push_back("AAAA");
	pItemNewTmp = new TreeItem(dataTmp, pRootColumn);
	pRootColumn->appendChild(pItemNewTmp);
	pItemParentTmp = pItemNewTmp;


	dataTmp.clear();
	dataTmp.push_back("AAAA_A");
	pItemNewTmp = new TreeItem(dataTmp, pItemParentTmp);
	pItemParentTmp->appendChild(pItemNewTmp);
	pItemParentTmp = pItemNewTmp;


	dataTmp.clear();
	dataTmp.push_back("AAAA_A_A");
	pItemNewTmp = new TreeItem(dataTmp, pItemParentTmp);
	pItemParentTmp->appendChild(pItemNewTmp);
	pItemParentTmp = pItemNewTmp;

	dataTmp.clear();
	dataTmp.push_back("BBBB");
	pItemNewTmp = new TreeItem(dataTmp, pRootColumn);
	pRootColumn->appendChild(pItemNewTmp);
	pItemParentTmp = pItemNewTmp;

	dataTmp.clear();
	dataTmp.push_back("BBBB_B");
	pItemNewTmp = new TreeItem(dataTmp, pItemParentTmp);
	pItemParentTmp->appendChild(pItemNewTmp);
	pItemParentTmp = pItemNewTmp;


	dataTmp.clear();
	dataTmp.push_back("BBBB_B_B");
	pItemNewTmp = new TreeItem(dataTmp, pItemParentTmp);
	pItemParentTmp->appendChild(pItemNewTmp);
	pItemParentTmp = pItemNewTmp;


	//
	TreeModel* pTreeModel = NULL;
	pTreeModel = new TreeModel(NULL);
	pTreeModel->setRootItem(pRoot);

    view->setModel(pTreeModel);
    for (int column = 0; column < pTreeModel->columnCount(); ++column)
        view->resizeColumnToContents(column);

    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(view->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &,
                                    const QItemSelection &)),
            this, SLOT(updateActions()));

    connect(actionsMenu, SIGNAL(aboutToShow()), this, SLOT(updateActions()));
    connect(insertRowAction, SIGNAL(triggered()), this, SLOT(insertRow()));
    connect(insertColumnAction, SIGNAL(triggered()), this, SLOT(insertColumn()));
    connect(removeRowAction, SIGNAL(triggered()), this, SLOT(removeRow()));
    connect(removeColumnAction, SIGNAL(triggered()), this, SLOT(removeColumn()));
    connect(insertChildAction, SIGNAL(triggered()), this, SLOT(insertChild()));

    updateActions();
}

void TestMVCMainWindow::insertChild()
{
    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();

    if (model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;

    for (int column = 0; column < model->columnCount(index); ++column) {
        QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"),
                                 Qt::EditRole);
    }

    view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
    updateActions();
}

bool TestMVCMainWindow::insertColumn(const QModelIndex &parent)
{
    QAbstractItemModel *model = view->model();
    int column = view->selectionModel()->currentIndex().column();

    // Insert a column in the parent item.
    bool changed = model->insertColumn(column + 1, parent);
    if (changed)
        model->setHeaderData(column + 1, Qt::Horizontal, QVariant("[No header]"),
                             Qt::EditRole);

    updateActions();

    return changed;
}

void TestMVCMainWindow::insertRow()
{
    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();

    if (!model->insertRow(index.row()+1, index.parent()))
        return;

    updateActions();

    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        QModelIndex child = model->index(index.row()+1, column, index.parent());
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
    }
}

bool TestMVCMainWindow::removeColumn(const QModelIndex &parent)
{
    QAbstractItemModel *model = view->model();
    int column = view->selectionModel()->currentIndex().column();

    // Insert columns in each child of the parent item.
    bool changed = model->removeColumn(column, parent);

    if (!parent.isValid() && changed)
        updateActions();

    return changed;
}

void TestMVCMainWindow::removeRow()
{
    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}

void TestMVCMainWindow::updateActions()
{
    bool hasSelection = !view->selectionModel()->selection().isEmpty();
    removeRowAction->setEnabled(hasSelection);
    removeColumnAction->setEnabled(hasSelection);

    bool hasCurrent = view->selectionModel()->currentIndex().isValid();
    insertRowAction->setEnabled(hasCurrent);
    insertColumnAction->setEnabled(hasCurrent);

    if (hasCurrent) {
        view->closePersistentEditor(view->selectionModel()->currentIndex());

        int row = view->selectionModel()->currentIndex().row();
        int column = view->selectionModel()->currentIndex().column();
        if (view->selectionModel()->currentIndex().parent().isValid())
            statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
        else
            statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
    }
}




