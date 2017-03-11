#include "UIRevertApplyButtons.hpp"
#include "ui_UIRevertApplyButtons.h"

UIRevertApplyButtons::UIRevertApplyButtons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIRevertApplyButtons)
{
    ui->setupUi(this);
	
	connect(ui->revertButton, &QPushButton::clicked, this, &UIRevertApplyButtons::OnRevert);
	
	connect(ui->applyButton, &QPushButton::clicked, this, &UIRevertApplyButtons::OnApply);
	
	SetEnabled(false);
}

UIRevertApplyButtons::~UIRevertApplyButtons()
{
    delete ui;
}

void UIRevertApplyButtons::SetEnabled(bool enabled)
{
	if (enabled ^ this->isEnabled())
		this->setEnabled(enabled);
}
