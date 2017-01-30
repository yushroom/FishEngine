#ifndef UIHEADER_HPP
#define UIHEADER_HPP

#include <QWidget>
#include <string>

namespace Ui {
class UIHeader;
}

class UIHeader : public QWidget
{
    Q_OBJECT

public:
    // show checkbox
    explicit UIHeader(std::string const & componentTypeName, bool enabled, QWidget *parent = 0);

    // hide checkbox
    explicit UIHeader(std::string const & componentTypeName, QWidget *parent = 0);

    ~UIHeader();

    // show checkBox
    bool CheckUpdate(std::string const & componentTypeName, bool & enabled);

    // hide checkBox
    void CheckUpdate(std::string const & componentTypeName);

private slots:
    void OnCheckBoxChanged(bool value);

private:
    Ui::UIHeader *ui;

    bool m_changed = false;
    bool m_enabled = true;
    std::string m_componentTypeName;
};

#endif // UIHEADER_HPP
