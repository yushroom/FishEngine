#include "GameObjectInspector.hpp"
#include "ui_GameObjectInspector.h"

#include <QTreeWidget>
#include <QTimer>

#include "Selection.hpp"
#include "EditorGUI.hpp"

#include <GameObject.hpp>
#include <LayerMask.hpp>
#include <TagManager.hpp>

#include "UIDebug.hpp"

//typedef std::weak_ptr<FishEngine::Component> ComponentRef;
////Q_DECLARE_METATYPE(ComponentRef);

GameObjectInspector::GameObjectInspector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameObjectInspector)
{
    ui->setupUi(this);

//    FishEditor::Selection::activeObjectChanged += [this]() {
//        FishEngine::Debug::Log("1");
//        this->UpdateInspector();
//    };
    connect(ui->name,  SIGNAL(editingFinished()),        this, SLOT(OnNameChanged()));
    connect(ui->layer, SIGNAL(currentIndexChanged(int)), this, SLOT(OnLayerChanged(int)));
    connect(ui->tag,   SIGNAL(currentIndexChanged(int)), this, SLOT(OnTagChanged(int)));
    connect(ui->activeCheckBox, SIGNAL(toggled(bool)),   this, SLOT(OnActiveCheckBoxChanged(bool)));

    UpdateInspector();

    auto timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateInspector()));
    timer->start(1000 / 15.0f); // 15 fps
}

GameObjectInspector::~GameObjectInspector()
{
    delete ui;
}


void GameObjectInspector::UpdateInspector()
{
    auto go = FishEditor::Selection::activeGameObject();

    if (go == nullptr)
    {
        hide();
        return;
    }

    if (m_changed)
    {
        Debug::LogError("[UpdateInspector] changed from UI");
        go->setName(m_name);
        go->setLayer(m_layerIndex);
        //go->setTag();
        go->m_tagIndex = m_tagIndex;
        go->SetActive(m_isActive);
        m_changed = false;
        goto SKIP_EDITOR;
    }

    if (m_isActive != go->activeSelf())
    {
        m_isActive = go->activeSelf();
        LOG;
        ui->activeCheckBox->blockSignals(true);
        ui->activeCheckBox->setChecked(m_isActive);
        ui->activeCheckBox->blockSignals(false);
    }

    if (m_name != go->name())
    {
        m_name = go->name();
        LOG;
        ui->name->blockSignals(true);
        ui->name->setText(m_name.c_str());
        ui->name->blockSignals(true);
    }

    // update Layer items
    if (m_layerDirtyFlag != FishEngine::LayerMask::s_editorFlag)
    {
        Debug::LogError("[UpdateInspector] Update Layers");
        QStringList layers;
        auto const & layerNames = FishEngine::LayerMask::allLayerNames();
        for (auto const & n : layerNames)
        {
            if (!n.empty())
                layers << n.c_str();
        }
        ui->layer->blockSignals(true);
        ui->layer->clear();
        ui->layer->addItems(layers);
        ui->layer->blockSignals(false);
        m_layerDirtyFlag = FishEngine::LayerMask::s_editorFlag;
    }

    if (m_layerIndex != go->layer())
    {
        m_layerIndex = go->layer();
        LOG;
        ui->layer->blockSignals(true);
        ui->layer->setCurrentIndex(m_layerIndex);
        ui->layer->blockSignals(false);
    }

    // update Tag items
    if (m_tagDirtyFlag != FishEngine::TagManager::s_editorFlag)
    {
        Debug::LogError("[UpdateInspector] Update Tags");
        QStringList tags;
        //auto const & all_tags = FishEngine::TagManager::s_tags;
        for (auto const & n : FishEngine::TagManager::s_tags)
        {
            if (!n.empty())
                tags << n.c_str();
        }
        ui->tag->blockSignals(true);
        ui->tag->clear();
        ui->tag->addItems(tags);
        ui->tag->blockSignals(false);
        m_tagDirtyFlag = FishEngine::TagManager::s_editorFlag;
    }

    if (m_tagIndex != go->m_tagIndex)
    {
        m_tagIndex = go->m_tagIndex;
        LOG;
        ui->tag->blockSignals(true);
        ui->tag->setCurrentIndex(m_tagIndex);
        ui->tag->blockSignals(false);
    }

//    ui->Tag->clear();
//    ui->Tag->addItem("Untagged");

SKIP_EDITOR:
    if (isHidden())
        show();

    FishEditor::EditorGUI::s_treeWidget = ui->ComponentArea;
    FishEditor::EditorGUI::BindGameObject(go);
}

void GameObjectInspector::OnNameChanged()
{
    auto name = ui->name->text().toStdString();
    if (m_name != name)
    {
        m_name = name;
        LOG;
        m_changed = true;
    }
}

void GameObjectInspector::OnActiveCheckBoxChanged(bool active)
{
    m_isActive = active;
    LOG;
    m_changed = true;
}

void GameObjectInspector::OnLayerChanged(int index)
{
    m_layerIndex = index;
    LOG;
    m_changed = true;
}

void GameObjectInspector::OnTagChanged(int index)
{
    m_tagIndex = index;
    LOG;
    m_changed = true;
}
