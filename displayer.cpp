#include "displayer.h"
#include "ui_displayer.h"
#include <QMessageBox>
#include <QFileDialog>

Displayer::Displayer(QWidget *parent, Entity *iteminfo, QListWidgetItem* listItem, QSqlDatabase * sqlbase, School * schoolptr, EntityType ID)
    : QDialog(parent)
    , ui(new Ui::Displayer)
    , item(iteminfo)
    ,ListItem(listItem)
    ,Sqlbase(sqlbase)
    ,school(schoolptr)
    ,id(ID)
{
    ui->setupUi(this);
    qDebug() << "Displayer created";
    if (!iteminfo) {
        qDebug() << "Item info is nullptr";
        return;
    }

    Teacher* teacher = dynamic_cast<Teacher*>(iteminfo);
    Student* student = dynamic_cast<Student*>(iteminfo);
    if (teacher) {
        qDebug() << "Displaying Teacher: " << iteminfo->getName();

        ui->YearandGroup_Password->setText("Password");

        ui->Emailedit->setText(teacher->getEmail());

        ui->IDedit->setText(QString::number(teacher->getID()));

        ui->Phone_numberedit->setText(teacher->getPhone_Number());

        ui->Nameedit->setText(teacher->getName());

        ui->YearandGroup_passwordedit->setText(teacher->getpassword());

        QPixmap pixmap(teacher->getImagepath());

        ui->Image->setPixmap(pixmap.scaled(ui->Image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        if ( teacher->getName() == "Admin")
        {
            ui->DeleteStudent_teacherButton->hide();
        }

    } else if (student) {
        qDebug() << "Displaying Student: " << iteminfo->getName();

        ui->Emailedit->setText(student->getEmail());

        ui->IDedit->setText(QString::number(student->getID()));

        ui->Phone_numberedit->setText(student->getPhone_Number());

        ui->Nameedit->setText(student->getName());

        ui->YearandGroup_passwordedit->setText(QString::number(student->getYear()) + "Y" + QString::number(student->getgroup()) + "G");

        QPixmap pixmap(student->getImagepath());
        ui->Image->setPixmap(pixmap.scaled(ui->Image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        if ( id == EntityType::Teacher)
        {
            ui->Emailedit->setReadOnly(true);
            ui->IDedit->setReadOnly(true);
            ui->Imagebutton->hide();
            ui->Phone_numberedit->setReadOnly(true);
            ui->Nameedit->setReadOnly(true);
            ui->YearandGroup_passwordedit->setReadOnly(true);
            ui->DeleteStudent_teacherButton->hide();
        }
    } else {
        qDebug() << "Invalid item info pointer";
    }
}

Displayer::~Displayer() {
    qDebug() << "Displayer destroyed";
    delete ui;
}

void Displayer::on_Savebutton_clicked() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save?", "Would you like to save the details?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (!item) {
            qDebug() << "Item info is nullptr";
            return;
        }

        Teacher* teacher = dynamic_cast<Teacher*>(item);
        Student* student = dynamic_cast<Student*>(item);
        if (teacher) {
            qDebug() << "Saving Teacher: " << teacher->getName();
            teacher->setEmail(ui->Emailedit->text());
            teacher->setID(ui->IDedit->text().toInt());
            teacher->setPhone_Number(ui->Phone_numberedit->text());
            teacher->setName(ui->Nameedit->text());
            teacher->setpassword(ui->YearandGroup_passwordedit->text());
            ListItem->setText(teacher->getName()+" ("+QString::number(teacher->getID())+")");
            teacher->update_Teacher(Sqlbase);
            this->close();
        } else if (student) {
            qDebug() << "Saving Student: " << student->getName();
            student->setEmail(ui->Emailedit->text());
            student->setID(ui->IDedit->text().toInt());
            student->setPhone_Number(ui->Phone_numberedit->text());
            student->setName(ui->Nameedit->text());
            ListItem->setText(student->getName()+" ("+QString::number(student->getID())+")");
            student->update_Student(Sqlbase);
            this->close();
        }
        else
        {
            qDebug() << "Invalid item info pointer";
        }
    } else
    {
        delete this;
    }
}




void Displayer::on_Imagebutton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        ui->Image->setPixmap(pixmap.scaled(ui->Image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Save the image path to the teacher object (this assumes you have a setter in your Teacher class)
        Teacher* teacher = dynamic_cast<Teacher*>(item);
        if (teacher) {
            teacher->setImagepath(fileName);
        }
        Student* student = dynamic_cast<Student*>(item);
        if (student) {
            student->setImagepath(fileName);
        }
    }
}

void Displayer::on_ChangeGroup_PasswordButton_clicked()
{
    Teacher* teacher = dynamic_cast<Teacher*>(item);
    Student* student = dynamic_cast<Student*>(item);
    if (teacher)
    {
        ui->YearandGroup_passwordedit->setReadOnly(false);
        QMessageBox::warning(this,"Warning!!","Now you can Change the Password");
    }
    else if (student)
    {
        Changer = new ChangeStudentGroup(this,school,student);
        connect(Changer, &QObject::destroyed, this, [this,student](){
            Student* student = dynamic_cast<Student*>(item);
            ui->YearandGroup_passwordedit->setText(QString::number(student->getYear()) + "Y" + QString::number(student->getgroup()) + "G");
        }); // this lambda function is used here instead of connecting to a function
        Changer->show();

    }
    else
    {
        qDebug() << "Invalid item info pointer";
    }

}

void Displayer::on_DeleteStudent_teacherButton_clicked()
{
    // remember to display the confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove?", "Would you like to remove the Entity ?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        Student* student = dynamic_cast<Student*>(item);
        Teacher* teacher = dynamic_cast<Teacher*>(item);
        reply = QMessageBox::question(this, "Remove?", "Would you like to remove the Entity definitively ?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            if ( student )
            {
                school->removeStudentDefinitively(student);
            emit removeStudent();
            }
            else if ( teacher )
            {
                school->removeTeacherDefinitively( teacher );
                emit removeteacher();
            }
        }
        else
        {
            item->setdeletion_State(true);
            if ( student )
            {
                school->addremmovedstudent(student);
                student->update_Student(school->getdatabase());
                emit removeStudent();
            }
            else if (teacher)
            {
                teacher->update_Teacher(school->getdatabase());
                emit removeteacher();
            }
        }

    }
    this->deleteLater();

}

