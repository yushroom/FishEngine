#include "UIMaterialHeader.hpp"
#include "ui_UIMaterialHeader.h"

UIMaterialHeader::UIMaterialHeader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIMaterialHeader)
{
    ui->setupUi(this);
}

UIMaterialHeader::~UIMaterialHeader()
{
    delete ui;
}
