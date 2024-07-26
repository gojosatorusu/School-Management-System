#ifndef SCHEDULESETTER_H
#define SCHEDULESETTER_H

#include <QDialog>
#include<QTableWidget>
#include "Entity.h"
namespace Ui {
class ScheduleSetter;
}

class ScheduleSetter : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleSetter(QStringList &,QWidget *parent = nullptr,QTableWidget* = nullptr, EntityType = EntityType::Teacher);
    ~ScheduleSetter();

private slots:
    void on_pushButton_clicked();
public : signals:
    void Added_Time_Slot(QString);

private:
    Ui::ScheduleSetter *ui;
    QTableWidget* Table;
    QStringList Time_Slots;
    EntityType id;
};

#endif // SCHEDULESETTER_H
