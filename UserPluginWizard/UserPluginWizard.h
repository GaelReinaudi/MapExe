#ifndef UserPluginWizard_H
#define UserPluginWizard_H

#include <QWizard>
#include <QDir>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QRadioButton;
QT_END_NAMESPACE

//! [0]
class UserPluginWizard : public QWizard
{
    Q_OBJECT

public:
    UserPluginWizard(QWidget *parent = 0);

	void accept();
	//! makes the list of files that we want to use to make the new project
	void MakeListOfFilesToUse(QDir fromDir);
	//! makes the new plugin project sub-directory in the provided directory. Returns the absolute path of the folder, or "" if couldn't make it.
	QString MakeProjectDirectory();

private:
	//! list of files to copy to the destination directory. Populated by MakeListOfFilesToUse()
	QStringList m_FilesToCopy;
	//! Names of folder that we discard
	QStringList m_FolderCopyFilterOut;
	//! extension of files not to be used
	QStringList m_ExtensionCopyFilterOut;
	//! directory from which we take the project to copy
	QDir m_FromDir;
	//! directory into which we put the new project sub-directory
	QDir m_ToDir;
	QString m_ToReplaceWithAgentName;
	QString m_ToReplaceWithGraphicsItemName;
	QString m_ToReplaceWithPluginName;
	QString m_ToReplaceWithProjectName;
	// 	//! extension of the files that have to be scanned and modified for the new project.
// 	QStringList m_ExtensionCopyFilterOut;
};
//! [0]

//! [1]
class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);

private:
    QLabel *label;
};
//! [1]

//! [2]
class ClassInfoPage : public QWizardPage
{
    Q_OBJECT

public:
    ClassInfoPage(QWidget *parent = 0);

private:
    QLabel *pluginNameLabel;
    QLabel *baseClassLabel;
    QLineEdit *pluginNameLineEdit;
    QLineEdit *baseClassLineEdit;
    QCheckBox *qobjectMacroCheckBox;
    QGroupBox *groupBox;
    QRadioButton *qobjectCtorRadioButton;
    QRadioButton *qwidgetCtorRadioButton;
    QRadioButton *defaultCtorRadioButton;
    QCheckBox *copyCtorCheckBox;
};
//! [2]

//! [3]
class ParametersPage : public QWizardPage
{
    Q_OBJECT

public:
    ParametersPage(QWidget *parent = 0);

protected:
    void initializePage();

private:
    QCheckBox *commentCheckBox;
    QCheckBox *protectCheckBox;
    QCheckBox *includeBaseCheckBox;
    QLabel *macroNameLabel;
    QLabel *baseIncludeLabel;
    QLineEdit *macroNameLineEdit;
    QLineEdit *baseIncludeLineEdit;
};
//! [3]

class OutputFilesPage : public QWizardPage
{
    Q_OBJECT

public:    
    OutputFilesPage(QWidget *parent = 0);

protected:
    void initializePage();

private:
    QLabel *outputDirLabel;
    QLabel *headerLabel;
    QLabel *implementationLabel;
    QLineEdit *outputDirLineEdit;
    QLineEdit *headerLineEdit;
    QLineEdit *implementationLineEdit;
};

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0);

protected:
    void initializePage();

private:
    QLabel *label;
};

#endif
