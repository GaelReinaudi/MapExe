#include "GMapGraphicsScene.h"
#include <QtGui>
#include <QFontDatabase>
#include <QImage>
#include "Agent/GStemCellAgent.h"
#include "Agent/GAgentWrapitem.h"
#include "Agent/GAgentManager.h"
#include "GRubberBandWrapitem.h"

GMapGraphicsScene::GMapGraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
	, m_pRootItem(new QGraphicsPixmapItem(QPixmap(":/crosshair.png"), 0))
	, m_pDragStemCellAgent(0)
	, m_pDraggedAgentGraphItem(0)
	, m_pRubberBandAgent(0)
    , m_pRubberWrap(0)
	, m_ClickedItem(0)
{
	m_pRootItem->setOffset(-500/2, -500/2);
	// try optimization
	// this might make the scene thread-safe: https://bugreports.qt-project.org/browse/QTBUG-2534
	setItemIndexMethod(QGraphicsScene::NoIndex);
	addItem(RootGraphicsItem());
}

GMapGraphicsScene::~GMapGraphicsScene()
{
}

void GMapGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent * event)
{
	QGraphicsScene::dragEnterEvent(event);
	// let's make a stem cell agent that can latter be converted to a specific type if possible.
	// that line doesn't work !
	QWidget* sourceWid = event->source();
	// TEMP. TRICK FOR NOW
	m_pDragStemCellAgent = new GDraggedInStemCellAgent(this, /*trick for now*/ views().first()); 
	// feed the stem cell with the information we get from the MIME data.
	// It is the stem cell's responsibility to make good use of this
	m_pDragStemCellAgent->InjectMimeData(event->mimeData());

	// test for logging user actions
	LogUserAction("DragEnterEvent");

	// make a graphics item that follows the 
	QPointF posDrop = event->scenePos();
	m_pDraggedAgentGraphItem = 0;
	if(m_pDragStemCellAgent) {
		m_pDraggedAgentGraphItem = m_pDragStemCellAgent->ProvideNewAgentWrappingItem(this);
		if(!m_pDraggedAgentGraphItem)
			return;
		m_pDraggedAgentGraphItem->setPos(posDrop);
	}
}

void GMapGraphicsScene::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{
//	// TO FIX: this forwards the latter to come drop event to the stem cell item and hence the scene refuses it
// 	QGraphicsScene::dragMoveEvent(event);

	if(!m_pDraggedAgentGraphItem)
		return;
	QPointF posDrag = event->scenePos();
	m_pDraggedAgentGraphItem->setPos(posDrag);
}

void GMapGraphicsScene::dragLeaveEvent( QGraphicsSceneDragDropEvent * event )
{
	QGraphicsScene::dragLeaveEvent(event);

	if(m_pDragStemCellAgent) {
		m_pDragStemCellAgent = 0;
	}
	if(m_pDraggedAgentGraphItem) {
		delete m_pDraggedAgentGraphItem;
		m_pDraggedAgentGraphItem = 0;
	}
}

void GMapGraphicsScene::dropEvent( QGraphicsSceneDragDropEvent * event )
{
	QGraphicsScene::dropEvent(event);

	// temp for cleaning the unused stem cell agent. but will be soon used to produce such an agent
	if(m_pDragStemCellAgent) {
		if(m_pDraggedAgentGraphItem) {
			if(m_pDraggedAgentGraphItem->TryTransform()) {
				m_pDraggedAgentGraphItem->Reset();
//				m_pDraggedAgentGraphItem->ShowContent();
			}
			m_pDragStemCellAgent = 0;
			m_pDraggedAgentGraphItem = 0;
		}
	}
}

void GMapGraphicsScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
    QGraphicsScene::mouseDoubleClickEvent(event);
    QGraphicsItem* pAgentGraphItem = 0;
	if(QGraphicsItem *item = itemAt(event->scenePos())) {
// 		GAgentWrapitem* pAgIt = qobject_cast<GAgentWrapitem*>(dynamic_cast<QObject*>(item));
// 		if(pAgIt) {
// 			pAgIt->mouseDoubleClickEvent(event);
// 		}
	}
}

void GMapGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	// only left clicks for now
	if(event->button() != Qt::LeftButton) {
		return;
	}

	m_ClickedItem = itemAt(event->scenePos());

	// if there is a rubber band and we clicked out of it, we destroy it
	if(m_pRubberWrap && !m_pRubberWrap->contains(m_pRubberWrap->mapFromScene(event->scenePos()))) {
        delete m_pRubberWrap;
        m_pRubberWrap = 0;
		delete m_pRubberBandAgent;
		m_pRubberBandAgent = 0;
	}
}

void GMapGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
	// only left clicks for now
	if(event->buttons() != Qt::LeftButton) {
		return;
	}

	if(!m_ClickedItem && !m_pRubberBandAgent && !m_pRubberWrap) {
		// make a graphics item that follows the 
		QPointF posMove = event->scenePos();
		m_pDraggedAgentGraphItem = 0;
		// TEMP. TRICK FOR NOW
		m_pRubberBandAgent = new GRubberBandStemCellAgent(this, /*trick for now*/ views().first()); 
		m_pRubberWrap = m_pRubberBandAgent->ProvideNewRubberBandWrappingItem(0);
		m_pRubberWrap->show();
		AddItem(m_pRubberWrap);
		m_pRubberWrap->setPos(posMove);
		m_pRubberWrap->setZValue(-1.0);
	}

	if(m_pRubberWrap) {
		m_pRubberWrap->MoveEndPoint(event->scenePos());
	}
}

void GMapGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if(m_pRubberWrap) {
        m_pRubberWrap->SetFixed();
		m_pRubberWrap->setFocus();
        if(m_pRubberWrap->rect().size().isNull()) {
			delete m_pRubberWrap;
			m_pRubberWrap = 0;
			delete m_pRubberBandAgent;
			m_pRubberBandAgent = 0;
        }
    }
}

void GMapGraphicsScene::PopulateSettings( QSettings& inQsettings )
{
	delete m_pRubberWrap;
	m_pRubberWrap = 0;
	delete m_pRubberBandAgent;
	m_pRubberBandAgent = 0;
	// top level agent items
	QList<GAgentWrapitem*> listTopAgentItem;
	foreach(QGraphicsItem* pAgIt, RootGraphicsItem()->childItems()) {
		GAgentWrapitem* pWrapItem = dynamic_cast<GAgentWrapitem*>(pAgIt);
		if(pWrapItem)
			listTopAgentItem.append(pWrapItem);
	}

	// save the list of top item made above
	GSerializable::SaveListAndItems<GAgentWrapitem>(inQsettings, "TopItem", listTopAgentItem);
}

void GMapGraphicsScene::InterpretSettings( QSettings& fromQsettings )
{
	QStringList TopItemListIDs = fromQsettings.value("TopItem").toStringList();
	foreach(const QString & strItemID, TopItemListIDs) {
		fromQsettings.beginGroup(strItemID);
		QString theAgentTypeID = fromQsettings.value("agent").toString();
		if(theAgentTypeID != "") {
			// just in case the agent already exists because it was created in another map
			GAgent* pAgent = AgentManagerInstance()->GetAgent(theAgentTypeID);
			if(pAgent) {
				Q_ASSERT_X(false, "GMapGraphicsScene::InterpretSettings", "Interesting, the agent existed already");
				QGraphicsItem* pAgIt = pAgent->ProvideNewAgentWrappingItem(this);
				if(pAgIt) {
// 					AddItem(pAgIt);
					GAgentWrapitem* pTabAg = dynamic_cast<GAgentWrapitem*>(pAgIt);
					if(pTabAg) {
						pTabAg->InterpretSettings(fromQsettings);
					}
				}
				qDebug() << "AAAAAAAAAAAAAAAAAAAAAA";
			} // but at startup it is (always?) going to go here and create a container
			else { // no preloaded agent
				GAgentWrapitem* pWrapItem = new GAgentWrapitem(theAgentTypeID, 0);
				AddItem(pWrapItem);
				pWrapItem->InterpretSettings(fromQsettings);
			}
		}

		fromQsettings.endGroup();
	}
}

void GMapGraphicsScene::AddItem( QGraphicsItem* pItem )
{
	if(!pItem)
		return;
 	pItem->setParentItem(RootGraphicsItem());

	m_AgItems.insert(pItem);
}









void GTestScenePopulator::run()
{
	makeBranch(m_LevelToLeaves, m_ItemPerGroup, m_BranchPerLevel, m_CurentPt, m_pScene);
	qDebug() << "done";
}

void GTestScenePopulator::makeBranch(int levelToLeaves, int itemPerGroup, int branchPerLevel, QPointF &curentPt, GMapGraphicsScene* pScene)
{
	static int staticSeed = 132;
	qsrand(staticSeed++);
	double lenghtBranch = 20000.0 * pow(2.0, double(levelToLeaves));
	double sqrtItPerGroup = sqrt(double(itemPerGroup)) * 2000.0;

	if(levelToLeaves > 0) {
		for(int b = 0; b < branchPerLevel ; b++) {
			QLineF br = QLineF::fromPolar(lenghtBranch, double(qrand()));
			br.translate(curentPt);
			QPointF pt = br.p2();
// 			emit ItemReady(new QGraphicsLineItem(br));
			makeBranch(levelToLeaves - 1, itemPerGroup, branchPerLevel, pt, pScene);
		}
		return;
	}

	// if at leaves
	for(int it = 0; it < itemPerGroup ; it++) {
		QGraphicsRectItem* pRectIt = new QGraphicsRectItem(-500.0, -500.0, 1000.0, 1500.0);
		pRectIt->setBrush(randRGB());
		pRectIt->setPen(QPen(Qt::NoPen));
		double x = (rand()) % int(sqrtItPerGroup);
		double y = (rand()) % int(sqrtItPerGroup);
		x -= sqrtItPerGroup / 2;
		y -= sqrtItPerGroup / 2;
		pRectIt->setPos(x, y);
		pRectIt->moveBy(curentPt.x(), curentPt.y());
		emit ItemReady(pRectIt);
	}
 	msleep(10);
}