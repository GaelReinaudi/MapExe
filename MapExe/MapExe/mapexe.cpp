#include "mapexe.h"
#include "labexegraphicsmap.h"
#include "SceneView/GSceneBench.h"
#include "agent.h"
#include "Agent/GAgentManager.h"

#define KEEP_DEBUG_MESSAGES

MapExe::MapExe(QWidget *parent, Qt::WFlags flags)
	: GLabControlPanel(parent)
{
	Q_UNUSED(flags);
	setWindowTitle("MapExe");

	// loads the plugins that defined derived classes of GAgent
	LoadPluginsInAgentManager();

	QPixmapCache::setCacheLimit(100000);

	LabExeGraphicsMap();

}

MapExe::~MapExe()
{
//2012-07-08	AgentManagerInstance()->CleanUp();
}

void MapExe::SaveBenches( QSettings &panelSettings )
{
	GLabControlPanel::SaveBenches(panelSettings);
	AgentManagerInstance()->PopulateSettings(panelSettings);
}

void MapExe::RestoreBenches( QSettings &panelSettings )
{
	// In case we hold the Ctrl key during startup, we don't load any settings and this will default to a Initialization map (see bellow)
	Qt::KeyboardModifiers modifierKey = QApplication::queryKeyboardModifiers();
	if(modifierKey == Qt::ControlModifier || modifierKey == Qt::MetaModifier) {
		// load nothing
	}
	else {
// 		AgentManagerInstance()->InterpretSettings(panelSettings);
		GLabControlPanel::RestoreBenches(panelSettings);
	}

#ifdef QT_NO_DEBUG
#ifdef KEEP_DEBUG_MESSAGES
	show();
	DebugDockWidget()->setFloating(true);
	DebugDockWidget()->show();
#else
 	DebugDockWidget()->setFloating(false);
#endif
	hide();
#endif

	if(WorckbenchListWidget()->count() == 0) {
		GWorkBench* pStartingMap = AddWorkBenchFromFile("../Init/StartingMap.map");
		if(pStartingMap) {
			QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm");
			QDir dirHome = QDir::home();
			dirHome.mkdir("Basalt");
			dirHome.cd("Basalt");
			QString pathSave = dirHome.absolutePath();
			pathSave += "/";
			pathSave += strDateTime;
			pathSave += ".map";
			pStartingMap->saveFile(pathSave);
			qDebug() << pathSave;
		}
	}
}

void MapExe::InsertWorkBenchInList(GWorkBench* pWB)
{
	GLabControlPanel::InsertWorkBenchInList(pWB);

	GSceneBench* pSceneBench = qobject_cast<GSceneBench*>(pWB);
	// tracking fps
#ifdef KEEP_DEBUG_MESSAGES
	pSceneBench->TrackAndDisplayFps();
#endif
}










