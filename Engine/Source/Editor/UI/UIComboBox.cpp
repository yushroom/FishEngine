#include "UIComboBox.hpp"
#include "ui_UIComboBox.h"

#include "../UIDebug.hpp"

UIComboBox::UIComboBox(std::string const & label, int index, const char * const *enumStringArray, int arraySize, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIComboBox),
    m_label(label),
    m_enumStringArray(enumStringArray),
    m_index(index)
{
    ui->setupUi(this);
    ui->horizontalLayout->setContentsMargins(0, 2, 2, 2);

    ui->label->setText(m_label.c_str());

    QStringList sl;
    for (int i = 0; i < arraySize; i++)
        sl << enumStringArray[i];
    ui->comboBox->addItems(sl);
    ui->comboBox->setCurrentIndex(m_index);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboBoxChanged(int)));
}

UIComboBox::~UIComboBox()
{
    delete ui;
}

bool UIComboBox::CheckUpdate(const std::string &label, int & index, const char * const *enumStringArray, int arraySize)
{
    if (m_changed)
    {
        FishEngine::Debug::Log("[Enum] index changed");
        index = m_index;
        m_changed = false;
        return true;
    }

    if (m_label != label)
    {
        FishEngine::Debug::Log("[Enum] new label: %s", label.c_str());
        m_label = label;
        LOG;
        ui->label->setText(m_label.c_str());
    }

    if (enumStringArray != m_enumStringArray)
    {
        FishEngine::Debug::Log("[Enum] new enum");
        m_enumStringArray = enumStringArray;
        m_index = index;
        QStringList sl;
        for (int i = 0; i < arraySize; i++)
            sl << enumStringArray[i];
        LOG;
        ui->comboBox->blockSignals(true);
        ui->comboBox->clear();
        ui->comboBox->addItems(sl);
        ui->comboBox->setCurrentIndex(m_index);
        ui->comboBox->blockSignals(false);
        return false;
    }

    if (m_index != index)
    {
        FishEngine::Debug::Log("[Enum] new index");
        m_index = index;
        LOG;
        ui->comboBox->blockSignals(true);
        ui->comboBox->setCurrentIndex(index);
        ui->comboBox->blockSignals(false);
    }
    return false;
}

void UIComboBox::OnComboBoxChanged(int index)
{
    m_index = index;
    m_changed = true;
}
