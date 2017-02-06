#ifndef UIMATERIALHEADER_HPP
#define UIMATERIALHEADER_HPP

#include <QWidget>

namespace Ui {
class UIMaterialHeader;
}

class UIMaterialHeader : public QWidget
{
    Q_OBJECT

public:
    explicit UIMaterialHeader(QWidget *parent = 0);
    explicit UIMaterialHeader(std::string const & name, QWidget *parent = 0);
    ~UIMaterialHeader();

    void CheckUpdate(std::string const & name);

private:
    Ui::UIMaterialHeader *ui;

    std::string m_name;
};

#endif // UIMATERIALHEADER_HPP
