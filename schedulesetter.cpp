#include "schedulesetter.h"
#include "ui_schedulesetter.h"
#include<QMessageBox>
#include<QTime>
#include<QTimeEdit>

ScheduleSetter::ScheduleSetter( QStringList& Time_slots,QWidget *parent ,QTableWidget* table, EntityType Id)
    : QDialog(parent)
    , ui(new Ui::ScheduleSetter)
    ,Table(table)
    ,Time_Slots(Time_slots)
    ,id(Id)
{
    ui->setupUi(this);
    ui->Startedit->setDisplayFormat("HH:mm");
    ui->Endedit->setDisplayFormat("HH:mm");
}

ScheduleSetter::~ScheduleSetter()
{
    delete ui;
}

void ScheduleSetter::on_pushButton_clicked()
{
    bool flag = 0;
    QTime start = ui->Startedit->time();
    if (start.hour() > 17 || start.hour() < 7 ){
        QMessageBox::about(this, "Waring!!", "starting hour is inappropriate!!");
        flag = 1;
    }
    QTime end = ui->Endedit->time();
    if (end.hour() > 18 || end.hour() < 9 ){
        QMessageBox::about(this, "Waring!!", "Ending hour is inappropriate!!");
        flag = 1;
    }
    if (end == start ){
        QMessageBox::about(this, "Waring!!", "Start time of the session can not be the End of the session!!!!");
        flag = 1;
    }

    if (! flag)
    {
        QString slot = QString("%1").arg(start.hour(), 2, 10, QChar( '0' ) ) + ":"+QString("%1").arg(start.minute(), 2, 10, QChar('0')) + "-" + QString("%1").arg(end.hour(), 2, 10, QChar('0')) + ":" + QString("%1").arg(end.minute(), 2, 10, QChar('0'));
        Time_Slots.push_back(slot);
        Table->insertRow(Table->rowCount());
        Table->setRowHeight(Table->rowCount() - 1, 60);
        Table->insertRow(Table->rowCount());
        Table->setRowHeight(Table->rowCount() - 1, 60);
        if (id == EntityType::Admin)
        {
            Time_Slots.push_back("Teacher / room");
            emit this->Added_Time_Slot(slot);
        }
        else Time_Slots.push_back("Group / room");
        for ( auto time : Time_Slots)
        {
            qDebug()<<time;
        }
        Table->setVerticalHeaderLabels(Time_Slots);
        for (int row = Table->rowCount() - 2 ; row < Table->rowCount(); row++)
        {
            for( int column = 0; column < 6; column++)
            {
                QTableWidgetItem * item = new QTableWidgetItem();
                Table->setItem(row,column,item);
            }
        }
        this->hide();
    }
}

