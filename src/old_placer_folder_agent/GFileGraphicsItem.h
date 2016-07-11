#ifndef GFILEGRAPHICSITEM_H
#define GFILEGRAPHICSITEM_H

#include <QtGui>
#include "agent.h"
class GFileAgent;

class GFileGraphicsItem : public QObject, public QGraphicsRectItem, public GHasAgent
{
	Q_OBJECT 

public:
    GFileGraphicsItem(GFileAgent* pAgent, QGraphicsItem *parentItem = 0);
    virtual ~GFileGraphicsItem();

protected slots:
	void Event_FileInfoUpdated( QFileInfo fileInfo );

private:
	QGraphicsTextItem m_FileNameItem;
};


#endif // GFILEGRAPHICSITEM_H
