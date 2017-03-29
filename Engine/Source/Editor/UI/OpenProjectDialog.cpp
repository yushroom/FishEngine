#include "OpenProjectDialog.hpp"
#include "ui_OpenProjectDialog.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>

#include <Application.hpp>

OpenProjectDialog::OpenProjectDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::OpenProjectDialog)
{
	ui->setupUi(this);
#if FISHENGINE_PLATFORM_WINDOWS
	ui->locationLineEdit->setText(R"(D:\program\github\FishEngine\Example)");
#else
	ui->locationLineEdit->setText("/Users/yushroom/program/FishEngine/Example");
#endif
	ui->projectNameLineEdit->setText("Sponza");
	ui->projectNameErrorLabel->hide();
	ui->locationErrorLabel->hide();

	connect(ui->createButton, &QPushButton::clicked, this, &OpenProjectDialog::OnCreateButtonClicked);
	connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui->projectNameLineEdit, &QLineEdit::textChanged, ui->projectNameErrorLabel, &QLabel::hide);
	connect(ui->toolButton, &QToolButton::clicked, this, &OpenProjectDialog::OnLocationToolButtonClicked);
	connect(ui->locationLineEdit, &QLineEdit::textChanged, ui->locationErrorLabel, &QLabel::hide);
	connect(ui->openButton, &QPushButton::clicked, this, &OpenProjectDialog::OnOpenButtonClicked);
	
	//OnOpenButtonClicked();
}

OpenProjectDialog::~OpenProjectDialog()
{
	delete ui;
}

void OpenProjectDialog::OnCreateButtonClicked()
{
#if 1
#if FISHENGINE_PLATFORM_WINDOWS
	FishEngine::Application::s_dataPath = R"(D:\program\github\FishEngine\Example\Sponza\Assets)";
#else
	FishEngine::Application::s_dataPath = "/Users/yushroom/program/FishEngine/Example/Sponza/Assets";
#endif
	accept();
#endif

	auto projectName = ui->projectNameLineEdit->text();
	auto location = ui->locationLineEdit->text();
	
	QDir dir = location;
	if ( dir.exists())
	{
		auto fullpath = dir.filePath(projectName);
		if (dir.exists(projectName))
		{
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
		ui->locationErrorLabel->show();
	}
}

void OpenProjectDialog::OnOpenButtonClicked()
{
	auto dir = QFileDialog::getExistingDirectory(this, "Open existing project", ui->locationLineEdit->text());
	if ( ! QDir(dir).exists("Assets"))
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
	ui->locationLineEdit->setText(dir);
}

void OpenProjectDialog::SetProjectPath(QString const & projectPath)
{
	FishEngine::Application::s_dataPath = QDir(projectPath).filePath("Assets").toStdString();
}

void OpenProjectDialog::CreateNewProject(QString const & projectPath)
{
	QDir(projectPath).mkdir("Assets");
}
