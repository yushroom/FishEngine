#ifndef UIFLOAT_HPP
#define UIFLOAT_HPP

#include <QWidget>

namespace Ui {
class UIFloat;
}

class UIFloat : public QWidget
{
    Q_OBJECT

public:
    explicit UIFloat(std::string const & label, float value, QWidget *parent = 0);
    ~UIFloat();

    // return true if the value is modified by Inspector
    bool CheckUpdate(std::string const & label, float & value);

private Q_SLOTS:
    inline void setValue(double v) { m_value = static_cast<float>(v); m_changed = true; }

private:
    Ui::UIFloat *ui;

    bool        m_changed = false;
    std::string m_label;
    float       m_value;
};

#endif // UIFLOAT_HPP
