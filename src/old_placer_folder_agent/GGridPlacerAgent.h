#ifndef GGRIDPLACERAGENT_H
#define GGRIDPLACERAGENT_H

#include "GPlacerAgent.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GGridPlacerAgent class is a GAgent class that takes care of placing items as a grid within a parent item.
/*!
*/
class GGridPlacerAgent : public GPlacerAgent
{
	Q_OBJECT

public:
    GGridPlacerAgent(QObject *parent);
    ~GGridPlacerAgent();

protected:
    //! Re-implemented.
    void AddItemToPlace(GAgentTabletItem* pItem, int remainingItemsToBeAdded = 0);

private:
    int m_TotalItems;
	int m_NumColumns;
};

#endif // GGRIDPLACERAGENT_H
