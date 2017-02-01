#ifndef UIHEADER_HPP
#define UIHEADER_HPP

#include <QWidget>
#include <string>

#include "UIHeaderState.hpp"

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
    FishEditor::UIHeaderState CheckUpdate(std::string const & componentTypeName, bool & enabled);

    // hide checkBox
    FishEditor::UIHeaderState CheckUpdate(std::string const & componentTypeName);


private slots:
    void OnCheckBoxChanged(bool value);
    void OnRemoveActionTrigered() { m_removeActionTrigered = true; }

private:
    Ui::UIHeader *ui;

    //bool m_changed = false;
    bool m_enabled = true;

    bool m_enabledChanged = false;
    bool m_removeActionTrigered = false;

//    void ResetState()
//    {
//        m_enabledChanged = false;
//        m_removeActionTrigered = false;
//    }

    std::string m_componentTypeName;
};

#endif // UIHEADER_HPP
