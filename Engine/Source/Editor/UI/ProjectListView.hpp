#pragma once

#include <QListView>

class ProjectListView : public QListView
{
	Q_OBJECT

public:
	explicit ProjectListView(QWidget *parent = 0);
	~ProjectListView();

protected:
    //virtual void dragEnterEvent(QDragEnterEvent *e) override;
    //virtual void dragMoveEvent(QDragMoveEvent *e) override;
	//virtual void dropEvent(QDropEvent *e) override;
};
