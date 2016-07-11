#ifndef GGROUPITEM_H
#define GGROUPITEM_H

#include <QGraphicsItemGroup>
#include "Agent/GContentGraphicsItem.h"
class GGroupAgent;

/////////////////////////////////////////////////////////////////////
//! \brief The GGroupItem class implements a very special kind of .
/*!
*/
class GGroupItem : public QObject, public GContentGraphicsItem<QGraphicsItemGroup, GGroupAgent>
{
	Q_OBJECT

public:
	GGroupItem(GGroupAgent* pAgent, GAgentWrapitem* parentItem = 0);
	~GGroupItem();

	//! Re-implemented
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget * widget = 0) {
		painter->setPen(randRGB());
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(boundingRect());
	}

protected slots:
	void Event_DroppedAgent(GAgentWrapitem* pAgWrap);

private:
	
};

#endif // GGROUPITEM_H
