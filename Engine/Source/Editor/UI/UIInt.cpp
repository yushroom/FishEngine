#include "UIInt.hpp"
#include "ui_UIInt.h"

#include "../UIDebug.hpp"

UIInt::UIInt(QString const& label, int value, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIInt)
{
    ui->setupUi(this);
    ui->label->setText(label);
    ui->spinBox->setValue(value);
}

UIInt::~UIInt()
{
    delete ui;
}
