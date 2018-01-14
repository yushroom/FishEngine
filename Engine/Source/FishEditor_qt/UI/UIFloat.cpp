#include "UIFloat.hpp"
#include "ui_UIFloat.h"

#include "UIDebug.hpp"
#include "../Helper.hpp"

UIFloat::UIFloat(const std::string &label, float value, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UIFloat),
	m_label(label),
	m_value(value)
{
	ui->setupUi(this);
	ui->horizontalLayout->setContentsMargins(0, 2, 2, 2);

	ui->label->setText(m_label.c_str());

	ui->value->setValue(m_value);
	connect(ui->value, &FloatLineEdit::valueChanged, this, &UIFloat::OnValueChanged);
}

UIFloat::~UIFloat()
{
	delete ui;
}

bool UIFloat::CheckUpdate(const std::string &label, float &value)
{
	if (m_changed)
	{
		LogInfo("[float] value changed");
		value = m_value;
		m_changed = false;
		return true;
	}

	if (m_label != label)
	{
		LogInfo("[float] new label: " + m_label);
		m_label = label;
		LOG;
		ui->label->setText(m_label.c_str());
	}

	if (m_value != value)
	{
		if (!ui->value->hasFocus())
		{
			LogInfo("[float] new value");
			LOG;
			m_value = value;
			//ui->value->blockSignals(true);
			ui->value->setValue(m_value);
			//ui->value->blockSignals(false);
		}
	}

	return false;
}

void UIFloat::SetValue(float value)
{
	if (m_value != value)
	{
		//if (!ui->value->hasFocus())
		//{
			LogInfo("[float] new value");
			LOG;
			m_value = value;
			//ui->value->blockSignals(true);
			ui->value->setValue(m_value);
			//ui->value->blockSignals(false);
		//}
	}
}

void UIFloat::OnValueChanged(float f)
{
	m_value = f;
	m_changed = true;
	emit ValueChanged(f);
}
