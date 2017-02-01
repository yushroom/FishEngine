#include "UIButton.hpp"

UIButton::UIButton(QWidget *parent)
    : UIButton("", parent)
{

}

UIButton::UIButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(OnClicked()));
}

bool UIButton::CheckClicked()
{
    if (m_clicked)
    {
        m_clicked = false;
        return true;
    }
    return false;
}

void UIButton::OnClicked()
{
    m_clicked = true;
}
