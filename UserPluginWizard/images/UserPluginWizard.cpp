#include <QtGui>

#include "UserPluginWizard.h"

UserPluginWizard::UserPluginWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new ClassInfoPage);
    addPage(new CodeStylePage);
    addPage(new OutputFilesPage);
    addPage(new ConclusionPage);

	setWizardStyle(QWizard::ModernStyle);

    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(tr("User Plugin Wizard"));

	m_FolderCopyFilterOut << "Debug" << "Release" << "Release with Debug" << "GeneratedFiles";
	m_ExtensionCopyFilterOut << "ncb" << "sln" << "suo" << "png";
}

void UserPluginWizard::accept()
{
	QDir dirToUse;
	MakeListOfFilesToUse(QDir(""));

	MakeProjectDirectory(QDir(".."));

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
			m_FilesToCopy << QDir::toNativeSeparators(finfo.filePath());
		}
	}
}

void UserPluginWizard::MakeProjectDirectory( QDir inDir )
{
	QString pluginNameDir = field("pluginName").toString() + "Plugin";
	pluginNameDir = QDir::cleanPath(pluginNameDir);
	inDir.mkdir(pluginNameDir);
	inDir.cd(pluginNameDir);

	foreach(QString filePath, m_FilesToCopy) {
		// opens the file
		QFile file(filePath);
		if(!file.open(QFile::ReadOnly | QFile::Text)) {
			QMessageBox::warning(0, QObject::tr("Plugin Wizard"), QObject::tr("Cannot read file %1:\n%2").arg(file.fileName()).arg(file.errorString()));
			continue;
		}
		// reads the content
		QByteArray content;
		content = file.readAll();
		// replaces strings
		content = content.replace("UserPluginWizard", "Xyz");

		// open the new file
		QString newFilePath = inDir.filePath(QDir::cleanPath(file.fileName()));
		newFilePath = newFilePath.replace("UserPluginWizard", "Xyz");

		QFile newfile(newFilePath);
		if(!newfile.open(QFile::WriteOnly | QFile::Text)) {
			QMessageBox::warning(0, QObject::tr("Plugin Wizard"), QObject::tr("Cannot write file %1:\n%2").arg(newfile.fileName()).arg(newfile.errorString()));
			continue;
		}
		newfile.write(content);
	}
}

void UserPluginWizard::Oldaccept()
{
    QByteArray pluginName = field("pluginName").toByteArray();
    QByteArray baseClass = field("baseClass").toByteArray();
    QByteArray macroName = field("macroName").toByteArray();
    QByteArray baseInclude = field("baseInclude").toByteArray();

    QString outputDir = field("outputDir").toString();
    QString header = field("header").toString();
    QString implementation = field("implementation").toString();

    QByteArray block;

    if (field("comment").toBool()) {
        block += "/*\n";
        block += "    " + header.toAscii() + "\n";
        block += "*/\n";
        block += "\n";
    }
    if (field("protect").toBool()) {
        block += "#ifndef " + macroName + "\n";
        block += "#define " + macroName + "\n";
        block += "\n";
    }
    if (field("includeBase").toBool()) {
        block += "#include " + baseInclude + "\n";
        block += "\n";
    }

    block += "class " + pluginName;
    if (!baseClass.isEmpty())
        block += " : public " + baseClass;
    block += "\n";
    block += "{\n";

    /* qmake ignore Q_OBJECT */

    if (field("qobjectMacro").toBool()) {
        block += "    Q_OBJECT\n";
        block += "\n";
    }
    block += "public:\n";

    if (field("qobjectCtor").toBool()) {
        block += "    " + pluginName + "(QObject *parent = 0);\n";
    } else if (field("qwidgetCtor").toBool()) {
        block += "    " + pluginName + "(QWidget *parent = 0);\n";
    } else if (field("defaultCtor").toBool()) {
        block += "    " + pluginName + "();\n";
        if (field("copyCtor").toBool()) {
            block += "    " + pluginName + "(const " + pluginName + " &other);\n";
            block += "\n";
            block += "    " + pluginName + " &operator=" + "(const " + pluginName
                     + " &other);\n";
        }
    }
    block += "};\n";

    if (field("protect").toBool()) {
        block += "\n";
        block += "#endif\n";
    }

    QFile headerFile(outputDir + "/" + header);
    if (!headerFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Simple Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(headerFile.fileName())
                             .arg(headerFile.errorString()));
        return;
    }
    headerFile.write(block);

    block.clear();

    if (field("comment").toBool()) {
        block += "/*\n";
        block += "    " + implementation.toAscii() + "\n";
        block += "*/\n";
        block += "\n";
    }
    block += "#include \"" + header.toAscii() + "\"\n";
    block += "\n";

    if (field("qobjectCtor").toBool()) {
        block += pluginName + "::" + pluginName + "(QObject *parent)\n";
        block += "    : " + baseClass + "(parent)\n";
        block += "{\n";
        block += "}\n";
    } else if (field("qwidgetCtor").toBool()) {
        block += pluginName + "::" + pluginName + "(QWidget *parent)\n";
        block += "    : " + baseClass + "(parent)\n";
        block += "{\n";
        block += "}\n";
    } else if (field("defaultCtor").toBool()) {
        block += pluginName + "::" + pluginName + "()\n";
        block += "{\n";
        block += "    // missing code\n";
        block += "}\n";

        if (field("copyCtor").toBool()) {
            block += "\n";
            block += pluginName + "::" + pluginName + "(const " + pluginName
                     + " &other)\n";
            block += "{\n";
            block += "    *this = other;\n";
            block += "}\n";
            block += "\n";
            block += pluginName + " &" + pluginName + "::operator=(const "
                     + pluginName + " &other)\n";
            block += "{\n";
            if (!baseClass.isEmpty())
                block += "    " + baseClass + "::operator=(other);\n";
            block += "    // missing code\n";
            block += "    return *this;\n";
            block += "}\n";
        }
    }

    QFile implementationFile(outputDir + "/" + implementation);
    if (!implementationFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Simple Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(implementationFile.fileName())
                             .arg(implementationFile.errorString()));
        return;
    }
    implementationFile.write(block);

    QDialog::accept();
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

CodeStylePage::CodeStylePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Code Style Options"));
    setSubTitle(tr("Choose the formatting of the generated code."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo2.png"));

    commentCheckBox = new QCheckBox(tr("&Start generated files with a comment"));
    commentCheckBox->setChecked(true);

    protectCheckBox = new QCheckBox(tr("&Protect header file against multiple inclusions"));
    protectCheckBox->setChecked(true);

    macroNameLabel = new QLabel(tr("&Macro name:"));
    macroNameLineEdit = new QLineEdit;
    macroNameLabel->setBuddy(macroNameLineEdit);

    includeBaseCheckBox = new QCheckBox(tr("&Include base class definition"));
    baseIncludeLabel = new QLabel(tr("Base class include:"));
    baseIncludeLineEdit = new QLineEdit;
    baseIncludeLabel->setBuddy(baseIncludeLineEdit);

    connect(protectCheckBox, SIGNAL(toggled(bool)), macroNameLabel, SLOT(setEnabled(bool)));
    connect(protectCheckBox, SIGNAL(toggled(bool)), macroNameLineEdit, SLOT(setEnabled(bool)));
    connect(includeBaseCheckBox, SIGNAL(toggled(bool)), baseIncludeLabel, SLOT(setEnabled(bool)));
    connect(includeBaseCheckBox, SIGNAL(toggled(bool)), baseIncludeLineEdit, SLOT(setEnabled(bool)));

    registerField("comment", commentCheckBox);
    registerField("protect", protectCheckBox);
    registerField("macroName", macroNameLineEdit);
    registerField("includeBase", includeBaseCheckBox);
    registerField("baseInclude", baseIncludeLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->setColumnMinimumWidth(0, 20);
    layout->addWidget(commentCheckBox, 0, 0, 1, 3);
    layout->addWidget(protectCheckBox, 1, 0, 1, 3);
    layout->addWidget(macroNameLabel, 2, 1);
    layout->addWidget(macroNameLineEdit, 2, 2);
    layout->addWidget(includeBaseCheckBox, 3, 0, 1, 3);
    layout->addWidget(baseIncludeLabel, 4, 1);
    layout->addWidget(baseIncludeLineEdit, 4, 2);
    setLayout(layout);
}

void CodeStylePage::initializePage()
{
    QString pluginName = field("pluginName").toString();
    macroNameLineEdit->setText(pluginName.toUpper() + "_H");

    QString baseClass = field("baseClass").toString();

    includeBaseCheckBox->setChecked(!baseClass.isEmpty());
    includeBaseCheckBox->setEnabled(!baseClass.isEmpty());
    baseIncludeLabel->setEnabled(!baseClass.isEmpty());
    baseIncludeLineEdit->setEnabled(!baseClass.isEmpty());

    if (baseClass.isEmpty()) {
        baseIncludeLineEdit->clear();
    } else if (QRegExp("Q[A-Z].*").exactMatch(baseClass)) {
        baseIncludeLineEdit->setText("<" + baseClass + ">");
    } else {
        baseIncludeLineEdit->setText("\"" + baseClass.toLower() + ".h\"");
    }
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
