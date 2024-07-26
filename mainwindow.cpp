#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qsqlquery.h"
#include<QSql>
#include<QDir>
#include <QString>
// #include "databasemanager.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,Admin(nullptr)
    ,reguser(nullptr)
{
    ui->setupUi(this);
    // //here we will create the Structure of the Whole Management System, first we will dynamically create the School object
    sqlbase= new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    sqlbase->setDatabaseName(QDir::currentPath()+"/Database/SchoolManagmentDatabase.db");
}

MainWindow::~MainWindow()
{
    delete sqlbase;
    delete ui;
}
void MainWindow::on_Log_in_button_clicked()
{

    if( ui->comboBox->currentText() != "ID")
    {
        QString username = ui->teacherNameInput->text() ;
        QString id = ui->comboBox->currentText();
        QString password = ui->passwordInput->text();
        if (sqlbase->open())
        {
            QSqlQuery query(*sqlbase) ;
            query.prepare("SELECT Name, Password FROM Teachers WHERE Name = :username AND Password = :password AND TeacherID = :id");
            query.bindValue(":username", username);
            query.bindValue(":password", password);
            query.bindValue(":id",id);
            if (query.exec())
            {
                if ((query).next())
                {
                    this->hide();
                    if ( username == "Admin" )
                    {
                        ui->passwordInput->clear();
                        ui->teacherNameInput->clear();
                        Admin = new Adminer(this,this,sqlbase);
                        Admin->show();
                    }
                    else
                    {
                        ui->passwordInput->clear();
                        ui->teacherNameInput->clear();
                        reguser = new RegUser(this,this,username +" (" + id + ")",sqlbase);
                        reguser->show();
                    }
                }
            }
        }
        sqlbase->close();
    }
}




void MainWindow::on_teacherNameInput_textChanged(const QString &arg1)
{
    ui->comboBox->clear();
    ui->comboBox->addItem("ID");
    if( !sqlbase->open())
    {
        qDebug()<<"Database is not open !!";
        return;
    }
    QString text = ui->teacherNameInput->text();
    QSqlQuery query(*sqlbase) ;
    query.prepare("SELECT TeacherID FROM Teachers WHERE Name = :username");
    query.bindValue(":username", text);
    if( ! query.exec())
    {
        qDebug()<<"Query not executed";
        return ;
    }
    while ( query.next())
    {
        QString ID = query.value(0).toString();
        ui->comboBox->addItem(ID);
    }
}

