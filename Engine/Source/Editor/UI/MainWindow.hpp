#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <boost/filesystem.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	//virtual void keyPressEvent(QKeyEvent * event) override;
	//virtual void keyReleaseEvent(QKeyEvent * event) override;

private:
	//bool eventFilter(QObject *watched, QEvent *event);
	void Init();
	
	bool CloseCurrentScene();

	void NewScene();
	void OpenScene();
	void SaveSceneAs();

	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
