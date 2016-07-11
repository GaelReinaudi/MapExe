#ifndef MAPEXE_H
#define MAPEXE_H

#include "ui_mapexe.h"
#include "labexe.h"

class MapExe : public GLabControlPanel
{
	Q_OBJECT

public:
	MapExe(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MapExe();

protected:
	//! Re-implemented to add some features to the newly added bench.
	void InsertWorkBenchInList(GWorkBench* pWB);

private:
	//! Re-implemented to save the agents
	void SaveBenches(QSettings &panelSettings);
	//! Re-implemented to restore the agents
	void RestoreBenches(QSettings &panelSettings);

private:
// 	Ui::MapExeClass ui;
};

#endif // MAPEXE_H
