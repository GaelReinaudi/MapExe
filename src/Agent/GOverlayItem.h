#ifndef GOVERLAYITEM_H
#define GOVERLAYITEM_H

#include <QGraphicsRectItem>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include "GCloseButtonItem.h"
class GAgentWrapitem;
/////////////////////////////////////////////////////////////////////
//! \brief The docccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
/*!
*/
class GOverlayItem : public QObject, public QGraphicsRectItem//, public QGraphicsLinearLayout
{

public:
    GOverlayItem(GAgentWrapitem* parentContainerItem);
    virtual ~GOverlayItem() {}
    void addOverlayButton (QGraphicsItem* pOvBut, int pos);

protected:
    //QGraphicsLinearLayout* m_pLinearLayout;
    //QGraphicsWidget* m_pLayoutWidget;
    double parentItemHeight;
    double buttonHeightMax;
    double marginWidth;
    //int numCurrentButtons;
private:
	
};

#endif // GOVERLAYITEM_H
