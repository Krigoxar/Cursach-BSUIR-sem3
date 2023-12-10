#include "studentwindow.h"
#include "ui_studentwindow.h"

StudentWindow::StudentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StudentWindow)
{
    ui->setupUi(this);

    ui->calendarWidget->setMinimumDate(QDate::currentDate());
    date = QDate::currentDate();
    this->connect(ui->calendarWidget, &QCalendarWidget::clicked,
                  this, &StudentWindow::Setdate);
}

void StudentWindow::Setdate(QDate date)
{
    this->date = date;
    UpdateWindow();
}

void StudentWindow::UpdateWindow()
{
    ui->Schedule->setSchedules(DBController::GetInstance()->sched.GetAllSchedulesinDate(date.toJulianDay(), dynamic_cast<StudentInfoDB*>(acc->getAdditionalInfo())->getGroup()));
}

StudentWindow::~StudentWindow()
{
    delete ui;
}

void StudentWindow::setAccAndFill(AccountDB* Acc)
{
    acc = Acc;
    UpdateWindow();
    ui->labelPasswordVal->setText(acc->Password.c_str());
    ui->labelLoginVal->setText(acc->Login.c_str());
    ui->labelTypeVal->setText(myto_string(acc->AccountType).c_str());
    auto addInf = dynamic_cast<StudentInfoDB*>(Acc->getAdditionalInfo());
    GroupDB* grp = NULL;
    if(addInf != NULL)
    {
        ui->labelFirstName->setText(addInf->FirstName.c_str());
        ui->labelSeconDame->setText(addInf->SecondName.c_str());
        ui->labelCurs->setText(std::to_string(addInf->Curs).c_str());
        grp = addInf->getGroup();
    }
    else
    {
        ui->labelFirstName->setText("");
        ui->labelSeconDame->setText("");
        ui->labelCurs->setText("");
    }
    if(grp != NULL)
    {
        ui->labelGroup->setText(grp->Name.c_str());
    }
    else
    {
        ui->labelGroup->setText("");
    }
}
