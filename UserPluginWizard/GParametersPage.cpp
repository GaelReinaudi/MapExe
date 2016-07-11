#include "GParametersPage.h"

GParametersPage::GParametersPage(QWidget *parent)
	: QWizardPage(parent)
{
	setupUi(this);
	setPixmap(QWizard::LogoPixmap, QPixmap (":/wizard/logo1.png"));

	pTopLabel->setText(tr(
		"Those parameters will be seamlessly saved and restored with the program settings. "
		"They will also be drag-and-drop-able to any other devices."
		));

	// Fields
	registerField("whatever", pNewIntLabel);
}

GParametersPage::~GParametersPage()
{

}
