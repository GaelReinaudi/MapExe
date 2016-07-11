#ifndef GDYNAMICCONTAINERITEM_H
#define GDYNAMICCONTAINERITEM_H

#include "../../../LabExe/src/ToolBox/GSerializable.h"
#include <QGraphicsRectItem>
#include <QGraphicsLayoutItem>
#include <QPainter>
#include <qmath.h>

//#define VISUAL_DEBUG

//! generates a random number between 0 and 255
static int rand255() { return qrand() % 256; }
//! returns a random QColor with fixed transparency 177
static QColor randRGB() { return QColor(rand255(),rand255(),rand255()); }

//! Typical size used to scale all of the items to a reference.
static QSizeF TypicalScreenSize(800, 600);
static QRectF TypicalScreen(QPointF(0.0, 0.0), TypicalScreenSize);
//static double TypicalSpacingOnScreen(100.0);
/////////////////////////////////////////////////////////////////////
//! \brief The GDynamicContainerItem class is intended to be a extremely lightweight item class that can wrap complex content 
/*!
A huge number of such item could be visible on a map, and the level of detail should then be close to zero. 
As the content needs to get more detailed (because we zoom in), the GDynamicContainerItem instance can query content 
and cache various pixmap version of the item (for when we zoom out again).
*/
template <typename DerivedContainer, typename ContentProvider>
class GDynamicContainerItem : public QObject, public QGraphicsRectItem, public GSerializable, QGraphicsLayoutItem
{
public:
	GDynamicContainerItem(QGraphicsItem* pParentItem, const QString & uniqueID = "") 
		: QGraphicsRectItem(pParentItem)
		, GSerializable(uniqueID)
		, m_DisplayStatus(Nothing)	{}
	virtual ~GDynamicContainerItem() {}

	enum { Type = UserType + 1 };
	//! Re-implemented // Enable the use of qgraphicsitem_cast with this item.	
	virtual int type() const { return Type; }

	//! Re-implemented 
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented 
	virtual void InterpretSettings(QSettings& fromQsettings);

	//! Re-implemented
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget * widget = 0){
		Q_UNUSED(widget);	Q_UNUSED(option);
#ifdef VISUAL_DEBUG
		painter->setPen(randRGB());
		painter->setBrush(randRGB());
		painter->drawRect(Rect());
#endif
	}

	//! Must return the provider that can make a new item when needed
	virtual ContentProvider* Provider() const = 0;
	//! Must retrieve (find or create) the ContentProvider that this item will use to interact with. Returns true if the provider could be retrieved or made and that the initializing data were successfully accepted.
	virtual bool ReMakeProvider() const = 0;

protected:
	//! Implemented for the QGraphicsLayoutItem
	QSizeF sizeHint(Qt::SizeHint which, const QSizeF & constraint = QSizeF()) const { Q_UNUSED(constraint);Q_UNUSED(which);
		return Rect().size();
	}

public:
	enum DisplayStatus{Nothing, Item, ItemOverlayScreenShot, ScreenShot, Rectangle} m_DisplayStatus;
	QRectF Rect() const { return rect(); }
	void SetRect(const QRectF & val) const { 
		const QRectF oldRect = Rect();
		const_cast<QGraphicsRectItem*>(static_cast<const QGraphicsRectItem*>(this))->setRect(val);
		if(val != oldRect) {
			static_cast<const DerivedContainer*>(this)->Event_RectChanged(val);
		}
	}
public:
	// private:
	//! for now here, the informations the Provider will need to initialize when it specialize in an other class (the one defined by m_TryBeClassName). We try to mimic the QSettings way of retrieving information.
	QVariantList m_TryBeClassInitilizationInfo;	

};

template <typename DerivedContainer, typename ContentProvider>
void GDynamicContainerItem<DerivedContainer, ContentProvider>::PopulateSettings( QSettings& inQsettings )
{
//	GSerializable::PopulateSettings(inQsettings);
	QPointF itPos = pos();
	QRectF itRect = boundingRect();
	inQsettings.setValue("xyltwh", QVariantList()
		<< qRound64(itPos.x())
		<< qRound64(itPos.y())
		<< qRound64(itRect.left())
		<< qRound64(itRect.top())
		<< qRound64(itRect.width())
		<< qRound64(itRect.height())
		);
	// if there is a provider, save the up-to-date data from it
	if(Provider()) {
		inQsettings.setValue("data", Provider()->ProvideInitializationData());
	} // if not, let's save the data we already got
	else {
		inQsettings.setValue("data", m_TryBeClassInitilizationInfo);
	}
// 	inQsettings.setValue("daaaaataaaaaaaaaaa", 
// 		QVariantList()
// 		<< QString("hhhhhhhhhhhhhhhhhhhhhhh")
// 		<< qRound64(itPos.x())
// 		);
}

template <typename DerivedContainer, typename ContentProvider>
void GDynamicContainerItem<DerivedContainer, ContentProvider>::InterpretSettings( QSettings& fromQsettings )
{
//	GSerializable::InterpretSettings(fromQsettings);
	QPointF thePos, theTopLeft;
	QSizeF theSize;
	QVariantList listGeom = fromQsettings.value("xyltwh").toList();
	if(listGeom.count() == 6) {
		thePos.rx() = listGeom[0].toInt();
		thePos.ry() = listGeom[1].toInt();
		theTopLeft.rx() = listGeom[2].toInt();
		theTopLeft.ry() = listGeom[3].toInt();
		theSize.rwidth() = listGeom[4].toInt();
		theSize.rheight() = listGeom[5].toInt();
		setPos(thePos);
		SetRect(QRectF(theTopLeft, theSize));
	}
	m_TryBeClassInitilizationInfo = fromQsettings.value("data").toList();
}

#endif // GDYNAMICCONTAINERITEM_H
