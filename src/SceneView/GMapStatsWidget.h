#ifndef GMAPSTATSWIDGET_H
#define GMAPSTATSWIDGET_H

#include <QWidget>
#include "ui_GMapStatsWidget.h"

class GAgent;
class GMapGraphicsScene;

/////////////////////////////////////////////////////////////////////
//! \brief The docccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
/*!
*/
class GMapStatsWidget : public QWidget, public Ui::GMapStatsWidget
{
	Q_OBJECT

public:
	GMapStatsWidget(GMapGraphicsScene* pScene, QWidget *parent = 0);

	void MakeNewField( QString strClassName );
	~GMapStatsWidget();

protected slots:
	void StepUpAgent(GAgent* pAgent);
	void StepDownAgent(GAgent* pAgent);
	void UpdateSceneItemsCount();

private:
	QHash<QString, QSpinBox*> m_Spinboxes;
	GMapGraphicsScene* m_pScene;
	//! flag to not clog the event loop when adding huge numbers of items
	bool m_IsAlreadyGoingToCountItems;
};

#endif // GMAPSTATSWIDGET_H
