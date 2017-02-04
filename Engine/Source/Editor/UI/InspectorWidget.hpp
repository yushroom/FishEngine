#ifndef INSPECTORWIDGET_HPP
#define INSPECTORWIDGET_HPP

#include <QWidget>

class QTreeWidget;
class UIGameObjectHeader;
class UIMaterialHeader;

namespace FishEditor
{
    class Inspector;
}

namespace FishEngine
{
    class GameObject;
}


class InspectorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InspectorWidget(QWidget *parent = 0);

    void Bind(std::shared_ptr<FishEngine::GameObject> go);

    virtual QSize sizeHint() const override
    {
        return QSize(250, 400);
    }

signals:

private:
    friend class FishEditor::Inspector;
    QTreeWidget         * m_treeWidget;
    UIGameObjectHeader  * m_gameObjectHeader;
    UIMaterialHeader    * m_materialHeader;
};

#endif // INSPECTORWIDGET_HPP
