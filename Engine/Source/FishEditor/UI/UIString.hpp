#ifndef UISTRING_HPP
#define UISTRING_HPP

#include <QWidget>

namespace Ui {
class UIString;
}

class UIString : public QWidget
{
	Q_OBJECT

public:
	explicit UIString(QString const & label, QString const &value, QWidget *parent = 0);
	~UIString();

private:
	Ui::UIString *ui;
};

#endif // UISTRING_HPP
