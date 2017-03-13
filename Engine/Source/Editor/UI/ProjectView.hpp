#ifndef PROJECTVIEW_HPP
#define PROJECTVIEW_HPP

#include <QWidget>

namespace Ui {
class ProjectView;
}

class ProjectViewFileModel;
class ProjectViewDirModel;

class ProjectView : public QWidget
{
	Q_OBJECT

public:
	explicit ProjectView(QWidget *parent = 0);
	~ProjectView();

	virtual QSize sizeHint() const override;

	void SetRootPath(std::string const & path);
	void SetRootPath(QString const & path);

private:
	void OnDirTreeViewSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void OnListViewDoubleClicked(const QModelIndex &index);
	//void OnListTreeViewClicked(const QModelIndex &index);
	void OnListViewSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void OnIconSizeChanged(int size);

private:
	Ui::ProjectView         * ui;

	ProjectViewDirModel     * m_dirModel;
	ProjectViewFileModel    * m_fileModel;

	int m_listViewIconSize = 16;
};

#endif // PROJECTVIEW_HPP
