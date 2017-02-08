#include "UIString.hpp"
#include "ui_UIString.h"

#include "../UIDebug.hpp"

UIString::UIString(const QString &label, const QString &value, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIString)
{
    ui->setupUi(this);
    ui->horizontalLayout->setContentsMargins(0, 2, 2, 2);
    ui->label->setText(label);
    ui->lineEdit->setText(value);
}

UIString::~UIString()
{
    delete ui;
}
