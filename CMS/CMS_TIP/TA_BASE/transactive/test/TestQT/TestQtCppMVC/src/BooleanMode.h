#ifndef __CLASS_BOOLEAN_MODEL_HH__
#define __CLASS_BOOLEAN_MODEL_HH__

#include "ProjectCommonData.h"

//qt sys
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtCore/QMap>


#include "QtCore/QAbstractItemModel"
#include "QtCore/QAbstractTableModel"

#include "Node.h"


NS_BEGIN(TA_Base_App)

class BooleanModel : public QAbstractItemModel 
{ 
public: 
	BooleanModel(QObject *parent = 0); 
	virtual ~BooleanModel(); 

public: 
	void setRootNode(Node *pRootNode); 

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const; 
	QModelIndex parent(const QModelIndex &child) const; 

	int rowCount(const QModelIndex &parent) const; 
	int columnCount(const QModelIndex &parent) const; 
	QVariant data(const QModelIndex &nIndex, int role) const; 
	QVariant headerData(int section, Qt::Orientation orientation, int role) const; 
private: 
	Node* _GetNodeByIndex(const QModelIndex &nIndex) const; 

	Node* m_pRootNode; 
};


NS_END(TA_Base_App)




#endif//__CLASS_BOOLEAN_MODEL_HH__



