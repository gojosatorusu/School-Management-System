// addgroup.cpp
#include "addgroup.h"
#include "ui_addgroup.h"

AddGroup::AddGroup(QWidget *parent, School* schoolptr)
    : QDialog(parent)
    , ui(new Ui::AddGroup)
    , school(schoolptr)
{
    ui->setupUi(this);
    for (int i = 0; i < school->getNumberofyears();) {
        ui->YearcomboBox->addItem(QString::number(++i));
    }
}

AddGroup::~AddGroup()
{
    delete ui;
}

void AddGroup::on_AddButton_clicked()
{
    if (ui->YearcomboBox->currentIndex() != 0) {
        int year = ui->YearcomboBox->currentIndex() ;
        int last = 0;
        for (auto group : school->getYears()[year - 1].getGroups())
        {
            if( last < group->getGroupNumber())
                last = group->getGroupNumber();
        }
        Group* group = new Group(last+1, year, school->getdatabase());
        school->getYears()[year - 1].addGroup(group);

        QSqlDatabase* Sqlbase = school->getdatabase();
        if (!Sqlbase->open())
        {
            qWarning() << "Database is not open! 1";
            return;
        }

        if (!Sqlbase->isOpen())
        {
            qWarning() << "Database is not open! 2";
            return;
        }
        QSqlQuery query(*Sqlbase);
        query.prepare("INSERT INTO `Groups` (Year, `Group`) VALUES (:year, :group)");
        query.bindValue(":year", year);
        query.bindValue(":group", group->getGroupNumber());

        if (!query.exec())
        {
            qWarning() << "Failed to Add the group:" << query.lastError().text();
            return;
        }
        query.prepare("INSERT INTO StudentSchedules (Year, `Group`, ScheduleItems) VALUES (:year, :group, :item)");
        query.bindValue(":year", year);
        query.bindValue(":group", group->getGroupNumber());
        query.bindValue(":item","");

        if (!query.exec())
        {
            qWarning() << "Failed to Create the Group Schedule " << query.lastError().text();
            return;
        }

        school->setaddedgroup(group);
        emit groupAdded();
        accept(); // Close the dialog with QDialog::Accepted status
    }
}


