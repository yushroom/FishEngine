#include "UIObjecField.hpp"
#include "ui_UIObjecField.h"

#include <Object.hpp>

#include "SelectObjectDialog.hpp"
#include "UIDebug.hpp"

UIObjecField::UIObjecField(std::string const & label, std::string const & objectName, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UIObjecField),
	m_label(label),
	m_objectName(objectName)
{
	ui->setupUi(this);
	ui->horizontalLayout->setContentsMargins(0, 2, 2, 2);

	ui->label->setText(m_label.c_str());
	ui->lineEdit->setText(objectName.c_str());
	connect(ui->picker, &QPushButton::clicked, this, &UIObjecField::OnPickerClicked);
}

UIObjecField::~UIObjecField()
{
	delete ui;
}

bool UIObjecField::CheckUpdate(const std::string &label, const std::string &objectName)
{
//    if (m_changed)
//    {
//        Debug::LogError("[UIObjecField] value changed");
//        value = m_value;
//        m_changed = false;
//        return true;
//    }

	if (m_label != label)
	{
		LogInfo("[UIObjecField] new label: " + m_label);
		m_label = label;
		LOG;
		ui->label->setText(m_label.c_str());
	}

	if (m_objectName != objectName)
	{
		m_objectName = objectName;
		LOG;
		ui->lineEdit->setText(objectName.c_str());
	}
	return false;
}

void UIObjecField::OnPickerClicked()
{
	SelectObjectDialog dialog;
	dialog.exec();
}
