#ifndef GSCENEBENCH_H
#define GSCENEBENCH_H
#include "labexegraphicsmap_global.h"

#include "TheLab/GWorkBench.h"

class GMapGraphicsScene;
class GMapGraphicsView;
class QGraphicsItem;

/////////////////////////////////////////////////////////////////////
//! \brief The GSceneBench class defines a workbench intended to display the Graphics map system. 
/*!
It is a GWorkBench where the central widget has been set to a GMapGraphicsView/Scene.
*/
class LABEXEGRAPHICSMAP_EXPORT GSceneBench : public GWorkBench
{
	Q_OBJECT

public:
	GSceneBench(QWidget *parent);
	~GSceneBench();

	//! Returns the scene that displays the image in its background.
	GMapGraphicsScene* Scene() const { return m_pScene; }
	void RandomizePosition( QGraphicsItem* pItem );
	//! Connects a fps tracker to the status bar
	void TrackAndDisplayFps();

public:
	//! Re-implemented from GWorkBench in order to put the devices gui in the scene.
	GDeviceWidget* AddDevice(GDevice* pTheDevice, QPoint whereInParent = QPoint(0, 0), bool putOnStack = true);
	//! Reimplemented.
	void PopulateSettings( QSettings& inQsettings );
	//! Re-implemented 
	void InterpretSettings(QSettings& fromQsettings);

private slots:
	//! Test that puts n pixmaps on the map
	void Test0(int nCol = 10, int nRow = 10);
	void Test1() {Test0(40, 25);}
	void Test2() {Test0(100, 100);}

	void Test10(int nCol = 10, int nRow = 10);
	void Test11() {Test10(100, 100);}

	void Test20(int totalItem = 1000, int branchPerLevel = 5);
	void Test21() {Test20(10000, 5);}
	void Test22() {Test20(100000, 5);}

	void Test30(int totalItem = 10);

	void Test40(int totalItem = 10000);

private:
	//! This is the scene that this bench view.
	GMapGraphicsScene* m_pScene;
	//! the view that shows the scene
	GMapGraphicsView* m_pView;
	//! stat widget
	QWidget* m_pStatWid;
};


#endif // GSCENEBENCH_H
