#pragma once

#include <QLineEdit>

class QDoubleValidator;

class FloatLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit FloatLineEdit(QWidget* parent = nullptr);

    void setValue(float);
    void setMinimum(float);
    void setMaximum(float);

signals:
    void valueChanged(float);

private:

    void OnEditingFinished();
    // OnEditingFinished is more efficient,
    // but OnTextChanged is safer
    void OnTextChanged(const QString &);

private:

    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;

    float m_value = 0;
    float m_min = std::numeric_limits<float>::lowest();
    float m_max = std::numeric_limits<float>::max();
    QDoubleValidator * m_validator;
};
