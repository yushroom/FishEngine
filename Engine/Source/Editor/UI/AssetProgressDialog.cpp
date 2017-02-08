#include "AssetProgressDialog.hpp"
#include "ui_AssetProgressDialog.h"

AssetProgressDialog::AssetProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AssetProgressDialog)
{
    ui->setupUi(this);
}

AssetProgressDialog::~AssetProgressDialog()
{
    delete ui;
}
