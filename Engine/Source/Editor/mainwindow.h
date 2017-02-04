#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //bool eventFilter(QObject *watched, QEvent *event);
    void Init();

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
