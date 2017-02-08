#include "UIColor.hpp"
#include "ui_UIColor.h"

#include <QColorDialog>

#include "../UIDebug.hpp"

UIColor::UIColor(std::string const & label, QColor const & value, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIColor),
    m_label(label),
    m_value(value)
{
    ui->setupUi(this);
    ui->horizontalLayout->setContentsMargins(0, 2, 2, 2);

    ui->label->setText(label.c_str());

    // https://wiki.qt.io/How_to_Change_the_Background_Color_of_QWidget
    ui->toolButton->setStyleSheet("background-color: " + m_value.name());

    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(ShowColorDialog()));
}

UIColor::~UIColor()
{
    delete ui;
}

bool UIColor::CheckUpdate(const std::string &label, QColor &value)
{
    if (m_changed)
    {
        Debug::Log("[UIColor] value changed");
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
        //if (!ui->doubleSpinBox->hasFocus())
        //{
            LOG;
            //ui->doubleSpinBox->blockSignals(true);
            //ui->doubleSpinBox->setValue(value);
            //ui->doubleSpinBox->blockSignals(false);
            m_value = value;
            ui->toolButton->setStyleSheet("background-color: " + m_value.name());
        //}
    }

    return false;
}

void UIColor::ShowColorDialog()
{
    auto const & color = QColorDialog::getColor(m_value, this, "Color", QColorDialog::ShowAlphaChannel);
//    ui->label_2->setPalette(QPalette(m_value));
//    ui->label_2->setAutoFillBackground(true);
    if (color.isValid())
    {
        m_value = color;
        ui->toolButton->setStyleSheet("background-color: " + m_value.name());
    }

    m_changed = true;
}
