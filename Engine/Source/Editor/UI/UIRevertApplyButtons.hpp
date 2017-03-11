#ifndef UIREVERTAPPLYBUTTONS_H
#define UIREVERTAPPLYBUTTONS_H

#include <QWidget>

namespace Ui {
class UIRevertApplyButtons;
}

class UIRevertApplyButtons : public QWidget
{
    Q_OBJECT

public:
    explicit UIRevertApplyButtons(QWidget *parent = 0);
    ~UIRevertApplyButtons();
	
Q_SIGNALS:
	void OnRevert();
	void OnApply();
	
public:
	void SetEnabled(bool enabled);

private:
    Ui::UIRevertApplyButtons *ui;
};

#endif // UIREVERTAPPLYBUTTONS_H
