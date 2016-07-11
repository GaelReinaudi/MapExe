#include "GRandomPlacerAgent.h"
#include "GAgentTabletItem.h"
#include <qmath.h>

GRandomPlacerAgent::GRandomPlacerAgent(QObject *parent)
	: GPlacerAgent(parent)
{

}

GRandomPlacerAgent::~GRandomPlacerAgent()
{

}

void GRandomPlacerAgent::AddItemToPlace(GAgentTabletItem* pItem, int remainingItemsToBeAdded /*= 0*/)
{
	if(!pItem)
		return;
	GAgentTabletItem* pParent = pItem->ParentAgentGraphicsItem();
	if(!pParent)
		return;
	// the bounding rect of the parent
	double distanceAverageItems = 20.0;
	double squaresize = qSqrt(remainingItemsToBeAdded) * distanceAverageItems;
	QRectF squareForDistanceAverage = QRectF(0.0, 0.0, squaresize, squaresize);
	QRectF parentRect = pParent->boundingRect() | pParent->childrenBoundingRect() | squareForDistanceAverage;
	// rand gives a number between 0 and RAND_MAX
	// this is a double between 0 and 1
	double x = double(qrand()) / double(RAND_MAX);
	double y = double(qrand()) / double(RAND_MAX);
	// we make a point in the parentRect
	x *= parentRect.width();
	y *= parentRect.height();
	x += parentRect.x();
	y += parentRect.y();
	pItem->setPos(x,y);

	FinishedPlacingItems(pParent);
}
