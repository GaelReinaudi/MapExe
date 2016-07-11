#ifndef GGROUPBUTTONITEM_H
#define GGROUPBUTTONITEM_H

#include <QGraphicsPixmapItem>

/////////////////////////////////////////////////////////////////////
//! \brief The GCloseButtonItem class is intended to provide deleting abilities to its parent container item
/*!
*/
class GGroupButtonItem : public QObject, public QGraphicsPixmapItem
{

public:
    GGroupButtonItem(QGraphicsItem* parentContainerItem);
    virtual ~GGroupButtonItem() {}
protected:
    //! Re-implemented
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
};

#endif // GGROUPBUTTONITEM_H
