#ifndef SESSIONACTIVATION_H
#define SESSIONACTIVATION_H

#include <QDialog>
#include "Entity.h"
namespace Ui {
class SessionActivation;
}

class SessionActivation : public QDialog
{
    Q_OBJECT

public:
    explicit SessionActivation(QWidget *parent = nullptr,School* = nullptr, Course* = nullptr);
    ~SessionActivation();

private slots:
    void on_pushButton_clicked();

    void on_LectureCheckBox_stateChanged(int arg1);

    void on_LabCheckBox_stateChanged(int arg1);

    void on_TutorialCheckBox_stateChanged(int arg1);

private:
    Ui::SessionActivation *ui;
    Course* course;
    School* school;
};

#endif // SESSIONACTIVATION_H
