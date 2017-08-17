#ifndef UIASSETHEADER_HPP
#define UIASSETHEADER_HPP

#include <QWidget>

namespace Ui {
class UIAssetHeader;
}

class UIAssetHeader : public QWidget
{
	Q_OBJECT

public:
	explicit UIAssetHeader(QWidget *parent = 0);
	~UIAssetHeader();

	void CheckUpdate(std::string const & name);
	
	void SetName(std::string const & name);
	
	void SetIcon(QIcon const & icon);

private:
	Ui::UIAssetHeader *ui;

	std::string m_name;
};

#endif // UIASSETHEADER_HPP
