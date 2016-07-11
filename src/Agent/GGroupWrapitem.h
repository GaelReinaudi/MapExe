#ifndef GGROUPWRAPITEM_H
#define GGROUPWRAPITEM_H

#include "GAgentWrapitem.h"

/////////////////////////////////////////////////////////////////////
//! \brief The docccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
/*!
*/
class GGroupWrapitem : public GAgentWrapitem
{
	Q_OBJECT

public:
	GGroupWrapitem(GAgent* pAgent, QGraphicsItem* pParentItem);
	~GGroupWrapitem();

//protected:
	//! implemented in order to add agent to this group
	virtual void Event_DroppedAgent( GAgentWrapitem* pAgWrap );
	//! Re-implemented
	void PopulateSettings( QSettings& inQsettings );
	//! Re-implemented
	void InterpretSettings( QSettings& fromQsettings );

private:
	
};

#endif // GGROUPWRAPITEM_H
