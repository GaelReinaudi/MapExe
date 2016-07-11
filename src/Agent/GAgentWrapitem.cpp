#include "GAgentWrapitem.h"
#include "GAgentManager.h"
#include "SceneView/GUnderlayItem.h"
#include "SceneView/GItemManager.h"

GItemManager* GAgentWrapitem::m_pItemManager = 0;
int zzszzzszszzfdsasg = qRegisterMetaType<GAgentWrapitem*>("GAgentWrapitem*");

GAgentWrapitem::GAgentWrapitem(GAgent* pAgent, QGraphicsItem* pParentItem)
	: GDynamicContainerItem<GAgentWrapitem, GAgent>(pParentItem)
	, m_pSharedProvider(pAgent, &GAgent::Destroy) // this custom deleter will remove the agent from the agent manager in a clean way and the GMapStatsWidget will be happy
	, m_AgentTypeID(pAgent ? pAgent->UniqueSystemID() : "")
	, m_MarkForRendering(false)
	, m_pUnderlay(0)
{
	Init();
	if(m_pSharedProvider) {
		m_pContentItem = QSharedPointer<QGraphicsItem>(m_pSharedProvider->NewContentItem(this));
		setFlag(QGraphicsItem::ItemHasNoContents);
	}
}

GAgentWrapitem::GAgentWrapitem(const QString & theAgentTypeID, QGraphicsItem* pParentItem)
	: GDynamicContainerItem<GAgentWrapitem, GAgent>(pParentItem)
	, m_pSharedProvider()
	, m_AgentTypeID(theAgentTypeID)
	, m_MarkForRendering(false)
	, m_pUnderlay(0)
{
	Init();
}

void GAgentWrapitem::Init()
{
	setFlag(QGraphicsItem::ItemIsFocusable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setAcceptHoverEvents(true);


#ifdef VISUAL_DEBUG
	m_pVdScreenshot = new QGraphicsRectItem(-20, -20, 40, 40, this);
	m_pVdScreenshot->setBrush(Qt::red);
	m_pVdScreenshot->setPos(20, -20);
#endif

	if(m_AgentTypeID.contains("Text"))
		return;
	m_pUnderlay = new GUnderlayItem(this);
}

GAgentWrapitem::~GAgentWrapitem()
{

}

void GAgentWrapitem::PopulateSettings( QSettings& inQsettings )
{
	GDynamicContainerItem<GAgentWrapitem, GAgent>::PopulateSettings(inQsettings);
	inQsettings.setValue("agent", m_AgentTypeID);
}

void GAgentWrapitem::InterpretSettings( QSettings& fromQsettings )
{
	GDynamicContainerItem<GAgentWrapitem, GAgent>::InterpretSettings(fromQsettings);
	m_AgentTypeID = fromQsettings.value("agent", 0).toString();
}

GAgent* GAgentWrapitem::Provider() const
{
	// if there is no pointed agent	try to get it from the agent manager.
	if(!m_pSharedProvider) {
		GAgent* pRetAgent = AgentManagerInstance()->GetAgent(m_AgentTypeID);
		m_pSharedProvider = QSharedPointer<GAgent>(pRetAgent);
	}
	return m_pSharedProvider.data();
}

bool GAgentWrapitem::ReMakeProvider() const
{
	// the type. ex: "GWebPageAgent"
	QString strType = GSerializable::GetTypeFromUniqueTypeID(m_AgentTypeID);
	// remakes the agent and fix its unique ID accordingly to m_AgentTypeID
	GAgent* pRetAgent = AgentManagerInstance()->CreateNewProgAgent(strType, m_AgentTypeID);
	m_pSharedProvider = QSharedPointer<GAgent>(pRetAgent);

	// if now we have it let's try to give it relevant information
	// it might be a stem cell...
	if(pRetAgent) {
		return pRetAgent->InjectInitializationData(m_TryBeClassInitilizationInfo);
	}	
	return false;
}

void GAgentWrapitem::Event_ContentRectChanged(const QRectF & newRect)
{
	SetRect(newRect);
	emit ContentRectChanged(newRect);
}

bool GAgentWrapitem::TryTransform()
{
	if(Provider()->isStemCell()) {
		QVariantList stemClassInit = Provider()->ProvideInitializationData();
		QString strTBCN = stemClassInit.takeFirst().toString();
		m_AgentTypeID = GSerializable::ChangeUniqueTypeID(m_AgentTypeID, strTBCN);
		m_TryBeClassInitilizationInfo = stemClassInit;
		m_MarkForRendering = true;
		return ReMakeProvider();
	}
	return false;
}

void GAgentWrapitem::GettingCloser( int newLevel )
{
	if(!m_MarkForRendering && newLevel > 1) {
		return;
	}

	setFlag(QGraphicsItem::ItemHasNoContents);
	if(m_pContentItem && m_pContentItem.data()) {
		m_pContentItem.data()->show();
		// not painting the wrapitem
		return;
	}
	if(!Agent())
		ReMakeProvider();
	if(Agent())
		m_pContentItem = QSharedPointer<QGraphicsItem>(Agent()->NewContentItem(this));

	if(m_MarkForRendering && m_pContentItem && m_pContentItem.data()) {
		ItemManager()->ScreenShotContent(this, m_pContentItem);
	}
	m_MarkForRendering = false;
}

void GAgentWrapitem::GettingFurther( int newLevel )
{
	switch(newLevel) {
	case 3:
		if(m_pContentItem) {
			// if the item is visible it has probably not been screenshot yet
			if(m_pContentItem.data()->isVisible())
				return GettingFurther(newLevel - 1);
			// no unloading for pdf... for now
			if(Agent() && !QString(Agent()->metaObject()->className()).contains("Pdf"))
				m_pContentItem = QSharedPointer<QGraphicsItem>();
		}
	case 2:
		if(m_pContentItem && m_pContentItem.data()) {
			ItemManager()->ScreenShotContent(this, m_pContentItem);
			m_pContentItem.data()->hide();
		}
		// painting the wrapitem again
		setFlag(QGraphicsItem::ItemHasNoContents, false);
		break;
	}
}

void GAgentWrapitem::Reset()
{
	m_pContentItem = QSharedPointer<QGraphicsItem>();
	int currentLevel = ItemManager()->Level(this);
	GettingCloser(currentLevel);
}

void GAgentWrapitem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget * widget /*= 0*/ )
{
	QPixmap screenSh = ItemManagerInstance()->Pix(this);
	QRect screenShRect = screenSh.rect();
	if(screenSh.isNull()) {
		//painter->setPen(randRGB());
		//painter->setBrush(randRGB());
		//painter->setBrush(QColor(0, 0, 0, 25));
		painter->fillRect(Rect(), QColor(0, 0, 0, 25));
		m_MarkForRendering = true;
 return;
		GDynamicContainerItem<GAgentWrapitem, GAgent>::paint(painter, option, widget);
//		screenShRect = boundingRect().toRect();
		return;
	}
	
#ifndef QT_DEBUGwwww
	painter->drawPixmap(Rect(), screenSh, screenShRect);
#else 
	painter->drawPixmap(QRect(-Rect().width(), 0.0, Rect().width(), Rect().height()), screenSh, screenShRect);
	QPixmap screenDig = ItemManagerInstance()->PixDig(this);
	painter->drawPixmap(QRect(Rect().width(), 0.0, 1000.0, 1000.0), screenDig, screenDig.rect());
	// Image unscaled of the "low-def" screenshots
	painter->drawPixmap(QRect(-screenShRect.width(), -screenShRect.height(), screenShRect.width(), screenShRect.height()), screenSh, screenShRect);
#endif
}

void GAgentWrapitem::SetPreferredSize(const QSizeF & theSize)
{
	SetSize(theSize);
	emit PreferredRectChanged(Rect());
}

void GAgentWrapitem::Event_DroppedAgent( GAgentWrapitem* pAgWrap )
{
	//throw std::exception("The method or operation is not implemented.");
}

void GAgentWrapitem::ShowOverlay()
{
	DeleteOverlay();
	GOverlayItem* pOL = new GOverlayItem(this);
	GAgent* pAg = Agent();
	if(pAg)
		pAg->PopulateOverlay(pOL);
	// shine in the overlay
	if(!m_pUnderlay) {
		qDebug() << "No underlay";
		return;
	}
	m_pUnderlay->ShineIn();
}

void GAgentWrapitem::DeleteOverlay()
{
	GOverlayItem* pOL = findChild<GOverlayItem*>("");
	if(pOL)
		delete pOL;
	// shine in the overlay
	if(!m_pUnderlay) {
		return;
	}
	m_pUnderlay->ShineOut();
}

GItemManager* GAgentWrapitem::ItemManager()
{
	if(!m_pItemManager) {
		m_pItemManager = new GItemManager(0);
		connect(qApp, SIGNAL(aboutToQuit()), m_pItemManager, SLOT(deleteLater()));
	}
	return m_pItemManager;
}

void GAgentWrapitem::DeleteItemManager()
{
	if(m_pItemManager) {
		delete m_pItemManager;
	}
}
