#include "UI/MainWindow.hpp"
#include <QApplication>
#include <QSurfaceFormat>
#include "UI/OpenProjectDialog.hpp"

int main(int argc, char *argv[])
{
	//FishEngine::Resources::SetAssetsDirectory("/Users/yushroom/program/graphics/FishEngine/Example/Sponza");
	QApplication a(argc, argv);
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
