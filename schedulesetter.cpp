#include "schedulesetter.h"
#include "ui_schedulesetter.h"
#include<QMessageBox>
#include<QTime>
#include<QTimeEdit>

ScheduleSetter::ScheduleSetter( QStringList* Time_slots,QWidget *parent ,QTableWidget* table, EntityType Id)
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
    bool flag = false;
    QTime start = ui->Startedit->time();
    QTime end = ui->Endedit->time();

    // Check for invalid start time
    if (start.hour() > 17 || start.hour() < 7) {
        QMessageBox::warning(this, "Warning!!", "Starting hour is inappropriate!!");
        flag = true;
    }

    // Check for invalid end time
    if (end.hour() > 18 || end.hour() < 9) {
        QMessageBox::warning(this, "Warning!!", "Ending hour is inappropriate!!");
        flag = true;
    }

    // Check if start time equals end time
    if (end == start) {
        QMessageBox::warning(this, "Warning!!", "Start time of the session cannot be the end of the session!!!!");
        flag = true;
    }
    if ( end.hour() < start.hour() || ( end.hour() == start.hour() && end.minute() < start.minute()))
    {
        QMessageBox::warning(this, "Warning!!", "Ending hour is inappropriate!!!!!!");
        flag = true;
    }

    // If all validations pass
    if (!flag)
    {
        QString slot = QString("%1:%2-%3:%4")
                           .arg(start.hour(), 2, 10, QChar('0'))
                           .arg(start.minute(), 2, 10, QChar('0'))
                           .arg(end.hour(), 2, 10, QChar('0'))
                           .arg(end.minute(), 2, 10, QChar('0'));

        // Check if the time slot already exists
        if (!Time_Slots->contains(slot))
        {
            // Add the time slot to the list
            Time_Slots->append(slot);

            // Add two new rows to the table for time slot and group/room or teacher/room
            int newRow = Table->rowCount();
            Table->insertRow(newRow);
            Table->setRowHeight(newRow, 60);

            Table->insertRow(newRow + 1);
            Table->setRowHeight(newRow + 1, 60);

            // Add appropriate labels to the Time_Slots for vertical headers
            if (id == EntityType::Admin)
            {
                Time_Slots->append("Teacher / room");
                emit this->Added_Time_Slot(slot);
            }
            else
            {
                Time_Slots->append("Group / room");
            }

            // Update the vertical headers with the time slots
            Table->setVerticalHeaderLabels(*Time_Slots);

            // Insert QTableWidgetItem objects into the new rows
            for (int row = newRow; row <= newRow + 1; row++)
            {
                for (int column = 0; column < 6; column++)
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    Table->setItem(row, column, item);
                }
            }

            this->hide();
        }
        else
        {
            QMessageBox::warning(this, "Warning!!", "This Time Slot already exists");
        }
    }
}
