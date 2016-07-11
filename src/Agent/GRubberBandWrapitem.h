#ifndef GDYNRUBBERITEM_H
#define GDYNRUBBERITEM_H

#include "agent.h"
#include "./SceneView/GRubberBandItem.h"

/////////////////////////////////////////////////////////////////////
//! \brief The docccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
/*!
*/
class GRubberBandWrapitem : public GAgentWrapitem
{
	Q_OBJECT

public:
    GRubberBandWrapitem(GAgent* pAgent, QGraphicsItem* pParentItem);
    ~GRubberBandWrapitem() {}
	GRubberBandItem* RubberItem() {
		return static_cast<GRubberBandItem*>(ContentItem());
	}
	//! indicates if the rubber band was in fact grown by MoveEndPoint().
	bool DidGrow() const { return m_DidGrow; }

	//! Sets the rubber band to be fixed, i.e. can't be grown anymore
	void SetFixed();
    void CreateGroup();
	//! Moves the corner of the rubber band.
	void MoveEndPoint( QPointF cornerPos );

protected:
	void keyPressEvent(QKeyEvent * event);

private:
	//! tells us if this rubber band should not be resized any more
	bool m_IsFixed;
	//! tells us if this rubber band did grow. Is used to 
	bool m_DidGrow;
};

#endif // GDYNRUBBERITEM_H
