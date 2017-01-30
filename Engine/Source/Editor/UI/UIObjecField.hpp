#ifndef UIOBJECFIELD_HPP
#define UIOBJECFIELD_HPP

#include <QWidget>

namespace Ui {
class UIObjecField;
}

//namespace FishEngine
//{
//    class Object;
//}

class UIObjecField : public QWidget
{
    Q_OBJECT

public:
    explicit UIObjecField(std::string const & label, std::string const & objectName, QWidget *parent = 0);
    ~UIObjecField();

private:
    Ui::UIObjecField *ui;

    bool        m_changed = false;
    std::string m_label;
    std::string m_objectName;

    //std::weak_ptr<FishEngine::Object> m_object;
};

#endif // UIOBJECFIELD_HPP
