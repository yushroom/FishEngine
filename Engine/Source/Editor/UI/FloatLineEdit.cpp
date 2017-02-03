#include "FloatLineEdit.hpp"

#include <QDoubleValidator>
#include "../Helper.hpp"
#include "../UIDebug.hpp"

FloatLineEdit::FloatLineEdit(QWidget *parent)
    : QLineEdit("0", parent), m_value(0)
{
    m_validator = new QDoubleValidator(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), 8);
    setValidator(m_validator);
    connect(this, &QLineEdit::editingFinished, this, &FloatLineEdit::OnEditingFinished);
    connect(this, &QLineEdit::textChanged, this, &FloatLineEdit::OnTextChanged);
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
    if (text().size() == 0)
    {
        m_value = 0;
        blockSignals(true);
        setText("0");
        blockSignals(false);
    }
}

void FloatLineEdit::OnTextChanged(QString const & s)
{
    if (s.size() == 0)
    {
        m_value = 0;
        // set "0" in OnEditingFinished
        return;
    }
    bool ok = false;
    float f = s.toFloat(&ok);
    if (ok && m_value != f)
    {
        if (f > m_max)
            f = m_max;
        else if (f < m_min)
            f = m_min;
        //Debug::LogError("%f %f", m_value, f);
        m_value = f;
        emit valueChanged(m_value);
    }
    // re-format
    blockSignals(true);
    setText(QString::fromStdString(floatToStdString(m_value)));
    blockSignals(false);
}

void FloatLineEdit::mousePressEvent(QMouseEvent *)
{
    selectAll();
}
