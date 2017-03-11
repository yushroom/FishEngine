#ifndef UIBOOL_HPP
#define UIBOOL_HPP

#include <QWidget>

namespace Ui {
class UIBool;
}

class UIBool : public QWidget
{
    Q_OBJECT

public:
    explicit UIBool(std::string const & label, bool value, QWidget *parent = 0);
    ~UIBool();

	// IMGUI
    bool CheckUpdate(std::string const & label, bool& value);
	
	void SetValue(bool value);
	
Q_SIGNALS:
	// RMGUI
	void OnValueChanged(bool value);

private slots:
    void OnCheckBoxChanged(bool value)
    {
        m_changed = true;
        m_value = value;
		emit OnValueChanged(value);
    }

private:

    Ui::UIBool *ui;

    bool        m_changed = false;
    std::string m_label;

    bool        m_value;
};

#endif // UIBOOL_HPP
