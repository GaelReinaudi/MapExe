#include "GForcePlacerAgent.h"
#include <QVector2D>
#include <qmath.h>

GForcePlacerAgent::GForcePlacerAgent(QObject *parent)
	: GPlacerAgent(parent)
{
// 	// connection to start the separate thread
// 	connect(this, SIGNAL(NoMoreItemsToAddForNow(GAgentTabletItem*)), this, SLOT(start()));
}

GForcePlacerAgent::~GForcePlacerAgent()
{
//	wait();
	terminate();
}

void GForcePlacerAgent::AddItemToPlace( GAgentTabletItem* pItemToPlace, int remainingItemsToBeAdded /*= 0*/ )
{
	GPlacerAgent::AddItemToPlace(pItemToPlace, remainingItemsToBeAdded);
	connect(pItemToPlace, SIGNAL(RectGrewNoticably(QRectF)), this, SLOT(start()), Qt::QueuedConnection);
}

void GForcePlacerAgent::run()
{
// 	msleep(1000);
	bool AllForceZero = false;
	while(!AllForceZero) 
	{
		AllForceZero = true;
		// for each parent that has items to be placed
		foreach(GAgentTabletItem* pItem, ListItemsToBePlaced(0)) {
            QVector2D force = Force(pItem);
            if(!force.isNull())
                AllForceZero = false;
			QPointF curPos = pItem->pos();
			QPointF moveTo = curPos;
            force *= 0.5;
            moveTo += force.toPointF();
			QMetaObject::invokeMethod(pItem, "MoveTo", Q_ARG(QPointF, moveTo));
		}
		msleep(20);
	}
}

QVector2D GForcePlacerAgent::Force( GAgentTabletItem* pItem )
{
	QVector2D force;
	if(!pItem)
		return force;
	GAgentTabletItem* pParentItem = pItem->ParentAgentGraphicsItem();
	if(!pParentItem)
		return force;
	QSet<GAgentTabletItem*> siblings = pItem->SiblinGAgentTabletItems();
	if(siblings.isEmpty())
		return  force;

 	QPointF itPos = pItem->boundingRect().center();
	QVector2D shortMove(1e100, 1e100);
	QVector2D shortMoveWithColiding;
	foreach(GAgentTabletItem* pOtherItem, siblings) {
		QVector2D ItemShortMove = ShortestMoveForEdgeToEdge(pItem, pOtherItem, TypicalSpacingOnScreen * 4.0);
		if(ItemShortMove.lengthSquared() < shortMove.lengthSquared())
			shortMove = ItemShortMove;
 		if(pItem->collidesWithItem(pOtherItem))
			shortMoveWithColiding += ItemShortMove;
	}
 	force += shortMove + shortMoveWithColiding;
//	force /= qMax(1, siblings.count());
	double limit = 1.0;
	if(force.length() < limit)
		return QVector2D();
	return force;
}

QVector2D GForcePlacerAgent::ShortestMoveForEdgeToEdge(GAgentTabletItem* pIt1ToMove, GAgentTabletItem* pIt2, double InflateRect1 /*= 0.0*/)
{
	QVector2D shortMove;
	if(!pIt1ToMove || !pIt2)
		return shortMove;
	// rectangles in the same coordinate system
	QRectF r1 = pIt1ToMove->boundingRect().adjusted(-InflateRect1, -InflateRect1, InflateRect1, InflateRect1);
	QRectF r2 = pIt1ToMove->mapRectFromItem(pIt2, pIt2->boundingRect());
	// left edge 1 to right edge 2
	double leftright = r2.right() - r1.left();
	// right edge 1 to left edge 2
	double rightleft = r2.left() - r1.right();
	// top edge 1 to bottom edge 2
	double topbottom = r2.bottom() - r1.top();
	// bottom edge 1 to top edge 2
	double bottomtop = r2.top() - r1.bottom();

	// what is the minimal
	double ABSleftright = abs(leftright);
	double ABSrightleft = abs(rightleft);
	double ABStopbottom = abs(topbottom);
	double ABSbottomtop = abs(bottomtop);
	double minH = qMin(ABSleftright, ABSrightleft);
	double minV = qMin(ABStopbottom, ABSbottomtop);
	// if opposite sign, it means that the items overlap 
	if(leftright * rightleft < 0.0 && topbottom * bottomtop < 0.0) {
		if(minH < minV) {
			if(ABSleftright < ABSrightleft)
				shortMove.setX(leftright);
			else
				shortMove.setX(rightleft);
		}
		else {
			if(ABStopbottom < ABSbottomtop)
				shortMove.setY(topbottom);
			else
				shortMove.setY(bottomtop);
		}
	}
	// same sign means no overlap
	if(leftright * rightleft >= 0.0) {
 		if(ABSleftright < ABSrightleft)
			shortMove.setX(leftright);
		else
			shortMove.setX(rightleft);
	}
	if(topbottom * bottomtop >= 0.0) {
		if(ABStopbottom < ABSbottomtop)
			shortMove.setY(topbottom);
		else
			shortMove.setY(bottomtop);
	}
	return shortMove;
}
