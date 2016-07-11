#ifndef GDYNAMICAGENTITEM_H
#define GDYNAMICAGENTITEM_H
#include "labexegraphicsmap_global.h"

#include "../../../LabExe/src/ToolBox/GSerializable.h"
#include "SceneView/GDynamicContainerItem.h"
#include "GAgent.h"
#include "GOverlayItem.h"
#include "GCloseButtonItem.h"
class GItemManager;
class GUnderlayItem;

/////////////////////////////////////////////////////////////////////
//! \brief The GAgentWrapitem is a GDynamicContainerItem that manages content from an agent
/*!
*/
class LABEXEGRAPHICSMAP_EXPORT GAgentWrapitem : public GDynamicContainerItem<GAgentWrapitem, GAgent>
{
    Q_OBJECT

public:
	GAgentWrapitem(GAgent* pAgent, QGraphicsItem* pParentItem);
	GAgentWrapitem(const QString & theAgentTypeID, QGraphicsItem* pParentItem);
	~GAgentWrapitem();
	void Init();

	enum { Type = UserType + 2 };
	//! Re-implemented // Enable the use of qgraphicsitem_cast with this item.
	virtual int type() const { return Type; }
	//! Re-implemented
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget * widget = 0);

public:
	//! Re-implemented 
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented 
	virtual void InterpretSettings(QSettings& fromQsettings);
	//! For convenience, same as Provider()
	GAgent* Agent() const {return Provider();}
	//! Implemented, same as Agent()
	GAgent* Provider() const;
	QGraphicsItem* ContentItem() const { return m_pContentItem.data(); }
	//! Re-implemented
	QRectF boundingRect() const {
		if(Rect().isNull()) {
			QRectF contRect = m_pContentItem ? ContentItem()->boundingRect() : QRectF();
			SetRect(contRect.adjusted(0.5, 0.5, -0.5, -0.5));
			if(Rect().isNull())
				return QRectF(-500.0, -200.0, 1000.0, 400.0);
		}
		return Rect();
	}

	//! Implemented
	//! Sets the size that should be considered as preferred for the content. The default resizes this Wrapitem and emits the signal PreferedRectChanged()
	virtual void SetPreferredSize(const QSizeF & theSize);

public slots:
	//! Re-implemented 
	void GettingCloser(int newLevel);
	//! Re-implemented 
	void GettingFurther(int newLevel);
	//! Resets this container by releasing any reference to a Provider() or a Content(). Then it UpdateDistanceLevel() to its current value.
	void Reset();
	virtual bool ReMakeProvider() const;
	//! Tries to transform this stem cell into a grown agent defined by the AgentTypeToBe() 
	bool TryTransform();

protected:
	//! Re-implemented
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
		QGraphicsItem::hoverEnterEvent(event);
		ShowOverlay();
	}
	//! Re-implemented
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
		QGraphicsItem::hoverLeaveEvent(event);
		DeleteOverlay();
	}
	//! Makes an overlay appear
	virtual void ShowOverlay();
	//! Destroys the overlay
	virtual void DeleteOverlay();


public:
	//! Called by the content when the bounding rect changed
	void Event_ContentRectChanged(const QRectF & newRect);
	//! Called when the rect of this Container changed. Default does nothing.
	void Event_RectChanged( const QRectF & val ) const {
		Q_UNUSED(val);
	}

protected:
	//! resizes this wrapitem. Default keeps the upper left corner fixed.
	virtual void SetSize( const QSizeF & theSize ) {SetRect(QRectF(Rect().topLeft(), theSize));}
public:
	//! should be used in the future to react when items are dropped in this wrap's boundaries
	virtual void Event_DroppedAgent( GAgentWrapitem* pAgWrap );

signals:
	//! Emitted from within Event_ContentRectChanged()
	void ContentRectChanged(const QRectF & newRect) const;
	//! Emitted from SetPreferredSize() in case someone (like the content) wants to use the preferred rect information
	void PreferredRectChanged(const QRectF & newRect) const;

private:
	//! Unique ID of the agent or "" if not defined yet
	QString m_AgentTypeID;
	//! The provider corresponding to this item. A QSharedPointer<T> is a very important piece of tool. See the Google doc about Optimization!
	mutable QSharedPointer<GAgent> m_pSharedProvider;
	// test for a single content item which would make sens since even a group should be grouped
	QSharedPointer<QGraphicsItem> m_pContentItem;

	bool m_MarkForRendering;
	GUnderlayItem* m_pUnderlay;

	//! global pointer to the screenshot manager
	static GItemManager* m_pItemManager;
public:
	static GItemManager* ItemManager();
	void DeleteItemManager();

#ifdef VISUAL_DEBUG
public:
	QGraphicsRectItem* m_pVdScreenshot;
	void VdRotate() {m_pVdScreenshot->rotate(9);}
#endif

	friend class GItemManager;
};

#endif // GDYNAMICAGENTITEM_H
