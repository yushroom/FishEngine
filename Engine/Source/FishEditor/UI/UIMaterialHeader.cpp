#include "UIMaterialHeader.hpp"
#include "ui_UIMaterialHeader.h"

#include "UIDebug.hpp"

UIMaterialHeader::UIMaterialHeader(QWidget *parent)
	: UIMaterialHeader("", parent)
{

}

UIMaterialHeader::UIMaterialHeader(std::string const & name, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UIMaterialHeader),
	m_name(name)
{
	ui->setupUi(this);
	//m_name = ui->name->text().toStdString();
	ui->name->setText(QString::fromStdString(name));
}

UIMaterialHeader::~UIMaterialHeader()
{
	delete ui;
}

void UIMaterialHeader::CheckUpdate(const std::string &name)
{
	if ( m_name != name)
	{
		m_name = name;
		LOG;
		ui->name->setText(QString::fromStdString(m_name));
	}
}
