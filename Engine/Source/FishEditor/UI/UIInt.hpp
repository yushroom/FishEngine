#ifndef UIINT_HPP
#define UIINT_HPP

#include <QWidget>

namespace Ui {
class UIInt;
}

class UIInt : public QWidget
{
	Q_OBJECT

public:
	explicit UIInt(QString const& label, int value, QWidget *parent = 0);
	~UIInt();

private:
	Ui::UIInt *ui;
};

#endif // UIINT_HPP
