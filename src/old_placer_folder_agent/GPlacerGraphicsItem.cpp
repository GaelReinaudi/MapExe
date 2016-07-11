#include "GPlacerGraphicsItem.h"

GPlacerGraphicsItem::GPlacerGraphicsItem(GPlacerAgent* pAgent, GAgentTabletItem *parentItem /*= 0*/)
	: GAgentTabletItem(pAgent, parentItem)
{
	SetTabletRect(QRectF(0, 0, 1, 1));
	setBrush(QBrush(Qt::blue));
	if(!parentItem || !pAgent) {
		qWarning() << "GPlacerGraphicsItem" << "no parent provided to place things within. 514656";
		return;
	}
	connect(parentItem, SIGNAL(ChildAgentGraphicsItemAdded(GAgentTabletItem*, int)), this, SLOT(Event_SiblingAgentItemAdded(GAgentTabletItem*, int)));
}

GPlacerGraphicsItem::~GPlacerGraphicsItem()
{

}

/////////////////////////////////////////////////////////////////////
/*!
The default implementation calls the placer agent's AddItemToPlace().
When remainingItemsToBeAdded == 0, the signal SiblingItemsAdded() is emitted.
\param:  GAgentTabletItem * pAddedSiblingAgentItem : the item that was added to the as a sibling item.
\param:  int remainingItemsToBeAdded : estimation of items that are still to be added when many are added at once.
*////////////////////////////////////////////////////////////////////
void GPlacerGraphicsItem::Event_SiblingAgentItemAdded( GAgentTabletItem* pAddedSiblingAgentItem, int remainingItemsToBeAdded /*= 0*/ )
{
	PlacerAgent()->AddItemToPlace(pAddedSiblingAgentItem, remainingItemsToBeAdded);
	if(!remainingItemsToBeAdded) {
 		emit SiblingItemsAdded();
	}
}
