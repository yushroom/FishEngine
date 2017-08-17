#ifndef LOGVIEW_HPP
#define LOGVIEW_HPP

#include <QDialog>

namespace Ui {
class LogView;
}

class LogViewModel;
class LogViewFilterModel;

class LogView : public QDialog
{
	Q_OBJECT

public:
	explicit LogView(QWidget *parent = 0);
	~LogView();

private:
	Ui::LogView *ui;
	
	LogViewModel * m_model = nullptr;
	LogViewFilterModel * m_filterModel = nullptr;
};

#endif // LOGVIEW_HPP
