#include "ModelImporterInspector.hpp"

#include "UIAssetHeader.hpp"
#include "UIBool.hpp"
#include "UIComboBox.hpp"
#include "UIRevertApplyButtons.hpp"
#include "UIFloat.hpp"

#include <QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QMessageBox>

#include <ModelImporter.hpp>

#include "../AssetDataBase.hpp"
#include "generate/Enum_ModelImporterNormals.hpp"
#include "generate/Enum_ModelImporterTangents.hpp"
#include "generate/Enum_ModelImporterMaterialSearch.hpp"

using namespace FishEditor;
using namespace FishEngine;

template<class T>
inline UIComboBox * CreateCombox(const char* name)
{
	static_assert(std::is_enum<T>(), "T must be enum type!");
	return new UIComboBox(name, 0, EnumToCStringArray<T>(), EnumCount<T>());
};

ModelImporterInspector::ModelImporterInspector(QWidget *parent) : QWidget(parent)
{
//	EditorGUI::FloatField("File Scale", &importer->m_fileScale);
//	EditorGUI::EnumPopup("Normals", &importer->m_importNormals);
//	EditorGUI::EnumPopup("Tangents", &importer->m_importTangents);
//	EditorGUI::EnumPopup("Material Search", &importer->m_materialSearch);
	m_verticalLayout = new QVBoxLayout(this);
	m_verticalLayout->setSpacing(0);
	m_verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
	m_verticalLayout->setContentsMargins(4, 0, 4, 0);
	m_assetHeader = new UIAssetHeader(this);
	m_assetHeader->setObjectName(QStringLiteral("widget"));
	
	m_verticalLayout->addWidget(m_assetHeader);
	
	m_globalScaleEdit = new UIFloat("Scale Factor", 0.0f, this);
	m_verticalLayout->addWidget(m_globalScaleEdit);
	m_fileScaleEdit = new UIFloat("File Scale", 0.0f, this);
	m_fileScaleEdit->setEnabled(false);
	m_verticalLayout->addWidget(m_fileScaleEdit);
	m_normalsCombox = CreateCombox<decltype(ModelImporter::m_importNormals)>("Normals");
	m_verticalLayout->addWidget(m_normalsCombox);
	m_tangentsCombox = CreateCombox<decltype(ModelImporter::m_importTangents)>("Tangents");
	m_verticalLayout->addWidget(m_tangentsCombox);
	m_materialSearchCombox = CreateCombox<decltype(ModelImporter::m_materialSearch)>("Material Search");
	m_verticalLayout->addWidget(m_materialSearchCombox);
	
	m_revertApplyButtons = new UIRevertApplyButtons();
	m_verticalLayout->addWidget(m_revertApplyButtons);
	
	auto verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	m_verticalLayout->addItem(verticalSpacer);
	
	m_cachedImporter = std::make_unique<ModelImporter>();

	connect(m_globalScaleEdit,
			&UIFloat::ValueChanged,
			[this](float value) {
				m_cachedImporter->m_globalScale = value;
				this->SetDirty(true);
			});
	
//	connect(m_fileScaleEdit,
//			&UIFloat::ValueChanged,
//			[this](float value) {
//				m_cachedImporter->m_fileScale = value;
//				this->SetDirty(true);
//			});
	
	connect(m_normalsCombox,
			&UIComboBox::OnValueChanged,
			[this](int index) {
				m_cachedImporter->m_importNormals = FishEngine::ToEnum<decltype(m_cachedImporter->m_importNormals)>(index);
				this->SetDirty(true);
			});
	
	connect(m_tangentsCombox,
			&UIComboBox::OnValueChanged,
			[this](int index) {
				m_cachedImporter->m_importTangents = FishEngine::ToEnum<decltype(m_cachedImporter->m_importTangents)>(index);
				this->SetDirty(true);
			});
	
	connect(m_materialSearchCombox,
			&UIComboBox::OnValueChanged,
			[this](int index) {
				m_cachedImporter->m_materialSearch = FishEngine::ToEnum<decltype(m_cachedImporter->m_materialSearch)>(index);
				this->SetDirty(true);
			});

	
	connect(m_revertApplyButtons, &UIRevertApplyButtons::OnRevert, this, &ModelImporterInspector::Revert);
	
	connect(m_revertApplyButtons, &UIRevertApplyButtons::OnApply, this, &ModelImporterInspector::Apply);
}

ModelImporterInspector::~ModelImporterInspector()
{
}

void ModelImporterInspector::SetDirty(bool dirty)
{
	m_isDirty = dirty;
	this->m_revertApplyButtons->SetEnabled(m_isDirty);
}

void ModelImporterInspector::Apply()
{
	SetDirty(false);
	*m_importer = *m_cachedImporter;
	m_importer->SaveAndReimport();
}

void ModelImporterInspector::Revert()
{
	//SetDirty(false);
	auto importer = m_importer;
	m_importer = nullptr;
	Bind(importer);
}

void ModelImporterInspector::Bind(std::shared_ptr<FishEditor::ModelImporter> const & importer)
{
	if (isHidden())
	{
		setHidden(false);
	}
	
	if (m_importer != importer)
	{
		SetDirty(false);
		m_importer = importer;
		m_assetHeader->SetName(m_importer->name());
		auto const & icon = AssetDatabase::GetCacheIcon(importer->assetPath());
		m_assetHeader->SetIcon(icon);
		
		*m_cachedImporter = *m_importer;
		m_globalScaleEdit->SetValue(m_cachedImporter->m_globalScale);
		m_fileScaleEdit->SetValue(m_cachedImporter->m_fileScale);
		int index = FishEngine::EnumToIndex(m_cachedImporter->m_importNormals);
		m_normalsCombox->SetValue(index);
		index = FishEngine::EnumToIndex(m_cachedImporter->m_importTangents);
		m_tangentsCombox->SetValue(index);
		index = FishEngine::EnumToIndex(m_cachedImporter->m_materialSearch);
		m_materialSearchCombox->SetValue(index);
	}
}

void ModelImporterInspector::Unbind()
{
	if (m_isDirty)
	{
		QString path = QString::fromStdString( m_importer->assetPath().string() );
		QString message = "Unapplied import settings for " + path;
		int ret = QMessageBox::warning(this,
									   "Unapplied import settings",
									   message,
									   QMessageBox::Discard | QMessageBox::Apply,
									   QMessageBox::Apply);
		ret == QMessageBox::Apply ? Apply() : Revert();
		m_isDirty = false;
	}
	m_importer = nullptr;
	if (!isHidden())
	{
		setHidden(true);
		m_revertApplyButtons->SetEnabled(false);
	}
}
