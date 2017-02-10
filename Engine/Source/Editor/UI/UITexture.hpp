#ifndef UITEXTURE_HPP
#define UITEXTURE_HPP

#include <QWidget>
#include <boost/uuid/uuid.hpp>

namespace Ui {
class UITexture;
}

enum class UITextureState
{
    None,
    TextureClicked,
    SelectButtonClicked
};

class UITexture : public QWidget
{
    Q_OBJECT

public:
    explicit UITexture(std::string const & label, QWidget *parent = 0);
    ~UITexture();

    UITextureState CheckUpdate(std::string const & label, boost::uuids::uuid const & textureGUID);

private:
    void OnTextureClicked()         { m_textureClicked = true; }
    void OnSelectButtonClicked()    { m_selectButtonClicked = true; }

private:
    Ui::UITexture *ui;

    //bool m_changed               = false;
    bool m_textureClicked        = false;
    bool m_selectButtonClicked   = false;
    std::string         m_label;
    boost::uuids::uuid  m_textureGUID;
};

#endif // UITEXTURE_HPP
