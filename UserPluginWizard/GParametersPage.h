#ifndef GPARAMETERSPAGE_H
#define GPARAMETERSPAGE_H

#include <QWizardPage>
#include "ui_GParametersPage.h"

class GParametersPage : public QWizardPage, public Ui::GParametersPage
{
	Q_OBJECT

public:
	GParametersPage(QWidget *parent = 0);
	~GParametersPage();
};

#endif // GPARAMETERSPAGE_H
