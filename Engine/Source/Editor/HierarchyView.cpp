#include "HierarchyView.hpp"
#include "ui_HierarchyView.h"

#include <QStandardItemModel>
#include <QTimer>

#include <Transform.hpp>
#include <GameObject.hpp>
#include "Selection.hpp"
#include <Scene.hpp>

using namespace FishEngine;
using namespace FishEditor;

Q_DECLARE_METATYPE(std::weak_ptr<FishEngine::Transform>);

HierarchyView::HierarchyView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HierarchyView)
{
    ui->setupUi(this);

    m_hierarchyModel = new QStandardItemModel();

    m_action1 = new QAction("Create Empty");
    connect(m_action1, SIGNAL(triggered()), this, SLOT(CreateEmpty()));
    ui->hierarchyTreeView->addAction(m_action1);
    ui->hierarchyTreeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->hierarchyTreeView->setModel(m_hierarchyModel);
    auto selectionModel = ui->hierarchyTreeView->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(OnHierarchyViewSelectionChanged(QItemSelection,QItemSelection)));

    auto timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateHierarchyModel()));
    timer->start(1000 / 10.0f); // 10 fps
}

HierarchyView::~HierarchyView()
{
    delete ui;
}


//void HierarchyView::UpdateHierarchyView()
//{
//    //auto model = ui->sceneView->m_hierarchyModel;
//}

void HierarchyView::OnHierarchyViewSelectionChanged(QItemSelection const & current,QItemSelection const & previous)
{
    if (m_blockSignal)
        return;
    //Debug::LogError("OnHierarchyViewSelectionChanged %d", current.size());
    //Debug::LogError("OnHierarchyViewSelectionChanged %d", ui->hierarchyTreeView->selectionModel()->selection().size());
    auto const & selections = ui->hierarchyTreeView->selectionModel()->selectedIndexes();
    //auto const & selections = current.indexes();
    if (selections.empty())
    {
        FishEditor::Selection::setTransforms({});
    }
    else
    {
        std::list<std::weak_ptr<FishEngine::Transform>> selected_transform;
        for (auto const & index : selections)
        {
            auto item = m_hierarchyModel->itemFromIndex(index);
            auto transform_ptr = item->data().value<std::weak_ptr<FishEngine::Transform>>();
            selected_transform.push_back(transform_ptr);
        }
        Selection::setTransforms(selected_transform);
        //Debug::LogError("OnHierarchyViewSelectionChanged[end] %d", Selection::transforms().size());
    }
}


QStandardItem* CreateHierarchyItem(TransformPtr const & transform)
{
    QStandardItem* item = new QStandardItem(QString(transform->gameObject()->name().c_str()));
    QVariant v;
    std::weak_ptr<FishEngine::Transform> pt = transform;
    v.setValue(pt);
    item->setData(v);
    for (auto const & child : transform->children())
    {
        auto c = child.lock();
        if (c != nullptr)
            item->appendRow( CreateHierarchyItem(c) );
        else
            abort();
    }
    return item;
}

QStandardItem* HierarchyView::UpdateHierarchyItem(QStandardItem * item, TransformPtr const & transform)
{
    if (item == nullptr)    // new
    {
        return CreateHierarchyItem(transform);
    }

    auto transform_in_data = item->data().value<std::weak_ptr<FishEngine::Transform>>();
    if (transform_in_data.lock() != transform)
    {
        item->setText(QString(transform->gameObject()->name().c_str()));
        QVariant v;
        std::weak_ptr<FishEngine::Transform> pt = transform;
        v.setValue(pt);
        item->setData(v);
    }
    else
    {
        auto display_name = item->text();
        QString name = transform->gameObject()->name().c_str();
        if (display_name != name)
        {
            item->setText(name);
        }
    }

    // TODO: remove additional rows
    int row = 0;
    for (auto const & child : transform->children())
    {
        auto child_transform = child.lock();
        if (child_transform != nullptr)
        {
            auto child_item = item->child(row);
            if (child_item == nullptr)
            {
                auto new_item = UpdateHierarchyItem(nullptr, child_transform);
                item->setChild(row, 0, new_item);
            }
            else
            {
                UpdateHierarchyItem(child_item, child_transform);
            }
        }
        else
        {
            abort();
        }
        row ++;
    }
    if (Selection::Contains(transform))
    {
        ui->hierarchyTreeView->selectionModel()->select(item->index(), QItemSelectionModel::Select);
    }
    return item;
}

void HierarchyView::UpdateHierarchyModel()
{
    //Debug::LogError("Here %d", Selection::transforms().size());
    //auto count = m_hierarchyModel->rowCount();
    //ui->hierarchyTreeView->selectionModel()->blockSignals(true);
    m_blockSignal = true;
    ui->hierarchyTreeView->selectionModel()->clearSelection();

    int row = 0;
    for (auto go : Scene::GameObjects())
    {
        auto transform = go->transform();
        if (transform->parent() != nullptr)
        {
            continue;
        }
        auto item = m_hierarchyModel->item(row);
        if (item == nullptr)
        {
            //m_hierarchyModel->appendRow(UpdateHierarchyItem(nullptr, transform));
            auto new_item = UpdateHierarchyItem(nullptr, transform);
            m_hierarchyModel->setItem(row, new_item);
        }
        else
        {
            UpdateHierarchyItem(item, transform);
        }
        row++;
        //ui->hierarchyTreeView->selectionModel()->sele
    }
    //ui->hierarchyTreeView->update();
    //ui->hierarchyTreeView->selectionModel()->blockSignals(false);
    m_blockSignal = false;
}

void HierarchyView::CreateEmpty()
{
    auto const & selections = Selection::transforms();
    if (selections.empty())
    {
        Scene::CreateGameObject("GameObject");
    }
    else
    {
        //FishEngine::Debug::LogError("CreateEmpty");
        for (auto & t : Selection::transforms())
        {
            auto go = Scene::CreateGameObject("GameObject");
            go->transform()->SetParent(t.lock());
        }
    }
}

//QStandardItem* CreateHierarchyItem(TransformPtr const & transform)
//{
//    QStandardItem* item = new QStandardItem(QString(transform->gameObject()->name().c_str()));
//    QVariant v;
//    std::weak_ptr<FishEngine::Transform> pt = transform;
//    v.setValue(pt);
//    item->setData(v);
//    for (auto const & child : transform->children())
//    {
//        auto c = child.lock();
//        if (c != nullptr)
//            item->appendRow( CreateHierarchyItem(c) );
//    }
//    //m_hierarchyModel->appendRow(item);
//    return item;
//}
