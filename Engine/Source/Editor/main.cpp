#include <QApplication>
#include <QSurfaceFormat>
#include "UI/MenuStyle.hpp"
#include "UI/OpenProjectDialog.hpp"
#include "UI/MainWindow.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// http://stackoverflow.com/questions/37020992/qt-prevent-menubar-from-grabbing-focus-after-alt-pressed-on-windows
	a.setStyle(new MenuStyle);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(4, 1);
	format.setProfile(QSurfaceFormat::CoreProfile);
	//format.setSamples(4);
	QSurfaceFormat::setDefaultFormat(format);

	OpenProjectDialog dialog;
	int result = dialog.exec();
	if (result == 0)
	{
		return 0;
	}

	MainWindow w;
	w.show();

	return a.exec();
}
