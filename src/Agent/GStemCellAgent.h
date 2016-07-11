#ifndef GSTEMCELLAGENT_H
#define GSTEMCELLAGENT_H

#include "GAgent.h"
#include "GRubberBandWrapitem.h"

class GMapGraphicsScene;
class GMapGraphicsView;
class GRubberBandWrapitem;

/////////////////////////////////////////////////////////////////////
//! \brief The GStemCellAgent class implements a very special kind of GAgent.
/*!
A stem cell agent is representing an instance of an agent that is not specialized into a fully functional 
agent (e.g. GPictureAgent, or GMp3Agent).

It has the ability to tranform into any other known type of agent by using the AgentManager()`s 
factory. If it has to transform into an unkown type of agent, it can store the details 
provided to him until it may be able to transform.
*/
class GStemCellAgent : public GAgent
{
	Q_OBJECT

public:
	GStemCellAgent(QObject *pParent, QString uniqueIdentifierName = "");
    virtual ~GStemCellAgent();

	//! Re-implemented because there is no tablet to populate here.
    virtual QGraphicsItem* NewContentItem(GAgentWrapitem* pParentItem);

	//! Returns the class name of the agent this might become.
	QString AgentTypeToBe() const { return m_TryBeClassName; }
	//! Re-implemented to return true!
	GStemCellAgent* isStemCell() {return this;}

	//! Implemented
	virtual bool InjectInitializationData(const QVariantList & theDataList);
	//! Implemented
	virtual QVariantList ProvideInitializationData();

private:
	//! the name of the class this stem cell agent might try to become
	QString m_TryBeClassName;
	QVariantList m_TryBeInitializationData;

	friend class GDraggedInStemCellAgent;
	friend class GAgentManager;
};

class GDraggedInStemCellAgent : public GStemCellAgent
{
	Q_OBJECT

public:
	GDraggedInStemCellAgent(GMapGraphicsScene* pScene, QWidget* pViewWidget);
	~GDraggedInStemCellAgent();

	//! Injects some Mime data in order to provide insight about what agent this stem cell agent could become.
	void InjectMimeData( const QMimeData* pMime );

private:
	//! Set this agent to follow the cursor on the provided scene as it is being dragged, and self destroys when leaving without dropping.
	GMapGraphicsScene* m_pSceneDraggedOn;
	QWidget* m_pViewDraggedOn;

};

class GRubberBandStemCellAgent : public GStemCellAgent
{
	Q_OBJECT

public:
	GRubberBandStemCellAgent(GMapGraphicsScene* pScene, QWidget* pViewWidget);
	~GRubberBandStemCellAgent();

	GRubberBandWrapitem* ProvideNewRubberBandWrappingItem(QGraphicsItem* pParentItem) {
		return new GRubberBandWrapitem(this, pParentItem);
	}

    QGraphicsItem* NewContentItem(GAgentWrapitem* pParentItem);
    virtual void PopulateOverlay(GOverlayItem *pOlIt);

// 	//! Injects the clipboard data in order to provide insight about what agent this stem cell agent could become.
// 	void InjectClipBoardData();

private:
	//! Set this agent to follow the cursor on the provided scene as it is being dragged, and self destroys when leaving without dropping.
	GMapGraphicsScene* m_pSceneDraggedOn;
	QWidget* m_pViewDraggedOn;

};



#endif // GSTEMCELLAGENT_H
