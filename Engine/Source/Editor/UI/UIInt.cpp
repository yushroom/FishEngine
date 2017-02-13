#include "UIInt.hpp"
#include "ui_UIInt.h"

#include "UIDebug.hpp"

UIInt::UIInt(QString const& label, int value, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIInt)
{
    ui->setupUi(this);
    ui->horizontalLayout->setContentsMargins(0, 2, 2, 2);
    ui->label->setText(label);
    ui->spinBox->setValue(value);
}

UIInt::~UIInt()
{
    delete ui;
}
