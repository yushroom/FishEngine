#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

namespace Ui {
class MainWindow;
}

class GameObjectInspector;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

private:

    bool eventFilter(QObject *watched, QEvent *event);

    void Init();

    GameObjectInspector * m_gameObjectInspector;

    //QStandardItemModel * model;

    Ui::MainWindow *ui;

    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
};

#endif // MAINWINDOW_H
