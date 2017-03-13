#ifndef UICOMBOBOX_HPP
#define UICOMBOBOX_HPP

#include <QWidget>

namespace Ui {
class UIComboBox;
}

class UIComboBox : public QWidget
{
	Q_OBJECT

public:
	explicit UIComboBox(std::string const & label, int index, const char* const* enumStringArray, int arraySize, QWidget *parent = 0);
	~UIComboBox();

	// IMGUI
	bool CheckUpdate(std::string const & label, int &index, const char* const* enumStringArray, int arraySize);
	
	// this method do not emit signal
	void SetValue(int index);

Q_SIGNALS:
	// RMGUI
	void OnValueChanged(int value);
	
private slots:
	void OnComboBoxChanged(int index);

private:
	Ui::UIComboBox *ui;

	bool        m_changed = false;

	std::string m_label;
	const char * const*     m_enumStringArray;
	int         m_arraySize;
	int         m_index;
};

#endif // UICOMBOBOX_HPP
