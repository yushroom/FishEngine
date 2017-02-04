#ifndef SELECTOBJECTDIALOG_HPP
#define SELECTOBJECTDIALOG_HPP

#include <QDialog>

namespace Ui {
class SelectObjectDialog;
}

class SelectObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectObjectDialog(QWidget *parent = 0);
    ~SelectObjectDialog();

private:
    Ui::SelectObjectDialog *ui;
};

#endif // SELECTOBJECTDIALOG_HPP
