#ifndef GPLACERAGENT_H
#define GPLACERAGENT_H

#include "GAgent.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GPlacerAgent class is a class that defines an agent that places items
/*!
Technically, it places its siblings, i.e. the children of its parent (except itself). 
When a GPlcerAgent object is added to an agent (as a dhild) it will listen for events from that agent 
and trigger placing.
*/
class GPlacerAgent : public GAgent
{
	Q_OBJECT

public:
	GPlacerAgent(QObject *parent);
	~GPlacerAgent();

	//! Re-implemented. Returns a new graphics item that will hook into the parent graphics item's ChildAgentGraphicsItemAdded() signal.
	virtual GAgentGraphicsItem* ProvideNewAgentGraphicsItem(GAgentTabletItem* pParentItem);

protected:
	//! Re-implemented.
	void PopulateAgentItem(GAgentTabletItem* pTabletItem) {}

protected:
	//! For a given inWhichParent, returns the list of items to be placed in it that can be used to transmit informations to the separate thread that tries to places the items.
	QList<GAgentTabletItem*> ListItemsToBePlaced(GAgentTabletItem* inWhichParent = 0) const;
	//! Called by the item when a child item is to be placed. The remainingItemsToBeAdded may be provided to know how many more items are expected to be added very very soon.
	virtual void AddItemToPlace(GAgentTabletItem* pItemToPlace, int remainingItemsToBeAdded = 0);
	//! Must be called when the placer has finished a job of placing items in a given parent. Default implementation removes the items from the ListItemsToBePlaced().
	virtual void FinishedPlacingItems(GAgentTabletItem* inWhichParent);
	
protected slots:
// 	//! Called by the GPlacerGraphicsItem`s of this placer when a batch of items were added as siblings and no more are expected from that batch (usually a good time to trigger the placing).
// 	virtual void Event_SiblingItemsAdded(GAgentTabletItem* pInWhichParentItem);

// signals:
// 	//! Emitted when remainingItemsToBeAdded == 0 in AddItemToPlace(). The argument inWhichParent is the parent that corresponds to this signal being emitted.
// 	void NoMoreItemsToAddForNow(GAgentTabletItem* inWhichParent);

private:
	//! Hash that contains, for a given parent item the list of items to place in it. This can be used to transmit informations to the separate thread that tries to places the items.
	QMultiHash<GAgentTabletItem*, GAgentTabletItem*> m_HashParent_ItemsToBePlaced;

	friend class GPlacerGraphicsItem;
};

#endif // GPLACERAGENT_H
