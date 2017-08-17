#ifndef ASSETPROGRESSDIALOG_HPP
#define ASSETPROGRESSDIALOG_HPP

#include <QDialog>

namespace Ui {
class AssetProgressDialog;
}

class AssetProgressDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AssetProgressDialog(QWidget *parent = 0);
	~AssetProgressDialog();

private:
	Ui::AssetProgressDialog *ui;
};

#endif // ASSETPROGRESSDIALOG_HPP
