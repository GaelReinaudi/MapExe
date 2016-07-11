#include <QtGui>

#include "UserPluginWizard.h"
#include "GParametersPage.h"

UserPluginWizard::UserPluginWizard(QWidget *parent)
    : QWizard(parent)
{
	addPage(new IntroPage);
	addPage(new ClassInfoPage);
	addPage(new GParametersPage);
	addPage(new ConclusionPage);

	setWizardStyle(QWizard::ModernStyle);

	setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
	setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

	setWindowTitle(tr("User Plugin Wizard"));

	m_FolderCopyFilterOut << "Debug" << "Release" << "Release with Debug" << "GeneratedFiles";
	m_ExtensionCopyFilterOut << "ncb" << "suo";// << "png";

	// sets some variable for a specific porject to copy
	m_ToReplaceWithGraphicsItemName = "WebViewGraphicsItem";
	m_ToReplaceWithAgentName = "WebPageAgent";
	m_ToReplaceWithPluginName = "WebPagePlugin";
	m_ToReplaceWithProjectName = "WebPagePlugin";

	// try to find the directory of the project to copy
    m_FromDir = QDir("../../MapExe-Plugins/PluginProjects");
    m_ToDir = QDir("../../MapExe-Plugins");
	m_ToDir.mkdir("UserProjects");
	m_ToDir.cd("UserProjects");

	if(!m_FromDir.cd(m_ToReplaceWithProjectName))
		qWarning() << "couldn't find the source project: " << m_ToReplaceWithProjectName;


	if(!m_FromDir.exists() || !m_ToDir.exists()) {
		qWarning() << "couldn't find the directories. " << m_FromDir << "or" << m_ToDir;
	}
}

void UserPluginWizard::accept()
{
	MakeListOfFilesToUse(m_FromDir);
	QString pathToProjectFolder = MakeProjectDirectory();
	if(!pathToProjectFolder.isEmpty())
		QDesktopServices::openUrl(QUrl("file:///" + pathToProjectFolder));

	QDialog::accept();
}

void UserPluginWizard::MakeListOfFilesToUse(QDir fromDir) {
	QStringList qsl = fromDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

	foreach (QString file, qsl) {
		QFileInfo finfo(QString("%1/%2").arg(fromDir.path()).arg(file));
		// if it is a link, we don't use it
		if(finfo.isSymLink())
			continue;
		// if it is a QDir, we iterate inside it
		if(finfo.isDir()) {
			QString dirname = finfo.fileName();
			// but we filter out some folders
			if(m_FolderCopyFilterOut.contains(dirname))
				continue;
			QDir sd(finfo.filePath());
			MakeListOfFilesToUse(sd);
		} 
		else {
			// we filter out some files extensions
			if(m_ExtensionCopyFilterOut.contains(finfo.suffix()))
				continue;
			// takes the relative path since we stored the m_FromDir before starting this process
			m_FilesToCopy << QDir::toNativeSeparators(m_FromDir.relativeFilePath(finfo.absoluteFilePath()));
		}
	}
}

QString UserPluginWizard::MakeProjectDirectory()
{
	QDir inDir = m_ToDir;
	QString className = field("pluginName").toString();
	QString agentClassName = field("pluginName").toString() + "Agent";
	QString graphicsItemClassName = field("pluginName").toString() + "GraphicsItem";
	QString pluginName = className + "Plugin";
	QString pluginNameDir = pluginName;
	pluginNameDir = QDir::cleanPath(pluginNameDir);
	inDir.mkdir(pluginNameDir);
	if(!inDir.cd(pluginNameDir))
		return "";

	foreach(QString relFilePathOriginal, m_FilesToCopy) {
		// opens the file from relatively to the m_FromDir
		QFile file(m_FromDir.filePath(relFilePathOriginal));
		if(!file.open(QFile::ReadOnly | QFile::Text)) {
			QMessageBox::warning(0, QObject::tr("Plugin Wizard"), QObject::tr("Cannot read file %1:\n%2").arg(file.fileName()).arg(file.errorString()));
			continue;
		}
		// reads the content
		QByteArray content;
		content = file.readAll();
		// replaces strings
		content.replace(m_ToReplaceWithPluginName, pluginNameDir.toUtf8());
		content.replace(m_ToReplaceWithAgentName, agentClassName.toUtf8());
		content.replace(m_ToReplaceWithGraphicsItemName, graphicsItemClassName.toUtf8());
		content.replace(m_ToReplaceWithProjectName, className.toUtf8());
		// for the vcproj, we change the guid
		if(QFileInfo(file).suffix() == "vcproj") {
			int posGUID = content.indexOf("ProjectGUID");
			if(posGUID >= 0) {
				posGUID = content.indexOf("{", posGUID);
				content.replace(posGUID, 38, QUuid::createUuid().toString().toUtf8());
			}
		}

		// open the new file

		QString newFilePath = inDir.filePath(QDir::cleanPath(relFilePathOriginal));
		newFilePath.replace(m_ToReplaceWithPluginName,  pluginName);
		newFilePath.replace(m_ToReplaceWithAgentName,  agentClassName);
		newFilePath.replace(m_ToReplaceWithGraphicsItemName,  graphicsItemClassName);
		newFilePath.replace(m_ToReplaceWithProjectName,  className);
		QFile newfile(newFilePath);
		// create the directory if needed
		QString relativeFilePathInDir = inDir.relativeFilePath(QFileInfo(newfile).absolutePath());
		inDir.mkpath(relativeFilePathInDir);

		if(!newfile.open(QFile::WriteOnly | QFile::Text)) {
			QMessageBox::warning(0, QObject::tr("Plugin Wizard"), QObject::tr("Cannot write file %1:\n%2").arg(newfile.fileName()).arg(newfile.errorString()));
			continue;
		}
		newfile.write(content);
	}
	return QDir::toNativeSeparators(inDir.absolutePath());
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    label = new QLabel(tr("This wizard will generate the skeleton of a C++ project "
                          "that uses the Qt Library in order to make a plugin for the LabExe. "
                          "We will need to specify the plugin name and set a few "
                          "options to produce the appropriate files."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

ClassInfoPage::ClassInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Plugin Information"));
    setSubTitle(tr("Specify basic information about the plugin for which you "
                   "want to generate skeleton source code files."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo1.png"));

    pluginNameLabel = new QLabel(tr("&Plugin name:"));
    pluginNameLineEdit = new QLineEdit;
    pluginNameLabel->setBuddy(pluginNameLineEdit);

//     baseClassLabel = new QLabel(tr("B&ase class:"));
//     baseClassLineEdit = new QLineEdit;
//     baseClassLabel->setBuddy(baseClassLineEdit);
// 
//     qobjectMacroCheckBox = new QCheckBox(tr("Generate Q_OBJECT &macro"));

    groupBox = new QGroupBox(tr("C&onstructor"));

//     qobjectCtorRadioButton = new QRadioButton(tr("&QObject-style constructor"));
//     qwidgetCtorRadioButton = new QRadioButton(tr("Q&Widget-style constructor"));
//     defaultCtorRadioButton = new QRadioButton(tr("&Default constructor"));
//     copyCtorCheckBox = new QCheckBox(tr("&Generate copy constructor and operator="));
// 
//     defaultCtorRadioButton->setChecked(true);
// 
//     connect(defaultCtorRadioButton, SIGNAL(toggled(bool)), copyCtorCheckBox, SLOT(setEnabled(bool)));

    registerField("pluginName*", pluginNameLineEdit);
//     registerField("baseClass", baseClassLineEdit);
//     registerField("qobjectMacro", qobjectMacroCheckBox);
//     registerField("qobjectCtor", qobjectCtorRadioButton);
//     registerField("qwidgetCtor", qwidgetCtorRadioButton);
//     registerField("defaultCtor", defaultCtorRadioButton);
//     registerField("copyCtor", copyCtorCheckBox);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
//     groupBoxLayout->addWidget(qobjectCtorRadioButton);
//     groupBoxLayout->addWidget(qwidgetCtorRadioButton);
//     groupBoxLayout->addWidget(defaultCtorRadioButton);
//     groupBoxLayout->addWidget(copyCtorCheckBox);
    groupBox->setLayout(groupBoxLayout);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(pluginNameLabel, 0, 0);
    layout->addWidget(pluginNameLineEdit, 0, 1);
// 	layout->addWidget(baseClassLabel, 1, 0);
//	layout->addWidget(baseClassLineEdit, 1, 1);
//	layout->addWidget(qobjectMacroCheckBox, 2, 0, 1, 2);
//	layout->addWidget(groupBox, 3, 0, 1, 2);
    setLayout(layout);
}

ParametersPage::ParametersPage(QWidget *parent)
	: QWizardPage(parent)
{
	setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo2.png"));

	setTitle(tr("Parameters"));
	setSubTitle(tr("Estimate the types of parameters that will be used to define your plugin. "
		"Those parameters will be inserted for you in the project, but you can "
		"of course add more by hand later. "
		));
	QLabel* pText = new QLabel(tr("Those parameters will be seamlessly saved and restored with the program settings. "
		"They will also be drag-and-drop-able to any other devices."
		));
	pText->setWordWrap(true);

	QGroupBox* pIntBox = new QGroupBox("Integer parameters");
	pIntBox->setCheckable(true);
	pIntBox->setChecked(false);
	QVBoxLayout *IntBoxLayout = new QVBoxLayout(pIntBox);
	QLabel* pTextInt = new QLabel(tr("hold values that are integer."
		));
	pTextInt->setWordWrap(true);
	IntBoxLayout->addWidget(pTextInt);


	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(pText);
	layout->addWidget(pIntBox);
	setLayout(layout);


// 	commentCheckBox = new QCheckBox(tr("&Start generated files with a comment"));
// 	commentCheckBox->setChecked(true);
// 
// 	protectCheckBox = new QCheckBox(tr("&Protect header file against multiple inclusions"));
// 	protectCheckBox->setChecked(true);
// 
// 	macroNameLabel = new QLabel(tr("&Macro name:"));
// 	macroNameLineEdit = new QLineEdit;
// 	macroNameLabel->setBuddy(macroNameLineEdit);
// 
// 	includeBaseCheckBox = new QCheckBox(tr("&Include base class definition"));
// 	baseIncludeLabel = new QLabel(tr("Base class include:"));
// 	baseIncludeLineEdit = new QLineEdit;
// 	baseIncludeLabel->setBuddy(baseIncludeLineEdit);
// 
// 	connect(protectCheckBox, SIGNAL(toggled(bool)), macroNameLabel, SLOT(setEnabled(bool)));
// 	connect(protectCheckBox, SIGNAL(toggled(bool)), macroNameLineEdit, SLOT(setEnabled(bool)));
// 	connect(includeBaseCheckBox, SIGNAL(toggled(bool)), baseIncludeLabel, SLOT(setEnabled(bool)));
// 	connect(includeBaseCheckBox, SIGNAL(toggled(bool)), baseIncludeLineEdit, SLOT(setEnabled(bool)));
// 
// 	registerField("comment", commentCheckBox);
// 	registerField("protect", protectCheckBox);
// 	registerField("macroName", macroNameLineEdit);
// 	registerField("includeBase", includeBaseCheckBox);
// 	registerField("baseInclude", baseIncludeLineEdit);
// 
// 	QGridLayout *layout = new QGridLayout;
// 	layout->setColumnMinimumWidth(0, 20);
// 	layout->addWidget(commentCheckBox, 0, 0, 1, 3);
// 	layout->addWidget(protectCheckBox, 1, 0, 1, 3);
// 	layout->addWidget(macroNameLabel, 2, 1);
// 	layout->addWidget(macroNameLineEdit, 2, 2);
// 	layout->addWidget(includeBaseCheckBox, 3, 0, 1, 3);
// 	layout->addWidget(baseIncludeLabel, 4, 1);
// 	layout->addWidget(baseIncludeLineEdit, 4, 2);
// 	setLayout(layout);
}

void ParametersPage::initializePage()
{


// 	QString pluginName = field("pluginName").toString();
// 	macroNameLineEdit->setText(pluginName.toUpper() + "_H");
// 
// 	QString baseClass = field("baseClass").toString();
// 
// 	includeBaseCheckBox->setChecked(!baseClass.isEmpty());
// 	includeBaseCheckBox->setEnabled(!baseClass.isEmpty());
// 	baseIncludeLabel->setEnabled(!baseClass.isEmpty());
// 	baseIncludeLineEdit->setEnabled(!baseClass.isEmpty());
// 
// 	if (baseClass.isEmpty()) {
// 		baseIncludeLineEdit->clear();
// 	} else if (QRegExp("Q[A-Z].*").exactMatch(baseClass)) {
// 		baseIncludeLineEdit->setText("<" + baseClass + ">");
// 	} else {
// 		baseIncludeLineEdit->setText("\"" + baseClass.toLower() + ".h\"");
// 	}
}
//! [16]

OutputFilesPage::OutputFilesPage(QWidget *parent)
: QWizardPage(parent)
{
	setTitle(tr("Output Files"));
	setSubTitle(tr("Specify where you want the wizard to put the generated "
		"skeleton code."));
	setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo3.png"));

	outputDirLabel = new QLabel(tr("&Output directory:"));
	outputDirLineEdit = new QLineEdit;
	outputDirLabel->setBuddy(outputDirLineEdit);

	headerLabel = new QLabel(tr("&Header file name:"));
	headerLineEdit = new QLineEdit;
	headerLabel->setBuddy(headerLineEdit);

	implementationLabel = new QLabel(tr("&Implementation file name:"));
	implementationLineEdit = new QLineEdit;
	implementationLabel->setBuddy(implementationLineEdit);

	registerField("outputDir*", outputDirLineEdit);
	registerField("header*", headerLineEdit);
	registerField("implementation*", implementationLineEdit);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(outputDirLabel, 0, 0);
	layout->addWidget(outputDirLineEdit, 0, 1);
	layout->addWidget(headerLabel, 1, 0);
	layout->addWidget(headerLineEdit, 1, 1);
	layout->addWidget(implementationLabel, 2, 0);
	layout->addWidget(implementationLineEdit, 2, 1);
	setLayout(layout);
}

//! [17]
void OutputFilesPage::initializePage()
{
	QString pluginName = field("pluginName").toString();
	headerLineEdit->setText(pluginName.toLower() + ".h");
	implementationLineEdit->setText(pluginName.toLower() + ".cpp");
	outputDirLineEdit->setText(QDir::convertSeparators(QDir::tempPath()));
}
//! [17]

ConclusionPage::ConclusionPage(QWidget *parent)
: QWizardPage(parent)
{
	setTitle(tr("Conclusion"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark2.png"));

	label = new QLabel;
	label->setWordWrap(true);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	setLayout(layout);
}

void ConclusionPage::initializePage()
{
	QString finishText = wizard()->buttonText(QWizard::FinishButton);
	finishText.remove('&');
	label->setText(tr("Click %1 to generate the class skeleton.")
		.arg(finishText));
}
