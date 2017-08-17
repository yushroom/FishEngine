#include "UISlider.hpp"
#include "ui_UISlider.h"

#include "UIDebug.hpp"

UISlider::UISlider(const std::string &label, float value, float leftValue, float rightValue, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UISlider),
	m_label(label),
	m_value(value),
	m_leftValue(leftValue),
	m_rightValue(rightValue)
{
	ui->setupUi(this);
	ui->horizontalLayout->setContentsMargins(0, 2, 2, 2);

	ui->label->setText(m_label.c_str());
	ui->slider->setMinimum(0);
	ui->slider->setMaximum(100);
	int int_value = static_cast<int>((value - leftValue) / (rightValue - leftValue) * 100);
	ui->slider->setValue(int_value);

	ui->spinBox->setMinimum(leftValue);
	ui->spinBox->setMaximum(rightValue);
	ui->spinBox->setValue(value);

	connect(ui->spinBox, &FloatLineEdit::valueChanged, this, &UISlider::OnSpinBoxValueChanged);
	connect(ui->slider, SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
}

UISlider::~UISlider()
{
	delete ui;
}


bool UISlider::CheckUpdate(const std::string &label, float &value)
{
	if (m_changed)
	{
		LogInfo("[UISlider] value changed");
		value = m_value;
		m_changed = false;
		return true;
	}

	if (m_label != label)
	{
		m_label = label;
		LOG;
		ui->label->setText(m_label.c_str());
	}

	if (m_value != value)
	{
		if (!ui->spinBox->hasFocus())
		{
			LOG;
			ui->spinBox->blockSignals(true);
			ui->spinBox->setValue(value);
			ui->spinBox->blockSignals(false);
			ui->slider->blockSignals(true);
			ui->slider->setValue(value);
			ui->spinBox->blockSignals(false);
			m_value = value;
		}
	}

	return false;
}

void UISlider::OnSpinBoxValueChanged(float v)
{
	m_value = v;
	ui->slider->blockSignals(true);
	int int_value = static_cast<int>((m_value - m_leftValue) / (m_rightValue - m_leftValue) * 100);
	ui->slider->setValue(int_value);
	ui->slider->blockSignals(false);
	m_changed = true;
}

void UISlider::OnSliderValueChanged(int v)
{
	m_value = static_cast<float>(v) / 100.0f * (m_rightValue - m_leftValue) + m_leftValue;
	ui->spinBox->blockSignals(true);
	ui->spinBox->setValue(m_value);
	ui->spinBox->blockSignals(false);
	m_changed = true;
}
