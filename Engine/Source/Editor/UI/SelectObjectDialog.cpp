#include "SelectObjectDialog.hpp"
#include "ui_SelectObjectDialog.h"

#include "ObjectListModel.hpp"

SelectObjectDialog::SelectObjectDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SelectObjectDialog)
{
	ui->setupUi(this);

	m_model = new ObjectListModel;
	ui->listView->setResizeMode(QListView::Adjust);
	ui->listView->setModel(m_model);
	ui->iconSizeSlider->setMinimum(0);
	ui->iconSizeSlider->setMaximum(32);
	ui->iconSizeSlider->setValue(m_listViewIconSize);
	OnIconSizeChanged(m_listViewIconSize);
	connect(ui->iconSizeSlider, &QSlider::valueChanged, this, &SelectObjectDialog::OnIconSizeChanged);
	connect(ui->listView->selectionModel(), &QItemSelectionModel::currentChanged, this, &SelectObjectDialog::OnListViewSelectionChanged);
}

SelectObjectDialog::~SelectObjectDialog()
{
	delete ui;
}

void SelectObjectDialog::ShowWithCallback(int classID, const Callback &callback)
{
	m_callback = callback;
	m_model->SetObjectType(classID);
	this->exec();
}

void SelectObjectDialog::OnIconSizeChanged(int size)
{
	m_listViewIconSize = size;
	if (size < 4)
	{
		ui->listView->setViewMode(QListView::ListMode);
		//ui->listView->setSpacing(0);
		ui->listView->setGridSize(QSize());
		ui->listView->setIconSize(QSize(12, 12));
	}
	else
	{
		ui->listView->setViewMode(QListView::IconMode);
		//ui->listView->setSpacing(-1);
		int s = size * 4;
		ui->listView->setGridSize(QSize(s+12, s+24));
		ui->listView->setIconSize(QSize(s, s));
	}
}

void SelectObjectDialog::OnListViewSelectionChanged(const QModelIndex &current, const QModelIndex &)
{
	m_callback(m_model->object(current));
}
