#pragma once

#include <QListView>

class ProjectViewFileModel;

class ProjectListView : public QListView
{
	Q_OBJECT

public:
	explicit ProjectListView(QWidget *parent = 0);
	~ProjectListView();

	void setFileModel(ProjectViewFileModel * model)
	{
		m_fileModel = model;
	}

protected:
	void ShowContexMenu(const QPoint&);
	//virtual void mousePressEvent(QMouseEvent *event) override;
    //virtual void dragEnterEvent(QDragEnterEvent *e) override;
    //virtual void dragMoveEvent(QDragMoveEvent *e) override;
	//virtual void dropEvent(QDropEvent *e) override;

	void MakeDir();
	void ShowInExplorer();

	ProjectViewFileModel    * m_fileModel;

	QMenu * m_menu;
	QAction * m_actionDelete;
	QAction * m_actionOpen;
	QAction * m_actionShowInExplorer;

	QAction * m_actionMoveToScene;
};
