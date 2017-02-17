#ifndef OPENPROJECTDIALOG_HPP
#define OPENPROJECTDIALOG_HPP

#include <QDialog>

namespace Ui {
class OpenProjectDialog;
}

class OpenProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenProjectDialog(QWidget *parent = 0);
    ~OpenProjectDialog();

private:

	void OnCreateButtonClicked();
	void OnOpenButtonClicked();
	void OnLocationToolButtonClicked();

	void SetProjectPath(QString const & projectPath);

	void CreateNewProject(QString const & projectPath);

    Ui::OpenProjectDialog *ui;
};

#endif // OPENPROJECTDIALOG_HPP
