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
    ~UIMaterialHeader();

private:
    Ui::UIMaterialHeader *ui;
};

#endif // UIMATERIALHEADER_HPP
