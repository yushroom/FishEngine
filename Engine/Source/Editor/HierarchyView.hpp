#ifndef HIERARCHYVIEW_HPP
#define HIERARCHYVIEW_HPP

#include <QWidget>
#include <QAction>

namespace Ui {
class HierarchyView;
}

namespace FishEngine
{
    class Transform;
}

class QStandardItemModel;
class QStandardItem;
class QItemSelection;

class HierarchyView : public QWidget
{
    Q_OBJECT

public:
    explicit HierarchyView(QWidget *parent = 0);
    ~HierarchyView();

public slots:
    //void UpdateHierarchyView();
    void OnHierarchyViewSelectionChanged(QItemSelection const &,QItemSelection const &);
    void UpdateHierarchyModel();

    void CreateEmpty();
    //void Create

private:
    Ui::HierarchyView *ui;

    bool m_blockSignal = false;

    QAction * m_action1;

    //QStandardItem* UpdateHierarchyItem(std::shared_ptr<FishEngine::Transform> const & transform);
    QStandardItemModel * m_hierarchyModel;

    QStandardItem* UpdateHierarchyItem(QStandardItem * item, std::shared_ptr<FishEngine::Transform> const & transform);
};

#endif // HIERARCHYVIEW_HPP
