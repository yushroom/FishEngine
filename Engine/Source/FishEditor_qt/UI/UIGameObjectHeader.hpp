#pragma once

#include <QWidget>
#include <memory>

namespace Ui {
class UIGameObjectHeader;
}

namespace FishEngine
{
	class GameObject;
}

class UIGameObjectHeader : public QWidget
{
	Q_OBJECT

public:
	explicit UIGameObjectHeader(QWidget *parent = 0);
	~UIGameObjectHeader();

	void Bind(std::shared_ptr<FishEngine::GameObject> go);

private:

	void OnNameChanged();
	void OnActiveCheckBoxChanged(bool);
	void OnLayerChanged(int);
	void OnTagChanged(int);

	Ui::UIGameObjectHeader *ui;

	bool m_changed      = false;

	bool m_isActive     = true;
	bool m_isStatic     = false;
	std::string m_name;

	int m_layerIndex    = -1;
	int m_layerDirtyFlag = -1;

	int m_tagIndex      = -1;
	int m_tagDirtyFlag  = -1;
};
