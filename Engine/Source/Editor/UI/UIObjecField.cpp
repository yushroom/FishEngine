#include "UIObjecField.hpp"
#include "ui_UIObjecField.h"

#include <Object.hpp>

UIObjecField::UIObjecField(std::string const & label, std::string const & objectName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIObjecField),
    m_label(label),
    m_objectName(objectName)
{
    ui->setupUi(this);

    ui->label->setText(m_label.c_str());
    ui->lineEdit->setText(objectName.c_str());
}

UIObjecField::~UIObjecField()
{
    delete ui;
}
