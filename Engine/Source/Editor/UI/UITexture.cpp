#include "UITexture.hpp"
#include "ui_UITexture.h"

#include "UIDebug.hpp"

#include "AssetImporter.hpp"
#include "AssetDataBase.hpp"

UITexture::UITexture(const std::string &label, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UITexture),
	m_label(label)
{
	ui->setupUi(this);
	ui->horizontalLayout->setContentsMargins(0, 2, 2, 2);
	ui->label->setText(QString::fromStdString(label));

	connect(ui->iconLabel, &ClickableLabel::clicked, this, &UITexture::OnTextureClicked);
	connect(ui->selectButton, &QPushButton::clicked, this, &UITexture::OnSelectButtonClicked);
}

UITexture::~UITexture()
{
	delete ui;
}

UITextureState UITexture::CheckUpdate(const std::string &label, const int textureInstanceID)
{
	if (m_label != label)
	{
		LogInfo("[UITexture] new label: " + m_label);
		m_label = label;
		LOG;
		ui->label->setText(m_label.c_str());
	}

	if (textureInstanceID != m_textureInstanceID)
	{
		m_textureInstanceID = textureInstanceID;
		auto path = FishEditor::AssetDatabase::GetAssetPath(textureInstanceID);
		auto const & icon = FishEditor::AssetDatabase::GetCacheIcon(path);
		ui->iconLabel->setPixmap(icon.pixmap(64, 64));
	}

	if ( m_textureClicked )
	{
		m_textureClicked = false;
		return UITextureState::TextureClicked;
	}
	if ( m_selectButtonClicked )
	{
		m_selectButtonClicked = false;
		return UITextureState::SelectButtonClicked;
	}

	return UITextureState::None;
}
