#ifndef INSPECTORWIDGET_HPP
#define INSPECTORWIDGET_HPP

#include <QWidget>
#include <memory>

class QTreeWidget;
class UIGameObjectHeader;
class UIMaterialHeader;
class UIAssetHeader;
class QMenu;
class TextureImporterInspector;

namespace FishEditor
{
    class Inspector;
	class TextureImporter;
	class ModelImporter;
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
	void Bind(std::shared_ptr<FishEditor::TextureImporter> importer);
	void Bind(std::shared_ptr<FishEditor::ModelImporter> importer);

    virtual QSize sizeHint() const override
    {
        return QSize(250, 400);
    }
	
	void HideAll();

signals:

private:
	void Update();
	//std::string ShowAddComponentMenu();

    friend class FishEditor::Inspector;
    QTreeWidget         * m_treeWidget;
    UIGameObjectHeader  * m_gameObjectHeader;
    UIMaterialHeader    * m_materialHeader;
    //UIAssetHeader       * m_assetHeader;
	TextureImporterInspector * m_textureImporterInspector;

    //QMenu               * m_menu;
};

#endif // INSPECTORWIDGET_HPP
