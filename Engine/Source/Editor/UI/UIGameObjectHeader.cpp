#include "UIGameObjectHeader.hpp"
#include "ui_UIGameObjectHeader.h"

#include <FishEngine/GameObject.hpp>
#include <FishEngine/LayerMask.hpp>
#include <FishEngine/TagManager.hpp>

#include "UIDebug.hpp"

UIGameObjectHeader::UIGameObjectHeader(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UIGameObjectHeader)
{
	ui->setupUi(this);


	
	connect(ui->layer,
			static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
			this,
			&UIGameObjectHeader::OnLayerChanged);
	
	connect(ui->tag,
			static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
			this,
			&UIGameObjectHeader::OnTagChanged);
	
	connect(ui->activeCheckBox,
			&QCheckBox::toggled,
			this,
			&UIGameObjectHeader::OnActiveCheckBoxChanged);
	
	connect(ui->nameEdit,
			&QLineEdit::editingFinished,
			this,
			&UIGameObjectHeader::OnNameChanged);
}

UIGameObjectHeader::~UIGameObjectHeader()
{
	delete ui;
}

void UIGameObjectHeader::Bind(std::shared_ptr<FishEngine::GameObject> go)
{
	if (m_changed)
	{
		LogInfo("[UpdateInspector] changed from UI");
		go->setName(m_name);
		go->setLayer(m_layerIndex);
		//go->setTag();
		go->m_tagIndex = m_tagIndex;
		go->SetActive(m_isActive);
		m_changed = false;
		return;
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
		ui->nameEdit->blockSignals(true);
		ui->nameEdit->setText(m_name.c_str());
		ui->nameEdit->blockSignals(false);
	}

	// update Layer items
	if (m_layerDirtyFlag != FishEngine::LayerMask::s_editorFlag)
	{
		LogInfo("[UpdateInspector] Update Layers");
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
		LogInfo("[UpdateInspector] Update Tags");
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
}

void UIGameObjectHeader::OnNameChanged()
{
	auto name = ui->nameEdit->text().toStdString();
	if (m_name != name)
	{
		m_name = name;
		LOG;
		m_changed = true;
	}
}

void UIGameObjectHeader::OnActiveCheckBoxChanged(bool active)
{
	m_isActive = active;
	LOG;
	m_changed = true;
}

void UIGameObjectHeader::OnLayerChanged(int index)
{
	m_layerIndex = index;
	LOG;
	m_changed = true;
}

void UIGameObjectHeader::OnTagChanged(int index)
{
	m_tagIndex = index;
	LOG;
	m_changed = true;
}

