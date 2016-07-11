#include "GMapStatsWidget.h"
#include "GMapGraphicsScene.h"
#include "agent.h"
#include "Agent/GAgentManager.h"

GMapStatsWidget::GMapStatsWidget(GMapGraphicsScene* pScene, QWidget *parent)
	: QWidget(parent)
	, m_pScene(pScene)
	, m_IsAlreadyGoingToCountItems(false)
{
	setupUi(this);
	// makes a list of already know types
	QStringList listAg = AgentManagerInstance()->RegisteredAgentTypes();
	foreach(QString strClassName, listAg) {
		MakeNewField(strClassName);
	}

	// for already existing agent when creating this widget,we call the stepup
	foreach(GAgent* pAg, AgentManagerInstance()->AllAgents()) {
		StepUpAgent(pAg);
	}
	// connect (queued for guaranteeing the full construction of the agent), so that we step up the appropriate box.
	connect(AgentManagerInstance(), SIGNAL(AddedAgent(GAgent*)), this, SLOT(StepUpAgent(GAgent*)), Qt::QueuedConnection);
	connect(AgentManagerInstance(), SIGNAL(RemovedAgent(GAgent*)), this, SLOT(StepDownAgent(GAgent*)), Qt::DirectConnection);
	connect(pClearButton, SIGNAL(clicked()), AgentManagerInstance(), SLOT(Clear()), Qt::QueuedConnection);

//	QTimer::singleShot(500, this, SLOT(UpdateSceneItemsCount()));
	QTimer* pTimer = new QTimer(this);
	pTimer->setInterval(2000);
	pTimer->start();
	connect(pTimer, SIGNAL(timeout()), this, SLOT(UpdateSceneItemsCount()));
}

GMapStatsWidget::~GMapStatsWidget()
{

}

void GMapStatsWidget::StepUpAgent( GAgent* pAgent )
{
	pAgentBox->stepUp();
	QString className = pAgent->metaObject()->className();
	if(!m_Spinboxes.contains(className))
		MakeNewField(className);

	m_Spinboxes.value(className)->stepUp();

	if(m_IsAlreadyGoingToCountItems)
		return;

	m_IsAlreadyGoingToCountItems = true;
// 	QMetaObject::invokeMethod(this, "UpdateSceneItemsCount", Qt::QueuedConnection);
	QTimer::singleShot(500, this, SLOT(UpdateSceneItemsCount()));
}

void GMapStatsWidget::StepDownAgent( GAgent* pAgent )
{
	pAgentBox->stepDown();
	QString className = pAgent->metaObject()->className();
	if(!m_Spinboxes.contains(className))
		MakeNewField(className);

	m_Spinboxes.value(className)->stepDown();

	if(m_IsAlreadyGoingToCountItems)
		return;

	m_IsAlreadyGoingToCountItems = true;
// 	QMetaObject::invokeMethod(this, "UpdateSceneItemsCount", Qt::QueuedConnection);
	QTimer::singleShot(500, this, SLOT(UpdateSceneItemsCount()));
}

void GMapStatsWidget::MakeNewField( QString strClassName )
{
	QLabel* pLab = new QLabel(strClassName, this);
	QSpinBox* pSpin = new QSpinBox(this);
	pSpin->setMinimum(-10000);
	pSpin->setMaximum(999999999);
	pSpin->setReadOnly(true);
	pSpin->setButtonSymbols(QAbstractSpinBox::NoButtons);
	pAgentLayout->addRow(pLab, pSpin);
	m_Spinboxes.insert(strClassName, pSpin);
}

void GMapStatsWidget::UpdateSceneItemsCount()
{
	if(!m_pScene)
		return;
	QList<QGraphicsItem*> listItems = m_pScene->items();
	// Minus one so that we don't count the RootITem().
	int nit = listItems.count() - 1;
	pItemsBox->setValue(nit);
// 	qDebug() << nit;
	m_IsAlreadyGoingToCountItems = false;
}