#ifndef HIERARCHYTREEVIEW_HPP
#define HIERARCHYTREEVIEW_HPP

#include <QTreeView>
#include <memory>

namespace FishEngine
{
	class Transform;
}

class QStandardItemModel;
class QStandardItem;
class QItemSelection;

class HierarchyTreeView : public QTreeView
{
	Q_OBJECT
public:
	explicit HierarchyTreeView(QWidget *parent = 0);

	virtual QSize sizeHint() const override;
	
	QAction * m_deleteAction;
	QAction * m_duplicateAction;

private slots:
	//void UpdateHierarchyView();
	void OnHierarchyViewSelectionChanged(QItemSelection const &,QItemSelection const &);
	void UpdateHierarchyModel();

	void ShowContexMenu(const QPoint&);

private:

	bool m_blockSignal = false;
	bool m_inDragDropMode = false;

	QMenu * m_menu;
	
	//QAction * m_createEmptyAction;
	//QAction * m_createCubeAction;
	//QAction * m_createSphereAction;
	//QAction * m_createCameraAction;

	//QStandardItem* UpdateHierarchyItem(std::shared_ptr<FishEngine::Transform> const & transform);
	QStandardItemModel * m_hierarchyModel;

	//void CreatePrimitive(PrimitiveType type);

	QStandardItem* UpdateHierarchyItem(QStandardItem * item, std::shared_ptr<FishEngine::Transform> const & transform);

	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual void dropEvent(QDropEvent *event) override;
};

#endif // HIERARCHYTREEVIEW_HPP
