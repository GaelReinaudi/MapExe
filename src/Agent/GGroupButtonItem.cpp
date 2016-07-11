#include "GGroupButtonItem.h"
#include <QPixmap>
#include <QDebug>
#include <QDir>
#include "GRubberBandWrapitem.h"

GGroupButtonItem::GGroupButtonItem( QGraphicsItem* parentContainerItem )
    : QGraphicsPixmapItem(parentContainerItem)
    , QObject(dynamic_cast<QObject*>(parentContainerItem))
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    QPixmap createGroupPixmap;
    createGroupPixmap.load(":/images/createGroup.png");
    setPixmap(createGroupPixmap);
}

void GGroupButtonItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
    QGraphicsPixmapItem::mouseReleaseEvent(event);
    qDebug() << "GGroupButtonItem::mouseReleaseEvent";

    GRubberBandWrapitem* pParentWrapItem = dynamic_cast<GRubberBandWrapitem*>(parentItem()->parentItem());
    pParentWrapItem->CreateGroup();
}
