#include "GCloseButtonItem.h"
#include <QPixmap>
#include <QDebug>
#include <QDir>

GCloseButtonItem::GCloseButtonItem( QGraphicsItem* parentContainerItem ) 
	: QGraphicsPixmapItem(parentContainerItem)
    //, QGraphicsLayoutItem()
	, QObject(dynamic_cast<QObject*>(parentContainerItem))
{
	setFlag(QGraphicsItem::ItemIsSelectable);
	QPixmap closeButtonPixmap;
	closeButtonPixmap.load(":/images/closeButton.png");
//	closeButtonPixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	setPixmap(closeButtonPixmap);
//	 setPos(-closeButtonPixmap.rect().bottomRight());
}

void GCloseButtonItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	QGraphicsPixmapItem::mouseReleaseEvent(event);
	qDebug() << "GCloseButtonItem::mouseReleaseEvent";

    QGraphicsItem* pParent = parentItem()->parentItem();
	if(pParent) {
		// try to use delete later if it is a QObject
		QObject* pObj = dynamic_cast<QObject*>(pParent);
		if(pObj) {
			pObj->deleteLater();
		}
		else // else just delete
			delete pParent;
	}
}


