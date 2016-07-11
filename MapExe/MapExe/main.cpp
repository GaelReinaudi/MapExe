#include "mapexe.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	// for autorun after install http://blogs.msdn.com/b/astebner/archive/2006/08/12/696833.aspx
	// and http://blogs.msdn.com/b/astebner/archive/2006/10/23/mailbag-how-to-set-the-noimpersonate-flag-for-a-custom-action-in-visual-studio-2005.aspx?PageIndex=3

	QApplication a(argc, argv);
    //a.setWindowIcon(QIcon("../Resources/Basalt.icns"));
	// sets the working directory to the bin path, even if the app was started from somewhere else (like, see above).
	QDir::setCurrent(QCoreApplication::applicationDirPath());
//	a.setStyle( new QCleanlooksStyle( ));
//	QApplication::setGraphicsSystem("raster");
	MapExe w;
	w.show();
	return a.exec();
}
