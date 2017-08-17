#include "ClickableLabel.hpp"

//ClickableLabel::ClickableLabel(const QString& text, QWidget* parent)
//    : QLabel(parent)
//{
//    setText(text);
//}

ClickableLabel::ClickableLabel(QWidget *parent)
	: QLabel(parent)
{

}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent*)
{
	emit clicked();
}
