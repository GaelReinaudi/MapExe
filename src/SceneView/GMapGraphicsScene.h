#ifndef GMAPGRAPHICSSCENE_H
#define GMAPGRAPHICSSCENE_H

#include "../../../LabExe/src/ToolBox/GSerializable.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMutex>
#include <QThread>

class GStemCellAgent;
class GDraggedInStemCellAgent;
class GRubberBandStemCellAgent;
class GAgentWrapitem;
class GRubberBandWrapitem;

/////////////////////////////////////////////////////////////////////
//! \brief The GMapGraphicsScene class inherits QGraphicsScene to provide a convenient interaction with the agent based structure.
/*!
*/
class GMapGraphicsScene : public QGraphicsScene, public GSerializable
{
	Q_OBJECT

public:
	GMapGraphicsScene(QObject *parent);
	~GMapGraphicsScene();

	//! Returns the item that parents top level items
	QGraphicsItem* RootGraphicsItem() const { return m_pRootItem; }

private: // made private to prevent direct use of it
	void addItem(QGraphicsItem *item) {QGraphicsScene::addItem(item);}

public:
	//! Reimplemented.
	void PopulateSettings( QSettings& inQsettings );
	//! Reimplemented.
	void InterpretSettings( QSettings& fromQsettings );

public slots:
	//! Adds the item as a top level item of this scene. It Actually parents it to the RootGraphicsItem()
	void AddItem(QGraphicsItem* pItem);

protected:
	//! Re-implemented
	void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
	//! Re-implemented
	void dragLeaveEvent(QGraphicsSceneDragDropEvent * event);
	//! Re-implemented
	void dragMoveEvent(QGraphicsSceneDragDropEvent * event);
	//! Re-implemented
	void dropEvent (QGraphicsSceneDragDropEvent * event);
    //! Re-implemented
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //! Re-implemented
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //! Re-implemented
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    //! Re-implemented
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

protected slots:
    //void SelChanged();

private:
	QGraphicsPixmapItem* m_pRootItem;
	//! tells us what was the last clicked item, or 0 if the click was not on an item
	QGraphicsItem* m_ClickedItem;
	//! When dragging something in the map, this points to a newly created stem cell agent. When leaving without dropping, it is destroyed.
	GDraggedInStemCellAgent* m_pDragStemCellAgent;
	//! When making a rubber band in the map, this points to a newly created stem cell agent. 
	GRubberBandStemCellAgent* m_pRubberBandAgent;
	//! Idem but the item
	GAgentWrapitem* m_pDraggedAgentGraphItem;
    GRubberBandWrapitem* m_pRubberWrap;

// 	QMutex m_TestMutex;
	//! contains the items pointers in a way that can be shared with other threads
	QSet<QGraphicsItem*> m_AgItems;
	friend class GSceneBench;
};

class GTestScenePopulator : public QThread
{
	Q_OBJECT

public:
	GTestScenePopulator(int levelToLeaves, int itemPerGroup, int branchPerLevel, QPointF & curentPt, GMapGraphicsScene* pScene) 
		: QThread(pScene)
		, m_LevelToLeaves(levelToLeaves)
		, m_ItemPerGroup(itemPerGroup)
		, m_BranchPerLevel(branchPerLevel)
		, m_CurentPt(curentPt)
		, m_pScene(pScene)
	{
		connect(this, SIGNAL(ItemReady(QGraphicsItem*)), m_pScene, SLOT(AddItem(QGraphicsItem*)));
	}
	~GTestScenePopulator()
	{
		wait(1000);
		terminate();
	}

	void run();

	void makeBranch(int levelToLeaves, int itemPerGroup, int branchPerLevel, QPointF &curentPt, GMapGraphicsScene* pScene);

signals:
	void ItemReady(QGraphicsItem* pItem);

private:
	int m_LevelToLeaves;
	int m_ItemPerGroup;
	int m_BranchPerLevel;
	QPointF m_CurentPt;
	GMapGraphicsScene* m_pScene;
};

#endif // GMAPGRAPHICSSCENE_H
