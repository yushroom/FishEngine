#ifndef TEXTUREIMPORTERINSPECTOR_HPP
#define TEXTUREIMPORTERINSPECTOR_HPP

#include <QWidget>

namespace FishEditor
{
	class TextureImporter;
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

class TextureImporterInspector : public QWidget
{
    Q_OBJECT

public:
    explicit TextureImporterInspector(QWidget *parent = 0);
    ~TextureImporterInspector();
	
	void Bind(std::shared_ptr<FishEditor::TextureImporter> const & importer);
	
	
	// unbind with importer and setHidden(true)
	void Unbind();

private:
	
	void Apply();
	void Revert();
	
	void SetDirty(bool dirty);
	
	QVBoxLayout		* m_verticalLayout;
	UIAssetHeader	* m_assetHeader;
	
	UIComboBox		* m_typeCombox;
	UIComboBox		* m_shapeCombox;
	UIBool			* m_readWriteToggle;
	UIBool			* m_mipmapToggle;
	UIComboBox		* m_filterModeCombox;
	UIComboBox		* m_wrapModeCombox;
	
	bool m_isDirty = false;
	
	std::unique_ptr<FishEditor::TextureImporter> m_cachedImporter;
	
	UIRevertApplyButtons * m_revertApplyButtons;
	
	std::shared_ptr<FishEditor::TextureImporter> m_importer;
};

#endif // TEXTUREIMPORTERINSPECTOR_HPP
