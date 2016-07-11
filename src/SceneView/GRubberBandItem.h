#ifndef GRUBBERBANDITEM_H
#define GRUBBERBANDITEM_H

#include <QGraphicsRectItem>
#include "Agent/GContentGraphicsItem.h"
class GRubberBandStemCellAgent;

/////////////////////////////////////////////////////////////////////
//! \brief The GRubberBandItem class implements a very special kind of QGraphicsRectItem.
/*!
The GRubberBandItem is a specialized rubber band that does more than just item selection.

In its first version, on mouse release, but before clicking again to "drag" the selected
items, one can now press a key and a TextPlugin GTextGraphicsItem will be created, whose
size is the size of the QGraphicsRectItem created by the rubber band.

More generally, though, we are taking advantage of the "free moment for action" that comes
after mouse release right after rubber band creation, but before clicking again to drag the selected items.
Eventually, all kinds of other content could be created inside of this "action window". The traditional
action - clicking again to drag the selected items - would be just one of many potential
actions that can be performed to make content out of the rubber band.
*/
class GRubberBandItem : public QObject, public GContentGraphicsItem<QGraphicsRectItem, GRubberBandStemCellAgent>
{
	Q_OBJECT

public:
    GRubberBandItem(GRubberBandStemCellAgent* pAgent, GAgentWrapitem* parentItem = 0);
    ~GRubberBandItem();

protected:
    
signals:
    
};

#endif // GRUBBERBANDITEM_H
