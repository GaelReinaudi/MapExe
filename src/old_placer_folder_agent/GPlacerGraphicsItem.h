#ifndef GPLACERGRAPHICSITEM_H
#define GPLACERGRAPHICSITEM_H

#include "GAgentTabletItem.h"
#include "GPlacerAgent.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GPlacerGraphicsItem class defines a GAgentTabletItem that will help the placer to hook into signals that trigger the placing.
/*!
Technically, it gathers information about its siblings, i.e. the children of its parent graphics item (except itself). 
*/
class GPlacerGraphicsItem : public GAgentTabletItem
{
	Q_OBJECT

public:
	GPlacerGraphicsItem(GPlacerAgent* pAgent, GAgentTabletItem *parentItem = 0);
	~GPlacerGraphicsItem();

	//! returns the actual placer agent of this item. 0 if something goes wrong.
	GPlacerAgent* PlacerAgent() {return qobject_cast<GPlacerAgent*>(Agent());}

protected slots:
	//! Called when a sibling agent graphics item has been added. 
	virtual void Event_SiblingAgentItemAdded(GAgentTabletItem* pAddedSiblingAgentItem, int remainingItemsToBeAdded = 0);

signals:
	//! Emitted when remainingItemsToBeAdded == 0 in Event_SiblingAgentItemAdded().
	void SiblingItemsAdded();//(const QList<GAgentTabletItem*> & siblings);

private:
	
};

#endif // GPLACERGRAPHICSITEM_H
