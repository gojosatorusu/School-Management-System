 #include "groupeditor.h"
#include "ui_groupeditor.h"
#include <QMessageBox>
GroupEditor::GroupEditor(QWidget *parent, Group* groupptr, School* schoolptr)
    : QDialog(parent)
    , ui(new Ui::GroupEditor)
    ,group(groupptr)
    ,school(schoolptr)
{
    ui->setupUi(this);
    if(group->getDelegate())
        ui->DelegateLineEdit->setText(group->getDelegate()->getName() + " (" + QString::number(group->getDelegate()->getID()) + ")"); // i must remember to use the empty button that will disqualify everyone from being competent from the position and remember to make him not able to choose the same guy
    if ( group->getSubDelegate())
        ui->ViceDelegateLineEdit->setText(group->getSubDelegate()->getName()+ " (" + QString::number(group->getSubDelegate()->getID()) + ")");
    fillComboboxes();
    ui->NumberofStudents->setText(QString::number(group->getNumberOfStudents()));
    ui->Malepercentage->setText(QString::number(group->MaleRate()*100));
    ui->FemalePercentage->setText(QString::number(group->FemaleRate()*100));
    ui->GroupNumberLineEdit->setText(QString::number(group->getGroupNumber()));
}

void GroupEditor::fillComboboxes()
{
    ui->DelegateComboBox->addItem("Students");
    ui->ViceDelegateComboBox->addItem("Students");
    if(group->getDelegate())
        for ( auto student : group->getGroupStudents())
        {
            if (student != group->getDelegate() && student != group->getSubDelegate() ){
                ui->DelegateComboBox->addItem(student->getName() + " (" + QString::number(student->getID()) + ")" );
                qDebug()<<" in this case the delegate is here present              the student which is a candidate is : "<<student->getName();
            }
            qDebug()<<"the student which is already a delegate or a sub delegate is "<<student->getName();
        }
    else
    {
        for ( auto student : group->getGroupStudents())
        {
            if ( student != group->getSubDelegate()){
            ui->DelegateComboBox->addItem(student->getName() + " (" + QString::number(student->getID()) + ")" );
            qDebug()<<" in this case the delegate is not here present              the student which is a candidate is : "<<student->getName();
            }
            else
                qDebug()<<"the Subdelegate is present  "<<student->getName();

        }
    }

    if(group->getSubDelegate())
        for ( auto student : group->getGroupStudents())
        {
            if (student != group->getDelegate() && student != group->getSubDelegate() ){
                ui->ViceDelegateComboBox->addItem(student->getName() + " (" + QString::number(student->getID()) + ")" );
                qDebug()<<" in this case the Subdelegate is here present              the student which is a candidate is : "<<student->getName();
            }
            qDebug()<<"the student which is already a delegate or a sub delegate is "<<student->getName();
        }
    else
    {
        for ( auto student : group->getGroupStudents())
        {
            if ( student != group->getDelegate()){

                ui->ViceDelegateComboBox->addItem(student->getName() + " (" + QString::number(student->getID()) + ")" );
                qDebug()<<" in this case the Subdelegate is not here present              the student which is a candidate is : "<<student->getName();
            }
            else
                qDebug()<<"the delegate is present  "<<student->getName();

        }

    }

}


GroupEditor::~GroupEditor()
{
    delete ui;
}



void GroupEditor::on_Savebutton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save?", "Would you like to Save the changes " , QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        // this function will change the group number ( students, database ...)
        bool ok;
        int newGroupNumber = ui->GroupNumberLineEdit->text().toInt(&ok);
        if ( ok)
        {
            if ( newGroupNumber != group->getGroupNumber())
            {
                for ( auto group : school->getYears()[group->getyear()-1].getGroups())
                    if( newGroupNumber == group->getGroupNumber())
                    {
                        QMessageBox::warning(this, "Warning!!!!", "the new Group Number you have enterd is not valid : the GroupNumber is already in use.");
                        return;
                    }
                for (auto student : group->getGroupStudents())
                {
                    // Update the student's year and group
                    student->setgroup(newGroupNumber);

                    // Update the student in the database
                    student->update_Student(school->getdatabase());
                }
                for (auto student: school->getRemovedStudents())
                {

                    if ( student->getYear() == newGroupNumber)
                    {
                        group->addStudent(student);
                    }
                }
                emit GroupHasBeenChanged(newGroupNumber);
                school->updateGroupNumber(group,newGroupNumber);
                group->setGroupNumber(newGroupNumber);
                qDebug()<<"the new Group Number is : "<<group->getGroupNumber();
            }
        }
        else
        {
            QMessageBox::warning(this, "Warning!!!!", "the new Group Number you have enterd is not valid : it must contain digits only");
            return;
        }
        Student* olddelegate;
        Student* newdelegate;
        if ( ui->DelegateComboBox->currentText() != ui->ViceDelegateComboBox->currentText() )
        {
            if ( ui->DelegateLineEdit->text() != "" && ui->DelegateComboBox->currentText() != "Students")
            {
                qDebug()<<"the contents of ui->DelegateLineEdit->text()  :         "<<ui->DelegateLineEdit->text();
                olddelegate = school->findStudentByName(ui->DelegateLineEdit->text());
                qDebug()<<"old delegaet found"<<olddelegate->getName();
                if ( olddelegate)
                {
                    qDebug()<<"old delegaet found"<<olddelegate->getName();
                    qDebug()<<olddelegate->getName();
                    olddelegate->setposition("Student");
                    group->setDelegate(nullptr);
                }
                else
                {
                    qDebug()<<"old delegaet not found";
                }
                olddelegate->update_Student(school->getdatabase());
                newdelegate = school->findStudentByName(ui->DelegateComboBox->currentText());
                if ( newdelegate)
                {
                    qDebug()<<newdelegate->getName();
                    newdelegate->setposition("Delegate");
                    group->setDelegate(newdelegate);
                }
                newdelegate->update_Student(school->getdatabase());
            }
            else if (ui->DelegateLineEdit->text() == "" && ui->DelegateComboBox->currentText() != "Students")
            {
                newdelegate = school->findStudentByName(ui->DelegateComboBox->currentText());
                if ( newdelegate)
                {
                    qDebug()<<newdelegate->getName();
                    newdelegate->setposition("Delegate");
                    group->setDelegate(newdelegate);
                }
                newdelegate->update_Student(school->getdatabase());
            }

            if ( ui->ViceDelegateLineEdit->text() != "" && ui->ViceDelegateComboBox->currentText() != "Students" )
            {
                olddelegate = school->findStudentByName(ui->ViceDelegateLineEdit->text());
                if ( olddelegate)
                {
                    qDebug()<<olddelegate->getName();
                    olddelegate->setposition("Student");
                    group->setSubDelegate(nullptr);
                }
                olddelegate->update_Student(school->getdatabase());

                newdelegate = school->findStudentByName(ui->ViceDelegateComboBox->currentText());
                if ( newdelegate)
                {
                    qDebug()<<newdelegate->getName();
                    newdelegate->setposition("Sub-Delegate");
                    group->setSubDelegate(newdelegate);
                }
                newdelegate->update_Student(school->getdatabase());
            }
            else if (ui->ViceDelegateLineEdit->text() == "" && ui->ViceDelegateComboBox->currentText() != "Students" )
            {
                newdelegate = school->findStudentByName(ui->ViceDelegateComboBox->currentText());
                if ( newdelegate)
                {
                    qDebug()<<newdelegate->getName();
                    newdelegate->setposition("Sub-Delegate");
                    group->setSubDelegate(newdelegate);
                }
                newdelegate->update_Student(school->getdatabase());
            }
            this->deleteLater();
        }
        else if (ui->ViceDelegateComboBox->currentText() == "Students" && ui->DelegateComboBox->currentText() == "Students" )
        {
            this->deleteLater();
        }
        else{
            QMessageBox::warning(this,"Warning !! ", "The delegate and Sub-Delegate cannot be the same Student"); // this is an additional step to make sure that in any way the sub-delegate is not the same person as the delegate
            return;
        }
    }
    else
    {
        this->deleteLater();
    }
}



//these following functions are to make sure that no student can be chosen twice and also to make switching the delegate and his vice-delegate easier



void GroupEditor::on_Disqualifybuttondelegate_clicked()
{


    ui->DelegateComboBox->clear();
    ui->DelegateComboBox->addItem("Students");
    if ( ui->DelegateLineEdit->text() != "" )
    {
        Student* olddelegate = school->findStudentByName(ui->DelegateLineEdit->text());
        if ( olddelegate)
        {
            qDebug()<<olddelegate->getName();
            olddelegate->setposition("Student");
            group->setDelegate(nullptr);
        }
        olddelegate->update_Student(school->getdatabase());
        ui->ViceDelegateComboBox->addItem(olddelegate->getName() + " (" + QString::number(olddelegate->getID()) + ")" );
    }
    for ( auto student : group->getGroupStudents())
    {
        if ( student != group->getSubDelegate())
            ui->DelegateComboBox->addItem(student->getName() + " (" + QString::number(student->getID()) + ")" );
    }
    ui->DelegateLineEdit->setText("");
}


void GroupEditor::on_DisqualifybuttonSubdelegate_clicked()
{


    ui->ViceDelegateComboBox->clear();
    ui->ViceDelegateComboBox->addItem("Students");
    if ( ui->ViceDelegateLineEdit->text() != "" )
    {
        Student* olddelegate = school->findStudentByName(ui->ViceDelegateLineEdit->text());
        if ( olddelegate)
        {
            qDebug()<<olddelegate->getName();
            olddelegate->setposition("Student");
            group->setSubDelegate(nullptr);
        }
        olddelegate->update_Student(school->getdatabase());
        ui->DelegateComboBox->addItem(olddelegate->getName() + " (" + QString::number(olddelegate->getID()) + ")" );
    }
    for ( auto student : group->getGroupStudents())
    {
        if ( student != group->getDelegate())
            ui->ViceDelegateComboBox->addItem(student->getName() + " (" + QString::number(student->getID()) + ")" );
    }
    ui->ViceDelegateLineEdit->setText("");
}


void GroupEditor::on_pushButton_clicked()
{
    Student* tempstudent = group->getDelegate();
    group->setDelegate(group->getSubDelegate());
    group->setSubDelegate(tempstudent);

    if (group->getDelegate())
    {
        group->getDelegate()->setposition("Delegate");
    }
    if (group->getSubDelegate())
    {
        group->getSubDelegate()->setposition("Sub-Delegate");
    }

    ui->DelegateComboBox->clear();
    ui->ViceDelegateComboBox->clear();
    fillComboboxes();

    if(group->getDelegate())
        ui->DelegateLineEdit->setText(group->getDelegate()->getName() + " (" + QString::number(group->getDelegate()->getID()) + ")"); // i must remember to use the empty button that will disqualify everyone from being competent from the position and remember to make him not able to choose the same guy
    else
        ui->DelegateLineEdit->setText("");

    if (group->getSubDelegate())
        ui->ViceDelegateLineEdit->setText(group->getSubDelegate()->getName()+ " (" + QString::number(group->getSubDelegate()->getID()) + ")");
    else
        ui->ViceDelegateLineEdit->setText("");
}



void GroupEditor::on_DeleteGroupButton_clicked()
{

    if (!group->Alldeleted()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Group has Students",
            "This group has students. Would you like to delete them definitively too?, this will NOT place them in the RECYCLE BIN !!",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
            );

        if (reply == QMessageBox::Yes)
        {
            school->deletestudentsfromgroup(group->getGroupStudents()); // Implement this function to delete all students from the group
        }
        else if (reply == QMessageBox::No)
        {
            QMessageBox::information(this, "Change Groups", "Please change the groups of the students first.");
            return;
        }
        else
        {
            return;
        }
    }

    // Confirm group deletion
    QMessageBox::StandardButton confirm = QMessageBox::question(
        this, "Confirm Deletion",
        QString("Are you sure you want to delete the group '%1'?").arg(group->getGroupNumber()),
        QMessageBox::Yes | QMessageBox::No
        );

    if (confirm == QMessageBox::Yes) {
        school->deleteGroupdefinitively(group); // Implement this function to delete the group from the data structure or database
        delete group; // Remove from QListWidget
    }
        this->deleteLater();
}


void GroupEditor::on_ChangeGroupButton_clicked()
{
    ui->GroupNumberLineEdit->setReadOnly(false);
}

