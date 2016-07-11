#ifndef GFOLDERGRAPHICSITEM_H
#define GFOLDERGRAPHICSITEM_H

#include <QtGui>
#include "agent.h"
class GFolderAgent;

/////////////////////////////////////////////////////////////////////
//! \brief The GFolderGraphicsItem class is the class that defines a graphics item for representing GFolderAgent object.
/*!
*/
class GFolderGraphicsItem : public QGraphicsRectItem, public GHasAgent
{
public:
    GFolderGraphicsItem(GFolderAgent* pAgent, QGraphicsItem *parentItem = 0);
    virtual ~GFolderGraphicsItem();

	//! Re-implemented to adapt the opacity as we zoom in/out
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget * widget = 0);

private:
    QGraphicsTextItem m_FolderNameItem;
};


#endif // GFOLDERGRAPHICSITEM_H
