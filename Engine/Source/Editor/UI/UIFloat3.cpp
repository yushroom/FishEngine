#include "UIFloat3.hpp"
#include "ui_UIFloat3.h"

#include <limits>

#include "../UIDebug.hpp"

UIFloat3::UIFloat3(const std::string &label, float x, float y, float z, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIFloat3),
    m_label(label)
{
    ui->setupUi(this);

    m_x = x;
    m_y = y;
    m_z = z;

    ui->label->setText(label.c_str());
    ui->x->setMinimum(std::numeric_limits<float>::lowest());
    ui->x->setMaximum(std::numeric_limits<float>::max());
    ui->y->setMinimum(std::numeric_limits<float>::lowest());
    ui->y->setMaximum(std::numeric_limits<float>::max());
    ui->z->setMinimum(std::numeric_limits<float>::lowest());
    ui->z->setMaximum(std::numeric_limits<float>::max());
    ui->x->setValue(x);
    ui->y->setValue(y);
    ui->z->setValue(z);

    // http://stackoverflow.com/questions/13422995/set-qlineedit-to-accept-only-numbers
    // http://blog.csdn.net/a_sungirl/article/details/17373405
    //ui->x->setValidator(new QDoubleValidator(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), 10));
    //ui->y->setValidator(new QDoubleValidator);
    //ui->z->setValidator(new QDoubleValidator);

//    QString s;
//    s.number(s, 'f', 8);

    connect(ui->x, &FloatLineEdit::valueChanged, this, &UIFloat3::setX);
    connect(ui->y, &FloatLineEdit::valueChanged, this, &UIFloat3::setY);
    connect(ui->z, &FloatLineEdit::valueChanged, this, &UIFloat3::setZ);
    //connect(ui->y, SIGNAL(valueChanged(double)), this, SLOT(setY(double)));
    //connect(ui->z, SIGNAL(valueChanged(double)), this, SLOT(setZ(double)));
}

UIFloat3::~UIFloat3()
{
    delete ui;
}

bool UIFloat3::CheckUpdate(std::string const & label, float &x, float &y, float &z)
{
    if (m_changed)
    {
        Debug::Log("[float3] value changed");
        x = m_x;
        y = m_y;
        z = m_z;
        m_changed = false;
        return true;
    }

    if (m_label != label)
    {
        Debug::Log("[float3] new label: %s", label.c_str());
        m_label = label;
        LOG;
        ui->label->setText(m_label.c_str());
    }

    if (m_x != x)
    {
        if (!ui->x->hasFocus())
        {
            Debug::Log("[float3] new x");
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
            Debug::Log("[float3] new y");
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
            Debug::Log("[float3] new z");
            LOG;
            ui->z->blockSignals(true);
            ui->z->setValue(z);
            ui->z->blockSignals(false);
            m_z = z;
        }
    }
    return false;
}
