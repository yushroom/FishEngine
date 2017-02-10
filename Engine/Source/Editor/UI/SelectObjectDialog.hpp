#ifndef SELECTOBJECTDIALOG_HPP
#define SELECTOBJECTDIALOG_HPP

#include <QDialog>
#include <memory>
#include <functional>

namespace Ui {
class SelectObjectDialog;
}

namespace FishEngine
{
    class Object;
}

class ObjectListModel;

class SelectObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectObjectDialog(QWidget *parent = 0);
    ~SelectObjectDialog();

    typedef std::function<void(std::shared_ptr<FishEngine::Object>)> Callback;

    //template <class T>
    void ShowWithCallback( Callback const & callback );

private:
    void OnIconSizeChanged(int size);
    void OnListViewSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::SelectObjectDialog *ui;

    int m_listViewIconSize = 16;
    ObjectListModel * m_model;

    Callback m_callback;
};

#endif // SELECTOBJECTDIALOG_HPP
