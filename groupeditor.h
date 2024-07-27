#ifndef GROUPEDITOR_H
#define GROUPEDITOR_H

#include <QDialog>
#include"Entity.h"
namespace Ui {
class GroupEditor;
}

class GroupEditor : public QDialog
{
    Q_OBJECT

public:
    explicit GroupEditor(QWidget *parent = nullptr, Group* = nullptr, School* = nullptr);
    ~GroupEditor();

private slots:
    void on_Savebutton_clicked();
    void fillComboboxes();
    void on_Disqualifybuttondelegate_clicked();
    void on_DisqualifybuttonSubdelegate_clicked();
    void on_pushButton_clicked();
    void on_DeleteGroupButton_clicked();
    void on_ChangeGroupButton_clicked();

public: signals:
    void GroupHasBeenChanged( int newGroup);
    void deletegroup();


private:
    Ui::GroupEditor *ui;
    Group* group;
    School* school;
};

#endif // GROUPEDITOR_H
