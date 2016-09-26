#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qtmdi); //name passend zu Projekt wählen

    QApplication app(argc, argv);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(0);
	format.setVersion(4, 4);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);
	
	MainWindow mainWin;
    mainWin.show();
    return app.exec();
	
}