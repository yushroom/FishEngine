#include "UIBool.hpp"
#include "ui_UIBool.h"

#include "../UIDebug.hpp"

UIBool::UIBool(const std::string &label, bool value, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIBool),
    m_label(label),
    m_value(value)
{
    ui->setupUi(this);
    ui->label->setText(label.c_str());
    ui->checkBox->setChecked(value);

    connect(ui->checkBox, SIGNAL(toggled(bool)), this, SLOT(OnCheckBoxChanged(bool)));
}

UIBool::~UIBool()
{
    delete ui;
}

bool UIBool::CheckUpdate(const std::string &label, bool &value)
{
    if (m_changed)
    {
        Debug::LogError("[UIBool] value changed");
        value = m_value;
        m_changed = false;
        return true;
    }

    if (m_label != label)
    {
        Debug::LogError("[UIBool] new label: %s", m_label.c_str());
        m_label = label;
        LOG;
        ui->label->setText(m_label.c_str());
    }

    if (m_value != value)
    {
        m_value = value;
        Debug::LogError("[UIBool] new value");
        LOG;
        ui->checkBox->blockSignals(true);
        ui->checkBox->setChecked(value);
        ui->checkBox->blockSignals(false);
    }
}
