#include "UIFloat4.hpp"
#include "ui_UIFloat4.h"

#include <limits>

#include "UIDebug.hpp"

UIFloat4::UIFloat4(std::string const & label, float x, float y, float z, float w, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIFloat4),
    m_label(label)
{
    ui->setupUi(this);
    ui->horizontalLayout->setContentsMargins(0, 2, 2, 2);

    m_x = x;
    m_y = y;
    m_z = z;
    m_w = w;

    ui->label->setText(label.c_str());
    ui->x->setMinimum(std::numeric_limits<float>::lowest());
    ui->x->setMaximum(std::numeric_limits<float>::max());
    ui->y->setMinimum(std::numeric_limits<float>::lowest());
    ui->y->setMaximum(std::numeric_limits<float>::max());
    ui->z->setMinimum(std::numeric_limits<float>::lowest());
    ui->z->setMaximum(std::numeric_limits<float>::max());
    ui->w->setMinimum(std::numeric_limits<float>::lowest());
    ui->w->setMaximum(std::numeric_limits<float>::max());
    ui->x->setValue(x);
    ui->y->setValue(y);
    ui->z->setValue(z);
    ui->w->setValue(w);

    connect(ui->x, &FloatLineEdit::valueChanged, this, &UIFloat4::setX);
    connect(ui->y, &FloatLineEdit::valueChanged, this, &UIFloat4::setY);
    connect(ui->z, &FloatLineEdit::valueChanged, this, &UIFloat4::setZ);
    connect(ui->w, &FloatLineEdit::valueChanged, this, &UIFloat4::setW);
}

UIFloat4::~UIFloat4()
{
    delete ui;
}


bool UIFloat4::CheckUpdate(std::string const & label, float &x, float &y, float &z, float &w)
{
    if (m_changed)
    {
        Debug::Log("[UIFloat4] value changed");
        x = m_x;
        y = m_y;
        z = m_z;
        w = m_w;
        m_changed = false;
        return true;
    }

    if (m_label != label)
    {
        Debug::Log("[UIFloat4] new label: %s", label.c_str());
        m_label = label;
        LOG;
        ui->label->setText(m_label.c_str());
    }

    if (m_x != x)
    {
        if (!ui->x->hasFocus())
        {
            Debug::Log("[UIFloat4] new x");
            LOG;
            ui->x->blockSignals(true);
            ui->x->setValue(x);
            ui->x->blockSignals(false);
            m_x = x;
        }
    }
    if (m_y != y)
    {
        if (!ui->y->hasFocus())
        {
            Debug::Log("[UIFloat4] new y");
            LOG;
            ui->y->blockSignals(true);
            ui->y->setValue(y);
            ui->y->blockSignals(false);
            m_y = y;
        }
    }
    if (m_z != z)
    {
        if (!ui->z->hasFocus())
        {
            Debug::Log("[UIFloat4] new z");
            LOG;
            ui->z->blockSignals(true);
            ui->z->setValue(z);
            ui->z->blockSignals(false);
            m_z = z;
        }
    }
    if (m_w != w)
    {
        if (!ui->w->hasFocus())
        {
            Debug::Log("[UIFloat4] new w");
            LOG;
            ui->w->blockSignals(true);
            ui->w->setValue(w);
            ui->w->blockSignals(false);
            m_w = w;
        }
    }
    return false;
}
