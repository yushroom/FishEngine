#pragma once

#include <QPushButton>

class UIButton : public QPushButton
{
	Q_OBJECT
public:
	explicit UIButton(QWidget* parent = nullptr);
	explicit UIButton(const QString &text, QWidget* parent = nullptr);

	bool CheckClicked();

private slots:
	void OnClicked();

private:
	bool m_clicked = false;
};
