#include "GStemCellAgent.h"
#include "GAgentManager.h"
#include "SceneView/GMapGraphicsScene.h"
#include "SceneView/GMapGraphicsView.h"
#include "SceneView/GRubberBandItem.h"
#include <QGraphicsProxyWidget>
#include <QtGui>
#include "GGroupButtonItem.h"

GStemCellAgent::GStemCellAgent(QObject *pParent, QString uniqueIdentifierName /*= ""*/)
	: GAgent(pParent, uniqueIdentifierName)
{

}

GStemCellAgent::~GStemCellAgent()
{

}

QGraphicsItem* GStemCellAgent::NewContentItem( GAgentWrapitem* pParentItem )
{
	QGraphicsPixmapItem* pPixIt = new QGraphicsPixmapItem(pParentItem);
#if defined(Q_OS_WIN)
	static QPixmap thePix("../images/StemCell.jpg");
#elif defined(Q_OS_MAC)
    static QPixmap thePix("../../../../images/StemCell.jpg");
#else
    static QPixmap thePix("../../../../images/StemCell.jpg");
#endif
	int theSize = 2000;
	static QPixmap scalledPixmap = thePix.scaled(theSize, theSize);
	pPixIt->setPixmap(scalledPixmap);
// 	pPixIt->setOffset(-theSize / 2, -theSize / 2);

	return pPixIt;
}

bool GStemCellAgent::InjectInitializationData( const QVariantList & theDataList )
{
	if(theDataList.count() < 2)
		return false;

	// the first item of the list must be the class name.
	QString strClassNameToBe = theDataList.first().toString();
	if(strClassNameToBe.isEmpty())
		return false;
	m_TryBeClassName = strClassNameToBe;

	// the rest must be the data necessary for initializing the corresponding agent.
	m_TryBeInitializationData = theDataList;
	// we drop the first item which was the class name
// 	if(m_TryBeInitializationData.count() >= 1) {
	m_TryBeInitializationData.takeFirst();
// 	}
	return true;
}

QVariantList GStemCellAgent::ProvideInitializationData()
{
	return QVariantList() << m_TryBeClassName << m_TryBeInitializationData;
}










//////////////////////////////////////////////////////
//				GDraggedInStemCellAgent				//
//////////////////////////////////////////////////////

GDraggedInStemCellAgent::GDraggedInStemCellAgent(GMapGraphicsScene* pScene, QWidget* pViewWidget) 
	: GStemCellAgent(pScene)
	, m_pSceneDraggedOn(pScene)
	, m_pViewDraggedOn(qobject_cast<QWidget*>(pViewWidget))
{
}

GDraggedInStemCellAgent::~GDraggedInStemCellAgent()
{
}

void GDraggedInStemCellAgent::InjectMimeData( const QMimeData* pMime )
{
	// we go through the filters to see how they score relatively to the mime data
	QMap<double, GAgentCreationFilter*> candidates;
	foreach(GAgentCreationFilter* pFilt, AgentManagerInstance()->m_CreationFilters) {
		double score = pFilt->MightCreateAgent(pMime);
		if(score) {
			candidates.insert(score, pFilt);
		}
	}
	// if it is not empty, 
	QList<double> allScores = candidates.keys();
	if(allScores.isEmpty())
		return;

	// we sort the scores, 
	qSort(allScores.begin(), allScores.end());
	// and get the best candidate
	GAgentCreationFilter* bestCandidate = candidates.value(allScores.last());
	m_TryBeClassName = bestCandidate->GetClassName();
	m_TryBeInitializationData = bestCandidate->ExtractInitializationData(pMime);
	qDebug() << m_TryBeClassName;
}










//////////////////////////////////////////////////////
//				GRubberBandStemCellAgent			//
//////////////////////////////////////////////////////

GRubberBandStemCellAgent::GRubberBandStemCellAgent(GMapGraphicsScene* pScene, QWidget* pViewWidget) 
	: GStemCellAgent(pScene)
	, m_pSceneDraggedOn(pScene)
	, m_pViewDraggedOn(qobject_cast<QWidget*>(pViewWidget))
{
}

GRubberBandStemCellAgent::~GRubberBandStemCellAgent()
{
}

QGraphicsItem* GRubberBandStemCellAgent::NewContentItem( GAgentWrapitem* pParentItem )
{
    QGraphicsRectItem* pRect = new GRubberBandItem(this, pParentItem);
	return pRect;
}

void GRubberBandStemCellAgent::PopulateOverlay(GOverlayItem *pOlIt)
{
    GAgent::PopulateOverlay(pOlIt);
    GGroupButtonItem* grButIt = new GGroupButtonItem(pOlIt);
    pOlIt->addOverlayButton(grButIt, 1);
}
