#include "OpenProjectDialog.hpp"
#include "ui_OpenProjectDialog.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>

#include <FishEngine/Application.hpp>

OpenProjectDialog::OpenProjectDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::OpenProjectDialog)
{
	ui->setupUi(this);

#if FISHENGINE_PLATFORM_WINDOWS
	QString projectRootDir = R"(D:\program\github\FishEngine\Example)";
#else
	QString projectRootDir = "/Users/yushroom/program/FishEngine/Example";
#endif
	ui->locationLineEdit->setText(projectRootDir);

	QDir dir(projectRootDir);
	dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	auto entryList = dir.entryList();
	for (auto & entry : entryList)
	{
		ui->listWidget->addItem(entry);
	}
	ui->listWidget->setAlternatingRowColors(true);

	ui->projectNameLineEdit->setText("New Project");
	ClearErrorMessage();

	connect(ui->createButton, &QPushButton::clicked, this, &OpenProjectDialog::OnCreateButtonClicked);
	connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui->projectNameLineEdit, &QLineEdit::textChanged, ui->projectNameErrorLabel, &QLabel::hide);
	connect(ui->toolButton, &QToolButton::clicked, this, &OpenProjectDialog::OnLocationToolButtonClicked);
	connect(ui->locationLineEdit, &QLineEdit::textChanged, ui->locationErrorLabel, &QLabel::hide);
	connect(ui->openButton, &QPushButton::clicked, this, &OpenProjectDialog::OnOpenButtonClicked);
	connect(ui->listWidget, &QListWidget::itemClicked, this, &OpenProjectDialog::OnProjectListWidgetItemClicked);
}

OpenProjectDialog::~OpenProjectDialog()
{
	delete ui;
}

void OpenProjectDialog::OnCreateButtonClicked()
{
	auto projectName = ui->projectNameLineEdit->text();
	auto location = ui->locationLineEdit->text();
	
	QDir dir = location;
	if ( dir.exists())
	{
		auto fullpath = dir.filePath(projectName);
		if (dir.exists(projectName))
		{
			ClearErrorMessage();
			ui->projectNameErrorLabel->show();
		}
		else
		{
			if (dir.mkdir(projectName))
			{
				CreateNewProject(fullpath);
				SetProjectPath(fullpath);
				accept();
			}
		}
	}
	else
	{
		ClearErrorMessage();
		ui->locationErrorLabel->show();
	}
}

void OpenProjectDialog::OnOpenButtonClicked()
{
	auto dir = QFileDialog::getExistingDirectory(this, "Open existing project", ui->locationLineEdit->text());
	if (dir.isNull())
	{

	}
	else if ( ! QDir(dir).exists("Assets"))
	{
		QMessageBox::warning(this, "Waring", "Selected folder is not a FishEngine project.");
	}
	else
	{
		SetProjectPath(dir);
		accept();
	}
}

void OpenProjectDialog::OnLocationToolButtonClicked()
{
	auto dir = QFileDialog::getExistingDirectory(this, "Choose location for project", ui->locationLineEdit->text());
	if (!dir.isNull())
	{
		ui->locationLineEdit->setText(dir);
	}
}

void OpenProjectDialog::OnProjectListWidgetItemClicked(QListWidgetItem * item)
{
	auto location = ui->locationLineEdit->text();
	QDir dir = location;
	dir = dir.filePath(item->text());
	if (!QDir(dir).exists("Assets"))
	{
		QMessageBox::warning(this, "Waring", "Project not found.");
	}
	else
	{
		SetProjectPath(dir.absolutePath());
		accept();
	}
}

void OpenProjectDialog::SetProjectPath(QString const & projectPath)
{
	FishEngine::Application::s_dataPath = QDir(projectPath).filePath("Assets").toStdString();
}

void OpenProjectDialog::CreateNewProject(QString const & projectPath)
{
	QDir(projectPath).mkdir("Assets");
}

void OpenProjectDialog::ClearErrorMessage()
{
	ui->projectNameErrorLabel->hide();
	ui->locationErrorLabel->hide();
}
