#ifndef GREPULSIONPLACERAGENT_H
#define GREPULSIONPLACERAGENT_H

#include "GPlacerAgent.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GGrasshopperPlacerAgent class is a GAgent class that takes care of placing items by pushing them away from siblings.
/*!
*/
class GGrasshopperPlacerAgent : public GPlacerAgent
{
	Q_OBJECT

public:
	GGrasshopperPlacerAgent(QObject *parent);
	~GGrasshopperPlacerAgent();

protected:
	//! Re-implemented for testing the multi-threading ability of the agent on the graphics items
	void run();

private slots:
	//! Moves the item away from its overlapping siblings
	void MoveAwayFromSiblings(GAgentTabletItem* pItem, int remainingItemsToBeAdded = 0);
	//! test. For iterating the repulsion mechanism many times until no more collision
	void Iterate();

private:
	
};

#endif // GREPULSIONPLACERAGENT_H
