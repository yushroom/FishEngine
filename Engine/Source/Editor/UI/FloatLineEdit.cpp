#include "FloatLineEdit.hpp"

#include <QDoubleValidator>
#include "../Helper.hpp"

FloatLineEdit::FloatLineEdit(QWidget *parent)
    : QLineEdit("0", parent)
{
    m_validator = new QDoubleValidator(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), 8);
    setValidator(m_validator);
    connect(this, &QLineEdit::editingFinished, this, &FloatLineEdit::OnEditingFinished);
}

void FloatLineEdit::setValue(float f)
{
    m_value = f;
    blockSignals(true);
    setText(QString::fromStdString(floatToStdString(m_value)));
    blockSignals(false);
}

void FloatLineEdit::setMinimum(float v)
{
    //m_validator->setBottom(v);
    m_min = v;
}

void FloatLineEdit::setMaximum(float v)
{
    //m_validator->setTop(v);
    m_max = v;
}

void FloatLineEdit::OnEditingFinished()
{
    bool ok = false;
    float f = text().toFloat(&ok);
    if (ok && m_value != f)
    {
        if (f > m_max)
            f = m_max;
        else if (f < m_min)
            f = m_min;
        m_value = f;
        emit valueChanged(m_value);
    }
    // re-format
    blockSignals(true);
    setText(QString::fromStdString(floatToStdString(m_value)));
    blockSignals(false);
}
