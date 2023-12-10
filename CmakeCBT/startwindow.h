#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <string>
#include "studentwindow.h"
#include "adminwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StartWindow; }
QT_END_NAMESPACE

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

    enum StatesEnum{
        Login,
        Signup,
    };

    void SetState(StatesEnum state);
    StatesEnum GetState();

public slots:
    void SwitchState();
    void LogIn();


private:

    StudentWindow wStud;
    AdminWindow wAdmin;
    StatesEnum state;
    Ui::StartWindow *ui;
};
#endif
