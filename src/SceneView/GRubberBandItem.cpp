#include "GRubberBandItem.h"
#include "agent.h"
#include "Agent/GAgentManager.h"
#include "SceneView/GMapGraphicsScene.h"
#include "Agent/GStemCellAgent.h"
#include "Agent/GAgentWrapitem.h"

#include <QDebug>
#include <QBrush>
#include <QGraphicsScene>
#include <QKeyEvent>

GRubberBandItem::GRubberBandItem(GRubberBandStemCellAgent* pAgent, GAgentWrapitem* parentItem)
    : GContentGraphicsItem<QGraphicsRectItem, GRubberBandStemCellAgent>(pAgent, parentItem)
    , QObject(0)
{
    setZValue(-1);
	setFlag(QGraphicsItem::ItemIsFocusable);
    setBrush(QBrush(QColor(50,50,50,50)));
}

GRubberBandItem::~GRubberBandItem()
{

}

