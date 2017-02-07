#ifndef INSPECTORWIDGET_HPP
#define INSPECTORWIDGET_HPP

#include <QWidget>
#include <memory>

class QTreeWidget;
class UIGameObjectHeader;
class UIMaterialHeader;
class UIAssetHeader;

namespace FishEditor
{
    class Inspector;
}

namespace FishEngine
{
    class GameObject;
    class Object;
}


class InspectorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InspectorWidget(QWidget *parent = 0);

    void Bind(std::shared_ptr<FishEngine::Object> obj);
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
    UIAssetHeader       * m_assetHeader;
};

#endif // INSPECTORWIDGET_HPP
