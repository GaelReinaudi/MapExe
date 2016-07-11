#include "GPlacerAgent.h"
#include "GPlacerGraphicsItem.h"

GPlacerAgent::GPlacerAgent(QObject *parent)
	: GAgent(parent)
{

}

GPlacerAgent::~GPlacerAgent()
{

}

GAgentGraphicsItem* GPlacerAgent::ProvideNewAgentGraphicsItem( GAgentTabletItem* pParentItem )
{
	// provides an item that will connect events from the pParentItem
	GAgentTabletItem* pGrpIt = new GPlacerGraphicsItem(this, pParentItem);
	return pGrpIt;
}

/////////////////////////////////////////////////////////////////////
/*!
The default implementation updates a list of all the items that were added and that can be used later to place them.
When remainingItemsToBeAdded == 0, the signal NoMoreItemsToAddForNow() is emitted.
\param:  GAgentTabletItem * pItemToPlace : the item that was added to the Agent's graphics item as a child.
\param:  int remainingItemsToBeAdded : estimation of items that are still to be added when many are added at once.
*////////////////////////////////////////////////////////////////////
void GPlacerAgent::AddItemToPlace(GAgentTabletItem* pItemToPlace, int remainingItemsToBeAdded /*= 0*/)
{
	if(!pItemToPlace)
		return;
	// the parent item
	GAgentTabletItem* pParentItem = pItemToPlace->ParentAgentGraphicsItem();
	m_HashParent_ItemsToBePlaced.insert(pParentItem, pItemToPlace);
// 	if(!remainingItemsToBeAdded)
// 		emit NoMoreItemsToAddForNow(pParentItem);
}

///////////////////////////////////////////////////////////////////
/*!
The default implementation.
\param:  GAgentTabletItem * pInWhichParentItem : the item that parents the placer graphics item and the siblings that were added.
*////////////////////////////////////////////////////////////////////
// void GPlacerAgent::Event_SiblingItemsAdded( GAgentTabletItem* pInWhichParentItem )
// {
// 	start();
// }

QList<GAgentTabletItem*> GPlacerAgent::ListItemsToBePlaced( GAgentTabletItem* inWhichParent /*= 0*/) const
{
	if(inWhichParent)
		return m_HashParent_ItemsToBePlaced.values(inWhichParent);
	return m_HashParent_ItemsToBePlaced.values();
}

void GPlacerAgent::FinishedPlacingItems( GAgentTabletItem* inWhichParent )
{
	m_HashParent_ItemsToBePlaced.remove(inWhichParent);
}
