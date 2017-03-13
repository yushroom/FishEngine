#pragma once

#include <QWidget>
#include <memory>

namespace FishEditor
{
	class ModelImporter;
}

namespace FishEngine
{
	enum class FilterMode;
	enum class TextureWrapMode;
}

class QVBoxLayout;
class UIAssetHeader;
class UIComboBox;
class UIBool;
class UIRevertApplyButtons;

class ModelImporterInspector : public QWidget
{
	Q_OBJECT
	
public:
	explicit ModelImporterInspector(QWidget *parent = 0);
	~ModelImporterInspector();
	
	void Bind(std::shared_ptr<FishEditor::ModelImporter> const & importer);
	
	
	// unbind with importer and setHidden(true)
	void Unbind();
	
private:
	
	void Apply();
	void Revert();
	
	void SetDirty(bool dirty);
	
	QVBoxLayout		* m_verticalLayout;
	UIAssetHeader	* m_assetHeader;
	
	UIComboBox		* m_normalsCombox;
	UIComboBox		* m_tangentsCombox;
	UIComboBox		* m_materialSearchCombox;
	
	bool m_isDirty = false;
	
	std::unique_ptr<FishEditor::ModelImporter> m_cachedImporter;
	
	UIRevertApplyButtons * m_revertApplyButtons;
	
	std::shared_ptr<FishEditor::ModelImporter> m_importer;
};

