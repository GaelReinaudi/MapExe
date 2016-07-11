#ifndef GFIREWORKSPLACERAGENT_H
#define GFIREWORKSPLACERAGENT_H

#include "GPlacerAgent.h"

//! \brief The GFireworksPlacerAgent class is awesome.
/*!
The GFireworksPlacerAgent class is a GAgent class that takes care of placing items in a fireworks-like fashion. Once a new folder is crawled, it is launched out of its parent folder. When it stops, it "pops", i.e. it is populated by its children items. The direction in which it is launched is based statistically off of the direction in in which its parent folder was launched. The distance it is launched is estimated by the number of children items it has.
*/
class GFireworksPlacerAgent : public GPlacerAgent
{
	Q_OBJECT

public:
	GFireworksPlacerAgent(QObject *parent);
	~GFireworksPlacerAgent();

protected:
    void AddItemToPlace(GAgentTabletItem* pItemToPlace, int remainingItemsToBeAdded = 0);

private slots:
    void Fire();
private:
	
};

#endif // GFIREWORKSPLACERAGENT_H
