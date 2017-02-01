#ifndef UISLIDER_HPP
#define UISLIDER_HPP

#include <QWidget>

namespace Ui {
class UISlider;
}

class UISlider : public QWidget
{
    Q_OBJECT

public:
    explicit UISlider(std::string const & label, float value, float leftValue, float rightValue, QWidget *parent = 0);
    ~UISlider();

    // return true if the value is modified by Inspector
    bool CheckUpdate(std::string const & label, float & value);

private Q_SLOTS:
    void OnSpinBoxValueChanged(float v);

    void OnSliderValueChanged(int v);

private:
    Ui::UISlider *ui;

    bool        m_changed = false;
    std::string m_label;
    float       m_value;
    float       m_leftValue;
    float       m_rightValue;
};

#endif // UISLIDER_HPP
