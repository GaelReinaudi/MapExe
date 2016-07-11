#ifndef GCLOSEBUTTONITEM_H
#define GCLOSEBUTTONITEM_H

#include <QGraphicsPixmapItem>

/////////////////////////////////////////////////////////////////////
//! \brief The GCloseButtonItem class is intended to provide deleting abilities to its parent container item
/*!
*/
class GCloseButtonItem : public QObject, public QGraphicsPixmapItem
{

public:
	GCloseButtonItem(QGraphicsItem* parentContainerItem);
	virtual ~GCloseButtonItem() {}

protected:
	//! Re-implemented
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	
};


#endif // GCLOSEBUTTONITEM_H
