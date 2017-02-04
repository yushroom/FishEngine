#include "InspectorWidget.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidget>
#include "UI/UIGameObjectHeader.hpp"
#include "UI/UIMaterialHeader.hpp"

InspectorWidget::InspectorWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout * rootLayout = new QVBoxLayout(this);
    rootLayout->setMargin(0);
    rootLayout->setSpacing(0);
    setLayout(rootLayout);
    m_treeWidget = new QTreeWidget();
    m_treeWidget->setHeaderHidden(true);
    m_treeWidget->setIndentation(12);
    m_treeWidget->setSelectionMode(QAbstractItemView::NoSelection);

    m_gameObjectHeader = new UIGameObjectHeader();
    m_materialHeader = new UIMaterialHeader();
    rootLayout->addWidget(m_gameObjectHeader);
    rootLayout->addWidget(m_materialHeader);
    m_gameObjectHeader->setHidden(true);
    m_materialHeader->setHidden(true);
    rootLayout->addWidget(m_treeWidget);
}

void InspectorWidget::Bind(std::shared_ptr<FishEngine::GameObject> go)
{
    if ( m_gameObjectHeader->isHidden() )
        m_gameObjectHeader->setHidden(false);
    if ( ! m_materialHeader->isHidden() )
        m_materialHeader->setHidden(true);

    m_gameObjectHeader->Bind(go);
}
