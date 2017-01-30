#include "UIFloat.hpp"
#include "ui_UIFloat.h"

#include "../UIDebug.hpp"

UIFloat::UIFloat(const std::string &label, float value, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIFloat),
    m_label(label),
    m_value(value)
{
    ui->setupUi(this);

    ui->label->setText(m_label.c_str());

    ui->doubleSpinBox->setMinimum(std::numeric_limits<float>::lowest());
    ui->doubleSpinBox->setMaximum(std::numeric_limits<float>::max());
    ui->doubleSpinBox->setValue(m_value);

    connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)));
}

UIFloat::~UIFloat()
{
    delete ui;
}

bool UIFloat::CheckUpdate(const std::string &label, float &value)
{
    if (m_changed)
    {
        Debug::LogError("[float] value changed");
        value = m_value;
        m_changed = false;
        return true;
    }

    if (m_label != label)
    {
        Debug::LogError("[float] new label: %s", m_label.c_str());
        m_label = label;
        LOG;
        ui->label->setText(m_label.c_str());
    }

    if (m_value != value)
    {
        if (!ui->doubleSpinBox->hasFocus())
        {
            Debug::LogError("[float] new value");
            LOG;
            ui->doubleSpinBox->blockSignals(true);
            ui->doubleSpinBox->setValue(value);
            ui->doubleSpinBox->blockSignals(false);
            m_value = value;
        }
    }

    return false;
}
