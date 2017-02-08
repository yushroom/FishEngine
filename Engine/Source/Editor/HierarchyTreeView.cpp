#include "HierarchyTreeView.hpp"

#include <QStandardItemModel>
#include <QTimer>
#include <QMenu>

#include <Transform.hpp>
#include <GameObject.hpp>
#include "Selection.hpp"
#include <Scene.hpp>
#include <Camera.hpp>

#include <cassert>

#include "UIDebug.hpp"

#include <QDropEvent>
#include <QStandardItemModel>

#include "Selection.hpp"

#include <Transform.hpp>
#include <Debug.hpp>
using namespace FishEngine;
using namespace FishEditor;

Q_DECLARE_METATYPE(std::weak_ptr<FishEngine::Transform>)

void CreateEmpty()
{
    auto const & selections = Selection::transforms();
    if (selections.empty())
    {
        auto go = Scene::CreateGameObject("GameObject");
        //Selection::transforms().clear();
        //Selection::transforms().push_back(go->transform());
        Selection::setTransforms({go->transform()});
    }
    else
    {
        std::list<std::weak_ptr<Transform>> selections;
        //FishEngine::Debug::LogError("CreateEmpty");
        for (auto & t : Selection::transforms())
        {
            auto go = Scene::CreateGameObject("GameObject");
            go->transform()->SetParent(t.lock());
            selections.push_back(go->transform());
        }
        Selection::setTransforms(selections);
    }
}

void CreatePrimitive(PrimitiveType type)
{
    auto const & selections = Selection::transforms();
    if (selections.empty())
    {
        auto go = GameObject::CreatePrimitive(type);
        Selection::setTransforms({go->transform()});
    }
    else
    {
        std::list<std::weak_ptr<Transform>> selections;
        //FishEngine::Debug::LogError("CreateEmpty");
        for (auto & t : Selection::transforms())
        {
            auto go = GameObject::CreatePrimitive(type);
            go->transform()->SetParent(t.lock());
            selections.push_back(go->transform());
        }
        Selection::setTransforms(selections);
    }
}


HierarchyTreeView::HierarchyTreeView(QWidget *parent)
    : QTreeView(parent)
{
    m_hierarchyModel = new QStandardItemModel();

    QAction * action;
    m_menu = new QMenu(this);
    action = m_menu->addAction("Copy");
    action->setEnabled(false);
    action = m_menu->addAction("Paste");
    action->setEnabled(false);
    m_menu->addSeparator();
    action = m_menu->addAction("Rename");
    action->setEnabled(false);
    action = m_menu->addAction("Duplicate");
    action->setEnabled(false);
    m_deleteAction = m_menu->addAction("Delete");
    connect(m_deleteAction, &QAction::triggered, [](){
        for (auto const & t : Selection::transforms())
        {
            Object::DestroyImmediate(t.lock()->gameObject());
            //assert(t.expired());
        }
        //Selection::transforms().clear();
        Selection::setTransforms({});
    });
    m_menu->addSeparator();
    action = m_menu->addAction("Select Prefab");
    action->setEnabled(false);
    m_menu->addSeparator();
    action = m_menu->addAction("Create Empty");
    //connect(action, SIGNAL(triggered()), this, SLOT(CreateEmpty()));
    connect(action, &QAction::triggered, &CreateEmpty);
    QMenu* sub_menu = m_menu->addMenu("3D Object");
    action = sub_menu->addAction("Cube");
    connect(action, &QAction::triggered, [](){ CreatePrimitive(PrimitiveType::Cube); });
    action = sub_menu->addAction("Sphere");
    connect(action, &QAction::triggered, [](){ CreatePrimitive(PrimitiveType::Sphere); });
    action = sub_menu->addAction("Capsule");
    connect(action, &QAction::triggered, [](){ CreatePrimitive(PrimitiveType::Capsule); });
    action->setEnabled(false);
    action = sub_menu->addAction("Cylinder");
    connect(action, &QAction::triggered, [](){ CreatePrimitive(PrimitiveType::Cylinder); });
    action = sub_menu->addAction("Plane");
    connect(action, &QAction::triggered, [](){ CreatePrimitive(PrimitiveType::Plane); });
    action = sub_menu->addAction("Quad");
    connect(action, &QAction::triggered, [](){ CreatePrimitive(PrimitiveType::Quad); });
    action = sub_menu->addAction("Ragdoll");
    action->setEnabled(false);
    action = sub_menu->addAction("Terrain");
    action->setEnabled(false);
    action = sub_menu->addAction("Tree");
    action->setEnabled(false);
    action = sub_menu->addAction("Wind Zone");
    action->setEnabled(false);
    action = sub_menu->addAction("3D Text");
    action->setEnabled(false);
    sub_menu = m_menu->addMenu("2D Object");
    sub_menu->setEnabled(false);
    action = sub_menu->addAction("Sprite");
    action->setEnabled(false);
    sub_menu = m_menu->addMenu("Light");
    sub_menu->setEnabled(false);
    sub_menu->addAction("Directional Light");
    sub_menu->addAction("Point Light");
    sub_menu->addAction("Spotlight");
    sub_menu->addAction("Area Light");
    sub_menu->addAction("Reflection Probe");
    sub_menu->addAction("Light Probe");
    sub_menu = m_menu->addMenu("Audio");
    sub_menu->setEnabled(false);
    sub_menu->addAction("Audio Source");
    sub_menu->addAction("Audio Reverb Zone");
    sub_menu = m_menu->addMenu("UI");
    sub_menu->setEnabled(false);
    for (auto const & t : {"Text", "Image", "Raw Image", "Button", "Toggle", "Slider", "Scrollbar", "Dropdown", "Input Field", "Canvas", "Panel", "Scroll View", "Event System"})
    {
        action = sub_menu->addAction(t);
        action->setEnabled(false);
    }
    action = m_menu->addAction("Particle System");
    action->setEnabled(false);
    action = m_menu->addAction("Camera");
    connect(action, &QAction::triggered, [](){
        //auto camera = Camera::Create();
        Scene::CreateCamera();
    });

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContexMenu(const QPoint&)));
    setModel(m_hierarchyModel);
    //auto selectionModel = selectionModel();
    connect(selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(OnHierarchyViewSelectionChanged(QItemSelection,QItemSelection)));

    auto timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateHierarchyModel()));
    timer->start(1000 / 10.0f); // 10 fps
}

QSize HierarchyTreeView::sizeHint() const
{
    return QSize(200, 400);
}

void HierarchyTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    Debug::Log("HierarchyTreeView::dragEnterEvent");
    m_inDragDropMode = true;
    QTreeView::dragEnterEvent(event);
}

void HierarchyTreeView::dropEvent(QDropEvent *event)
{
    Debug::Log("HierarchyTreeView::dropEvent");
    m_inDragDropMode = false;
    const QModelIndex & index = indexAt(event->pos());

    if (!index.isValid()) // set null parent
    {
        Debug::Log("HierarchyTreeView::dropEvent, invalid");

        for (auto const & t : Selection::transforms())
        {
            t.lock()->SetParent(nullptr);
        }

        event->setDropAction(Qt::IgnoreAction);
        return;
    }

    auto to_item = m_hierarchyModel->itemFromIndex(index);
    auto new_parent = to_item->data().value<std::weak_ptr<FishEngine::Transform>>().lock();

    for (auto const & t : Selection::transforms())
    {
        t.lock()->SetParent(new_parent);
	}

    QTreeView::dropEvent(event);
}


//void HierarchyView::UpdateHierarchyView()
//{
//    //auto model = ui->sceneView->m_hierarchyModel;
//}

void HierarchyTreeView::OnHierarchyViewSelectionChanged(QItemSelection const & ,QItemSelection const & )
{
    if (m_blockSignal)
        return;
    //Debug::LogError("OnHierarchyViewSelectionChanged %d", current.size());
    //Debug::LogError("OnHierarchyViewSelectionChanged %d", ui->hierarchyTreeView->selectionModel()->selection().size());
    auto const & selections = selectionModel()->selectedIndexes();
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

QStandardItem* HierarchyTreeView::UpdateHierarchyItem(QStandardItem * item, TransformPtr const & transform)
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
    int redundantRows = item->rowCount() - row;
    if (redundantRows > 0)
    {
        item->removeRows(row, redundantRows);
    }
    if (Selection::Contains(transform))
    {
        selectionModel()->select(item->index(), QItemSelectionModel::Select);
    }
    return item;
}

void HierarchyTreeView::UpdateHierarchyModel()
{
    if (m_inDragDropMode)
        return;
    //LOG;
    //Debug::LogError("Here %d", Selection::transforms().size());
    //auto count = m_hierarchyModel->rowCount();
    //ui->hierarchyTreeView->selectionModel()->blockSignals(true);
    m_blockSignal = true;
    selectionModel()->clearSelection();

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

    int redundantRows = m_hierarchyModel->rowCount() - row;
    if (redundantRows > 0)
    {
        m_hierarchyModel->removeRows(row, redundantRows);
    }
    //ui->hierarchyTreeView->update();
    //ui->hierarchyTreeView->selectionModel()->blockSignals(false);
    m_blockSignal = false;
}

void HierarchyTreeView::ShowContexMenu(const QPoint &)
{
    if (Selection::transforms().empty())
        m_deleteAction->setEnabled(false);
    else
        m_deleteAction->setEnabled(true);
    m_menu->exec(QCursor::pos());
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
