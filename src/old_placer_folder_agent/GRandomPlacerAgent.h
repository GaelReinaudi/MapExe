#ifndef GRANDOMPLACERAGENT_H
#define GRANDOMPLACERAGENT_H

#include "GPlacerAgent.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GRandomPlacerAgent class is a GAgent class that takes care of placing items randomly within a parent item.
/*!
*/
class GRandomPlacerAgent : public GPlacerAgent
{
	Q_OBJECT

public:
	GRandomPlacerAgent(QObject *parent);
	~GRandomPlacerAgent();

protected:
	//! Re-implemented to randomizes the position of the item in its parent.
	void AddItemToPlace(GAgentTabletItem* pItem, int remainingItemsToBeAdded = 0);

private:
	
};

#endif // GRANDOMPLACERAGENT_H
