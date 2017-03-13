#include "TextureImporterInspector.hpp"

#include "UIAssetHeader.hpp"
#include "UIBool.hpp"
#include "UIComboBox.hpp"
#include "UIRevertApplyButtons.hpp"

#include <QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QMessageBox>

#include <TextureImporter.hpp>

#include "../AssetDataBase.hpp"
#include "generate/Enum_TextureImporterType.hpp"
#include "generate/Enum_TextureImporterShape.hpp"
#include "generate/Enum_FilterMode.hpp"
#include "generate/Enum_TextureWrapMode.hpp"

using namespace FishEditor;
using namespace FishEngine;

template<class T>
inline UIComboBox * CreateCombox(const char* name)
{
	static_assert(std::is_enum<T>(), "T must be enum type!");
	return new UIComboBox(name, 0, EnumToCStringArray<T>(), EnumCount<T>());
};

TextureImporterInspector::TextureImporterInspector(QWidget *parent) :
	QWidget(parent)
{
	m_verticalLayout = new QVBoxLayout(this);
	m_verticalLayout->setSpacing(0);
	m_verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
	m_verticalLayout->setContentsMargins(4, 0, 4, 0);
	m_assetHeader = new UIAssetHeader(this);
	m_assetHeader->setObjectName(QStringLiteral("widget"));
	
	m_verticalLayout->addWidget(m_assetHeader);
	
	m_typeCombox = CreateCombox<TextureImporterType>("Texture Type");
	m_verticalLayout->addWidget(m_typeCombox);
	m_shapeCombox = CreateCombox<TextureImporterShape>("Texture Shape");
	m_verticalLayout->addWidget(m_shapeCombox);
	m_readWriteToggle = new UIBool("Read/Write Enabled", true);
	m_verticalLayout->addWidget(m_readWriteToggle);
	m_mipmapToggle = new UIBool("Generate Mip Maps", true);
	m_verticalLayout->addWidget(m_mipmapToggle);
	m_filterModeCombox = CreateCombox<FilterMode>("Filter Mode");
	m_verticalLayout->addWidget(m_filterModeCombox);
	m_wrapModeCombox = CreateCombox<TextureWrapMode>("Wrap Mode");
	m_verticalLayout->addWidget(m_wrapModeCombox);
	
	m_revertApplyButtons = new UIRevertApplyButtons();
	m_verticalLayout->addWidget(m_revertApplyButtons);
	
	auto verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	m_verticalLayout->addItem(verticalSpacer);
	
	m_cachedImporter = std::make_unique<TextureImporter>();
	
	connect(m_readWriteToggle,
			&UIBool::OnValueChanged,
			[this](bool value) {
				m_cachedImporter->m_isReadable = value;
				this->SetDirty(true);
			});
	
	connect(m_mipmapToggle,
			&UIBool::OnValueChanged,
			[this](bool value) {
				m_cachedImporter->m_mipmapEnabled = value;
				this->SetDirty(true);
			});
	
	
	connect(m_typeCombox,
			&UIComboBox::OnValueChanged,
			[this](int index) {
				m_cachedImporter->m_textureType = FishEngine::ToEnum<decltype(m_cachedImporter->m_textureType)>(index);
				this->SetDirty(true);
			});
	
	connect(m_shapeCombox,
			&UIComboBox::OnValueChanged,
			[this](int index) {
				m_cachedImporter->m_textureShape = FishEngine::ToEnum<decltype(m_cachedImporter->m_textureShape)>(index);
				this->SetDirty(true);
			});
	
	connect(m_filterModeCombox,
			&UIComboBox::OnValueChanged,
			[this](int index) {
				m_cachedImporter->m_textureSettings.m_filterMode = FishEngine::ToEnum<decltype(m_cachedImporter->m_textureSettings.m_filterMode)>(index);
				this->SetDirty(true);
			});
	
	connect(m_wrapModeCombox,
			&UIComboBox::OnValueChanged,
			[this](int index) {
				m_cachedImporter->m_textureSettings.m_wrapMode = FishEngine::ToEnum<decltype(m_cachedImporter->m_textureSettings.m_wrapMode)>(index);
				this->SetDirty(true);
			});
	
	connect(m_revertApplyButtons, &UIRevertApplyButtons::OnRevert, this, &TextureImporterInspector::Revert);
	
	connect(m_revertApplyButtons, &UIRevertApplyButtons::OnApply, this, &TextureImporterInspector::Apply);
}

TextureImporterInspector::~TextureImporterInspector()
{
}

void TextureImporterInspector::SetDirty(bool dirty)
{
	m_isDirty = dirty;
	this->m_revertApplyButtons->SetEnabled(m_isDirty);
}

void TextureImporterInspector::Apply()
{
	SetDirty(false);
	*m_importer = *m_cachedImporter;
	m_importer->SaveAndReimport();
}

void TextureImporterInspector::Revert()
{
	//SetDirty(false);
	auto importer = m_importer;
	m_importer = nullptr;
	Bind(importer);
}

void TextureImporterInspector::Bind(std::shared_ptr<FishEditor::TextureImporter> const & importer)
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
		m_readWriteToggle->SetValue(m_cachedImporter->m_isReadable);
		m_mipmapToggle->SetValue(m_cachedImporter->m_mipmapEnabled);
		int index = FishEngine::EnumToIndex(m_cachedImporter->m_textureType);
		m_typeCombox->SetValue(index);
		index = FishEngine::EnumToIndex(m_cachedImporter->m_textureShape);
		m_shapeCombox->SetValue(index);
		index = FishEngine::EnumToIndex(m_cachedImporter->filterMode());
		m_filterModeCombox->SetValue(index);
		index = FishEngine::EnumToIndex(m_cachedImporter->wrapMode());
		m_wrapModeCombox->SetValue(index);
	}
}

void TextureImporterInspector::Unbind()
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
