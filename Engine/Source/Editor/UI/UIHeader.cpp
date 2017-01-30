#include "UIHeader.hpp"
#include "ui_UIHeader.h"

#include "../UIDebug.hpp"

UIHeader::UIHeader(std::string const & componentTypeName, bool enabled, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIHeader),
    m_enabled(enabled),
    m_componentTypeName(componentTypeName)
{
    ui->setupUi(this);

    ui->label->setText(QString(componentTypeName.c_str()));
    ui->checkBox->setChecked(m_enabled);

    connect(ui->checkBox, SIGNAL(toggled(bool)), this, SLOT(OnCheckBoxChanged(bool)));
}

UIHeader::UIHeader(std::string const & componentTypeName, QWidget *parent) :
    UIHeader(componentTypeName, true, parent)
{
    ui->checkBox->setHidden(true);
}

UIHeader::~UIHeader()
{
    delete ui;
}

bool UIHeader::CheckUpdate(const std::string &componentTypeName, bool &enabled)
{
    if (m_changed)
    {
        Debug::LogError("[Header] enabled changed");
        enabled = m_enabled;
        m_changed = false;
        return true;
    }

    if (ui->checkBox->isHidden())
    {
        LOG;
        ui->checkBox->setHidden(false);
    }

    if (m_enabled != enabled)
    {
        Debug::LogError("[Header] new enabled");
        m_enabled = enabled;
        LOG;
        ui->checkBox->setChecked(m_enabled);
    }

    if (componentTypeName != m_componentTypeName)
    {
        Debug::LogError("[Header] new component name: %s", componentTypeName.c_str());
        m_componentTypeName = componentTypeName;
        LOG;
        ui->label->setText(QString(componentTypeName.c_str()));
    }
    return false;
}

void UIHeader::CheckUpdate(const std::string &componentTypeName)
{
    if (!ui->checkBox->isHidden())
    {
        LOG;
        ui->checkBox->setHidden(true);
    }

    if (componentTypeName != m_componentTypeName)
    {
        Debug::LogError("[Header] new component name: %s", componentTypeName.c_str());
        m_componentTypeName = componentTypeName;
        LOG;
        ui->label->setText(QString(componentTypeName.c_str()));
    }
}

void UIHeader::OnCheckBoxChanged(bool value)
{
    m_enabled = value;
    m_changed = true;
}
