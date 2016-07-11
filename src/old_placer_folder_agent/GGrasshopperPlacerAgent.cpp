#include "GGrasshopperPlacerAgent.h"

GGrasshopperPlacerAgent::GGrasshopperPlacerAgent(QObject *parent)
	: GPlacerAgent(parent)
{
// 	// connection to start the separate thread
// 	connect(this, SIGNAL(NoMoreItemsToAddForNow(GAgentTabletItem*)), this, SLOT(start()));
}

GGrasshopperPlacerAgent::~GGrasshopperPlacerAgent()
{
//	wait();
	terminate();
}

void GGrasshopperPlacerAgent::MoveAwayFromSiblings(GAgentTabletItem* pItem, int remainingItemsToBeAdded /*= 0*/ )
{
	if(!pItem)
		return;
	GAgentTabletItem* pParentItem = pItem->ParentAgentGraphicsItem();
	if(!pParentItem)
		return;
	QList<QGraphicsItem*> parentChildren = pParentItem->childItems();
	QList<QGraphicsItem*> colliding = pItem->collidingItems();
	QSet<QGraphicsItem*> collidingSiblings = parentChildren.toSet().intersect(colliding.toSet());

	if(collidingSiblings.isEmpty()) {
		FinishedPlacingItems(pParentItem);
		return;
	}
	//! center of mass of those colliding siblings
	QPointF centerMass(0.0, 0.0);
	foreach(QGraphicsItem* pIt, collidingSiblings) {
		QPointF pointFromItem = pItem->mapFromItem(pIt, pIt->boundingRect().center());
		centerMass += pointFromItem - pItem->boundingRect().center();
	}
	//! Move away form there
	QPointF MoveBy = -centerMass;
	MoveBy /= MoveBy.manhattanLength() + 0.01;
	MoveBy /= MoveBy.manhattanLength() + 0.01;
	MoveBy *= 50;

	QPointF MoveTo = pItem->pos() + MoveBy;
	QMetaObject::invokeMethod(pItem, "AnimateMoveTo", Q_ARG(QPointF, MoveTo), Q_ARG(int, 200));

	// test to iterate the placing
	connect(pItem, SIGNAL(FinishedAnimation()), this, SLOT(Iterate()), Qt::UniqueConnection);
}

void GGrasshopperPlacerAgent::Iterate()
{
	start();
// 	GAgentTabletItem* pAgIt = qobject_cast<GAgentTabletItem*>(sender());
// 	if(pAgIt) {
// 		MoveAwayFromSiblings(pAgIt);
// 	}
}

void GGrasshopperPlacerAgent::run()
{
	foreach(GAgentTabletItem* pItem, ListItemsToBePlaced(0 /*all*/)) {
		MoveAwayFromSiblings(pItem);
	}
}
