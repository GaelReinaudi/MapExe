#include "GSceneBench.h"
#include "GMapGraphicsScene.h"
#include "GMapGraphicsView.h"
#include "agent.h"
#include "device.h"
#include "GMapStatsWidget.h"
#include "Agent/GAgentManager.h"
#include <QtCore/qmath.h>
#include <QtCore>
#include <QtTest>

G_REGISTER_WORKBENCH_CLASS(GSceneBench)

GSceneBench::GSceneBench(QWidget *parent)
	: GWorkBench(parent)
{
	m_pScene = new GMapGraphicsScene(this);
	m_pView = new GMapGraphicsView(this);
	m_pView->setScene(m_pScene);
	connect(m_pScene, SIGNAL(sceneRectChanged(QRectF)), m_pView, SLOT(UpdateFromSceneRect(QRectF)),Qt::QueuedConnection);
	setCentralWidget(m_pView);

	m_pStatWid = new GMapStatsWidget(m_pScene, 0);
#ifndef QT_NO_DEBUG
	m_pStatWid->show();
#endif
	m_pStatWid->move(0, 0);

	setUnifiedTitleAndToolBarOnMac(true);

//#ifndef QT_NO_DEBUG
	QToolBar* theTB = addToolBar("Tests");
	theTB->setObjectName("Tests");
	theTB->addAction("Clear", AgentManagerInstance(), SLOT(Clear()));

	theTB->addAction("100 Pix", this, SLOT(Test0()));
	theTB->addAction("1,000 Pix", this, SLOT(Test1()));
	theTB->addAction("10,000 Pix", this, SLOT(Test2()));

	theTB->addAction("100 Webs", this, SLOT(Test10()));
	theTB->addAction("10,000 Webs", this, SLOT(Test11()));

	theTB->addAction("1,000 QRect", this, SLOT(Test20()));
	theTB->addAction("10,000 QRect", this, SLOT(Test21()));
	theTB->addAction("100,000 QRect", this, SLOT(Test22()));

	theTB->addAction("QML 10 games", this, SLOT(Test30()));

	theTB->addAction("QML 10,000 Rect", this, SLOT(Test40()));
//#endif
}

GSceneBench::~GSceneBench()
{
	delete m_pStatWid;
}

GDeviceWidget* GSceneBench::AddDevice( GDevice* pTheDevice, QPoint whereInParent /*= QPoint(0, 0)*/, bool putOnStack /*= true*/ )
{
	GDeviceWidget* pDevWid = GWorkBench::AddDevice(pTheDevice, whereInParent, putOnStack);

	// provide another device widget that will go in the scene
	// is the device a GAgent?
	GAgent* pAgent = qobject_cast<GAgent*>(pTheDevice);
	// or is it a device that GHasAgent?
	GHasAgent* pHasAgent = dynamic_cast<GHasAgent*>(pTheDevice);

	if(!pAgent)
		if(pHasAgent)
			pAgent = pHasAgent->TheAgent();

	if(!pAgent)
		return pDevWid;

	QGraphicsItem* pAgentGraphItem = pAgent->ProvideNewAgentWrappingItem(m_pScene);

	return pDevWid;
}

void GSceneBench::RandomizePosition( QGraphicsItem* pItem )
{
	// rand gives a number between 0 and RAND_MAX
	// this is a double between -1 and 1
	double x = 2.0 * (double(qrand() - RAND_MAX / 2) / double(RAND_MAX));
	double y = 2.0 * (double(qrand() - RAND_MAX / 2) / double(RAND_MAX));
	x *= 5000.0;
	y *= 5000.0;
	pItem->setPos(x,y);
}

void GSceneBench::PopulateSettings( QSettings& inQsettings )
{
	GWorkBench::PopulateSettings(inQsettings);
	// save the agents informations
	m_pScene->PopulateSettings(inQsettings);
	
	double theZoom = m_pView->transform().m11();
	inQsettings.setValue("zoom", theZoom);

	QPointF centerPos = m_pView->GetCenter();
	inQsettings.setValue("center", centerPos);
}

void GSceneBench::InterpretSettings( QSettings& fromQsettings )
{
	GWorkBench::InterpretSettings(fromQsettings);

	// test to create agent from data inside the bench file
	AgentManagerInstance()->InterpretSettings(fromQsettings);

	// read the agents informations
	m_pScene->InterpretSettings(fromQsettings);

	double theZoom = fromQsettings.value("zoom", 0.5).toDouble();
	m_pView->SetZoom(theZoom);

	QPointF centerPos = fromQsettings.value("center").toPointF();
	if(!centerPos.isNull())
		QMetaObject::invokeMethod(m_pView, "SetCenter", Qt::QueuedConnection, Q_ARG(QPointF, centerPos));
}

void GSceneBench::TrackAndDisplayFps()
{
	FPSTracker* pFps = new FPSTracker(m_pView);
	m_pView->viewport()->installEventFilter(pFps);
	connect(pFps, SIGNAL(FpsUpdated(QString)), statusBar(), SLOT(clearMessage()), Qt::QueuedConnection);
	connect(pFps, SIGNAL(FpsUpdated(QString)), statusBar(), SLOT(showMessage(QString)), Qt::QueuedConnection);

	QDoubleSpinBox* pSpinZoom = new QDoubleSpinBox();
	pSpinZoom->setButtonSymbols(QAbstractSpinBox::NoButtons);
	pSpinZoom->setDecimals(4);
	pSpinZoom->setPrefix("zoom = ");
	pSpinZoom->setKeyboardTracking(false);
	statusBar()->insertPermanentWidget(0, pSpinZoom);
	connect(m_pView, SIGNAL(ZoomChanged(double)), pSpinZoom, SLOT(setValue(double)));

	QDoubleSpinBox* pSpinLogDist = new QDoubleSpinBox();
	pSpinLogDist->setButtonSymbols(QAbstractSpinBox::NoButtons);
	pSpinLogDist->setDecimals(2);
	pSpinLogDist->setRange(-9.9, 99.9);
	pSpinLogDist->setPrefix("log2 dist. = ");
	pSpinLogDist->setKeyboardTracking(false);
	statusBar()->insertPermanentWidget(1, pSpinLogDist);
	connect(m_pView, SIGNAL(LogDistanceChanged(double)), pSpinLogDist, SLOT(setValue(double)));

	QSpinBox* pSpinDistLevel = new QSpinBox();
	pSpinDistLevel->setButtonSymbols(QAbstractSpinBox::NoButtons);
	pSpinDistLevel->setRange(-9, 99);
	pSpinDistLevel->setPrefix("dist. level = ");
	pSpinDistLevel->setKeyboardTracking(false);
	statusBar()->insertPermanentWidget(2, pSpinDistLevel);
	connect(m_pView, SIGNAL(DistanceLevelChanged(int)), pSpinDistLevel, SLOT(setValue(int)));
}

void GSceneBench::Test0(int nCol /*= 10*/, int nRow /*= 10*/)
{
#if defined(Q_OS_MAC)
	QString thePmFilePath("../../../../Init/venus964.jpg");
#else
	QString thePmFilePath("../Init/venus964.jpg");
#endif

	QPixmap thePM(thePmFilePath);
	QSize pmSize = thePM.size();

	//Populate the scene
	QGraphicsItem* pAgentGraphItem = 0;
	for(int x = 0; x < nCol; ++x) {
		for(int y = 0; y < nRow; ++y) {
			GAgent* pAgent = AgentManagerInstance()->CreateNewProgAgent("GPictureAgent");
			if(pAgent) {
				pAgentGraphItem = pAgent->ProvideNewAgentWrappingItem(m_pScene);
				QMetaObject::invokeMethod(pAgent, "SetUri", Q_ARG(QString, thePmFilePath));
			}
			if(pAgentGraphItem) {
				pAgentGraphItem->setPos(x * 1.3 * pmSize.width(), y * 1.3 * pmSize.height());
			}
		}
	}
}

void GSceneBench::Test10( int nCol /*= 10*/, int nRow /*= 10*/ )
{
#if defined(Q_OS_MAC)
	QString thePmFilePath("../../../../images/crosshair.png");
#else
	QString thePmFilePath("../images/crosshair.png");
//	QString thePmFilePath("http://www.google.com");
#endif

	QPixmap thePM(thePmFilePath);
	QSize pmSize = thePM.size();

	//Populate the scene
	QGraphicsItem* pAgentGraphItem;
	for(int x = 0; x < nCol; ++x) {
		for(int y = 0; y < nRow; ++y) {
			GAgent* pAgent = AgentManagerInstance()->CreateNewProgAgent("GWebPageAgent");
			if(pAgent) {
				pAgentGraphItem = pAgent->ProvideNewAgentWrappingItem(m_pScene);
				QMetaObject::invokeMethod(pAgent, "LoadUrl", Q_ARG(QUrl, QUrl(thePmFilePath)));
			}
			pAgentGraphItem->setPos(x * 2 * pmSize.width(), y * 2 * pmSize.height());
		}
	}
}

void GSceneBench::Test20(int totalItem /*= 1000*/, int branchPerLevel /*= */)
{
	int nLevel = 0;
	int itemPerGroup = 100;
	int a = 1;
	while(a < totalItem / itemPerGroup) {
		a *= branchPerLevel;
		++nLevel;
	}
	itemPerGroup = totalItem / qPow(double(branchPerLevel), double(nLevel));

	QPointF curentPt(0.0, 0.0);

	GTestScenePopulator* pPopulator = new GTestScenePopulator(nLevel, itemPerGroup, branchPerLevel, curentPt, m_pScene);
	pPopulator->start();
}

#include <QDeclarativeItem>
#include <QDeclarativeEngine>
void GSceneBench::Test30( int totalItem /*= 10*/ )
{
	static QDeclarativeEngine engine;
	QFile qmlFile("../src/Resources/testQmlGame/samegame4/samegame.qml");
	qDebug() << QFileInfo(qmlFile).absoluteFilePath();
	static QDeclarativeComponent component(&engine, QFileInfo(qmlFile).absoluteFilePath());

	int spaces = 1000;
	int nRow = qFloor(qSqrt(totalItem));
	int nCol = nRow + 1;
	for(int x = 0; x < nCol; ++x) {
		for(int y = 0; y < nRow; ++y) {
			QDeclarativeItem* pQMLitem = qobject_cast<QDeclarativeItem*>(component.create());
			if(pQMLitem) {
				m_pScene->addItem(pQMLitem);
				pQMLitem->setPos(x * 2 * spaces, y * 2 * spaces);
			}
		}
	}
}

void GSceneBench::Test40( int totalItem /*= 10000*/ )
{
	QFile qmlFile("../src/Resources/1000Rect.qml");
	qDebug() << QFileInfo(qmlFile).absoluteFilePath();
	static QDeclarativeEngine engine;

	engine.clearComponentCache();
	QDeclarativeComponent component(&engine, QFileInfo(qmlFile).absoluteFilePath());

	QDeclarativeItem* pQMLitem = qobject_cast<QDeclarativeItem*>(component.create());
	if(pQMLitem) {
		m_pScene->addItem(pQMLitem);
	}
}











