#ifndef UICOLOR_HPP
#define UICOLOR_HPP

#include <QWidget>

namespace Ui {
class UIColor;
}

class UIColor : public QWidget
{
	Q_OBJECT

public:
	explicit UIColor(std::string const & label, QColor const & value, QWidget *parent = 0);
	~UIColor();

	bool CheckUpdate(std::string const & label, QColor & value);

private slots:
	void ShowColorDialog();

private:
	Ui::UIColor *ui;

	bool        m_changed = false;
	std::string m_label;
	QColor      m_value;
};

#endif // UICOLOR_HPP
