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

private slots:
    void OnEditingFinished();

private:
    float m_value = 0;
    float m_min = std::numeric_limits<float>::lowest();
    float m_max = std::numeric_limits<float>::max();
    QDoubleValidator * m_validator;
};
