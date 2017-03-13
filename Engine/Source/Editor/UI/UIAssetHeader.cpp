#include "UIAssetHeader.hpp"
#include "ui_UIAssetHeader.h"

#include "UIDebug.hpp"

UIAssetHeader::UIAssetHeader(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UIAssetHeader)
{
	ui->setupUi(this);
	m_name = ui->name->text().toStdString();
}

UIAssetHeader::~UIAssetHeader()
{
	delete ui;
}

void UIAssetHeader::CheckUpdate(const std::string &name)
{
	if (m_name != name)
	{
		m_name = name;
		LOG;
		ui->name->setText(QString::fromStdString(name));
	}
}

void UIAssetHeader::SetName(std::string const & name)
{
	if (m_name != name)
	{
		m_name = name;
		LOG;
		ui->name->setText(QString::fromStdString(name));
	}
}

void UIAssetHeader::SetIcon(QIcon const & icon)
{
	ui->icon->setPixmap(icon.pixmap(ui->icon->width(), ui->icon->height()));
}
