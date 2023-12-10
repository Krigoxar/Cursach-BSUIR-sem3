#include "startwindow.h"
#include "ui_startwindow.h"

#include "dbcontroller.h"
#include "model.h"

StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    StartWindow::connect(ui->SwitchStateButton, &QAbstractButton::clicked,
                         this, &StartWindow::SwitchState);
    StartWindow::connect(ui->ReadyButton, &QAbstractButton::clicked,
                         this, &StartWindow::LogIn);
    this->state = StatesEnum::Login;
    //std::string str = bsoncxx::to_json(make_document(kvp("asas", "asda")));
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::SetState(StatesEnum state)
{
    this->state = state;
    switch (state)
    {
    case StatesEnum::Login:
        ui->SwitchStateButton->setText(QString("or up"));
        ui->ReadyButton->setText(QString("Sing in"));
        break;
    case StatesEnum::Signup:
        ui->SwitchStateButton->setText(QString("or in"));
        ui->ReadyButton->setText(QString("Sign up"));
        break;
    default:
        break;
    }
}

StartWindow::StatesEnum StartWindow::GetState()
{
    return StatesEnum::Login;
}

void StartWindow::SwitchState()
{
    if(this->state == StatesEnum::Login)
    {
        SetState(StatesEnum::Signup);
    }
    else
    {
        SetState(StatesEnum::Login);
    }
}

void StartWindow::LogIn()
{
    LogPass logPass = LogPass(ui->LoginLineEdit->text().toStdString(), ui->PasswordLineEdit->text().toStdString());
    DBController* dbc = DBController::GetInstance();
    if(state == StatesEnum::Login)
    {
        if(!dbc->accs.IsAccountExist(logPass))
        {
            return;
        }

        AccountDB* fullAcc = dbc->accs.FindFullAccount(logPass);

        if(fullAcc->AccountType == AccountType::Student || fullAcc->AccountType == AccountType::Teacher)
        {
            wStud.setAccAndFill(fullAcc);
            wStud.show();
            this->close();
        }
        else if(fullAcc->AccountType == AccountType::Admin)
        {
            wAdmin.show();
            this->close();
        }
    }
    else if (state == StatesEnum::Signup)
    {
        if(dbc->accs.IsAccountExist(logPass))
        {
            return;
        }
        Account* ac = new Account(logPass.Login, logPass.Password, AccountType::Student);
        dbc->accs.Add(ac);
    }
}
