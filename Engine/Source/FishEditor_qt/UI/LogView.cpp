#include "LogView.hpp"
#include "ui_LogView.h"

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

#include <FishEngine/Internal/SimpleLogger.hpp>

using namespace FishEngine;

class LogViewModel : public QAbstractListModel
{
	//Q_OBJECT

public:
	explicit LogViewModel(QObject *parent = nullptr)
		: QAbstractListModel(parent)
	{
	}

	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const override
	{
		auto & d = SimpleLogger::GetInstance().m_data;
		return static_cast<int>(d.size());
	}

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
	{
		if ( !index.isValid() || index.model() != this)
		{
			return QVariant();
		}
		int row = index.row();
		auto & data = SimpleLogger::GetInstance().m_data;
		if (row >= data.size() || row < 0)
		{
			return QVariant();
		}

		auto & d = data[row];
		static QIcon info_icon(":/Resources/console_info.png");
		static QIcon warn_icon(":/Resources/console_warn.png");
		static QIcon error_icon(":/Resources/console_error.png");

		switch (role)
		{
		case Qt::DisplayRole:
			return QString::fromStdString(d.message);
		case Qt::DecorationRole:
			switch (d.type)
			{
			case LogType::Log:
				return info_icon;
			case LogType::Warning:
				return warn_icon;
			case LogType::Error:
				return error_icon;
			default: ;
			}
			break;
		case Qt::UserRole:
			return 1 << static_cast<int>(d.type);
		}
		return QVariant();
	}
};


class LogViewFilterModel : public QSortFilterProxyModel
{
	//Q_OBJECT
public:
	LogViewFilterModel(QObject *parent = nullptr)
		: QSortFilterProxyModel(parent)
	{

	}


	virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override
	{
		auto index = sourceModel()->index(source_row, 0, source_parent);
		auto type = sourceModel()->data(index, Qt::UserRole).toInt();
		return type & m_type;
	}

	void enableType(LogType type)
	{
		m_type |= 1 << static_cast<int>(type);
		invalidateFilter();
	}

	void disableType(LogType type)
	{
		m_type &= ~(1 << static_cast<int>(type));
		invalidateFilter();
	}

	void changeType(LogType type, bool enable)
	{
		if (enable)
			enableType(type);
		else
			disableType(type);
	}

private:
	int m_type = 0;
};


LogView::LogView(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LogView)
{
	ui->setupUi(this);
	
	m_model = new LogViewModel();
	//ui->tableView->setModel(m_model);

	ui->infoButton->setChecked(true);
	ui->warnButton->setChecked(true);
	ui->errorButton->setChecked(true);

	m_filterModel = new LogViewFilterModel();
	m_filterModel->enableType(LogType::Log);
	m_filterModel->enableType(LogType::Warning);
	m_filterModel->enableType(LogType::Error);
	
	m_filterModel->setSourceModel(m_model);
	ui->tableView->setModel(m_filterModel);
	
	connect(ui->infoButton, &QToolButton::toggled, [this](){
		m_filterModel->changeType(LogType::Log, ui->infoButton->isChecked());
	});
	
	connect(ui->warnButton, &QToolButton::toggled, [this](){
		m_filterModel->changeType(LogType::Warning, ui->warnButton->isChecked());
	});
	
	connect(ui->errorButton, &QToolButton::toggled, [this](){
		m_filterModel->changeType(LogType::Error, ui->errorButton->isChecked());
	});
}

LogView::~LogView()
{
	delete ui;
}
