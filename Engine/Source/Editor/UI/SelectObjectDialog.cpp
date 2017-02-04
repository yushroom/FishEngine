#include "SelectObjectDialog.hpp"
#include "ui_SelectObjectDialog.h"

SelectObjectDialog::SelectObjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectObjectDialog)
{
    ui->setupUi(this);
}

SelectObjectDialog::~SelectObjectDialog()
{
    delete ui;
}
