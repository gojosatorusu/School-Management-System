#ifndef ADDGROUP_H
#define ADDGROUP_H

#include <QDialog>
#include "Entity.h"

namespace Ui {
class AddGroup;
}

class AddGroup : public QDialog
{
    Q_OBJECT

public:
    explicit AddGroup(QWidget *parent = nullptr, School* = nullptr);
    ~AddGroup();

private slots:
    void on_AddButton_clicked();
signals:
    void groupAdded();

private:
    Ui::AddGroup *ui;
    School* school;
};

#endif // ADDGROUP_H
