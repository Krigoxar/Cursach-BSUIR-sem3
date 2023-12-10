#include "ui_adminwindow.h"
#include "adminwindow.h"

AdminWindow::AdminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    ui->tableWidget->UpdateTable();
    ui->SchedulesTable->UpdateTable();
    UpdateGroupsScrean();
    UpdateScheduleScrean();
    ui->Calendar->setMinimumDate(QDate::currentDate().addDays(1));

    AdminWindow::connect(ui->tableWidget, &AccountsTable::AccountDBChanged,
                         this, &AdminWindow::SetAccountToDisplayWidget);

    AdminWindow::connect(ui->GroupsTable, &GroupsDBTable::AccountDBChanged,
                         this, &AdminWindow::SetGroup);

    AdminWindow::connect(ui->GroupsTableSch, &GroupsDBTable::AccountDBChanged,
                         this, &AdminWindow::SetGroupSch);

    AdminWindow::connect(ui->DeleteButton, &QAbstractButton::clicked,
                         this, &AdminWindow::DeleteAccount);

    AdminWindow::connect(ui->UpdateButton, &QAbstractButton::clicked,
                         this, &AdminWindow::UpdateAccount);

    AdminWindow::connect(ui->AddButton, &QAbstractButton::clicked,
                         this, &AdminWindow::AddNewAccount);

    AdminWindow::connect(ui->DeleteGroup, &QAbstractButton::clicked,
                         this, &AdminWindow::DeleteGroup);

    AdminWindow::connect(ui->UpdateGroup, &QAbstractButton::clicked,
                         this, &AdminWindow::UpdateGroup);

    AdminWindow::connect(ui->AddGroup, &QAbstractButton::clicked,
                         this, &AdminWindow::AddNewGroup);

    AdminWindow::connect(ui->GroupedAccs, &GroupedAccountsTable::AddAcc,
                         this, &AdminWindow::AddAccToGroup);

    AdminWindow::connect(ui->NotAssignedAccs, &UngroupedAccountsTable::DelAcc,
                         this, &AdminWindow::DeleteAccFromGroup);

    AdminWindow::connect(ui->tabWidget, &QTabWidget::currentChanged,
                         this, &AdminWindow::ChangeCurentSize);

    AdminWindow::connect(ui->TaeachersTable, &TeachersTable::AccountDBChanged,
                         this, &AdminWindow::SetAccToSchedule);

    AdminWindow::connect(ui->AddClass, &QAbstractButton::clicked,
                         this, &AdminWindow::AddClass);

    AdminWindow::connect(ui->DeleteClass, &QAbstractButton::clicked,
                         this, &AdminWindow::DeleteClass);

    AdminWindow::connect(ui->Calendar, &QCalendarWidget::clicked,
                         this, &AdminWindow::DataChanged);

    AdminWindow::connect(ui->SchedulesTable, &ScheduleTable::AddSchedule,
                         this, &AdminWindow::AddSchedule);

    AdminWindow::connect(ui->ClassesTable, &PosibleLessonsTable::DeleteSchedule,
                         this, &AdminWindow::DeleteSchedule);

    SetGroup(NULL);
    initialWidth = width();
}

void AdminWindow::resizeEvent(QResizeEvent *event)
{
    switch (ui->tabWidget->currentIndex()) {
    case 0:
        initialWidth = width();

        break;
    case 1:
        initialWidth = ((width() / 4) * 3);

        break;
    case 2:
        initialWidth = (width() / 2);

        break;
    }
    QMainWindow::resizeEvent(event);
}

void AdminWindow::ChangeCurentSize(int tab)
{
    switch (tab) {
    case 0:
        resize(initialWidth, height());
        UpdateGroupsScrean();
        break;
    case 1:
        resize(((initialWidth * 4) / 3), height());
        UpdateScheduleScrean();
        break;
    case 2:
        resize((initialWidth * 2), height());
        break;
    }
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::SetAccountToDisplayWidget(AccountDB *acc)
{
    ui->AccountDisp->setAccountDB(acc);
}

void AdminWindow::SetGroup(GroupDB *grp)
{
    group = grp;
    allCurators.clear();
    ui->GroupedAccs->setGroup(grp);
    auto accs = DBController::GetInstance()->accs.GetAll();
    for(auto acc:accs)
    {
        if(acc->getAdditionalInfo() != NULL)
        {
            auto teach = dynamic_cast<TeacherInfoDB*>(acc->getAdditionalInfo());
            if(teach != NULL)
            {
                this->allCurators.push_back(teach);
            }
        }
    }
    ui->CuratorComboBox->clear();
    ui->CuratorComboBox->addItem("None");
    for(auto cur:allCurators)
    {
        ui->CuratorComboBox->addItem((cur->FirstName + " " + cur->SecondName).c_str());
    }

    ui->CuratorComboBox->setCurrentText("None");
    if(group == NULL)
    {
        ui->GroupNameEdit->setText("");
    }
    else
    {
        if(group->getCurator() != NULL)
        {
            ui->CuratorComboBox->setCurrentText((group->getCurator()->FirstName + " " + group->getCurator()->SecondName).c_str());
        }
        ui->GroupNameEdit->setText(group->Name.c_str());
    }
}

void AdminWindow::SetGroupSch(GroupDB *acc)
{
    UpdateScheduleScrean(NULL, acc);
}

void AdminWindow::SetAccToSchedule(AccountDB *acc)
{
    teacher = dynamic_cast<TeacherInfoDB*>(acc->getAdditionalInfo());
    ui->ClassesTable->setTeacher(dynamic_cast<TeacherInfoDB*>(acc->getAdditionalInfo()));
    UpdateScheduleScrean(NULL , NULL);
}

void AdminWindow::AddNewAccount()
{
    auto data = ui->AccountDisp->getNewAccount();
    auto res = DBController::GetInstance()->accs.Add(data._Myfirst._Val);
    if(data._Get_rest()._Myfirst._Val != NULL)
    {
        auto Info = data._Get_rest()._Myfirst._Val;
        Info->setOwner(res);
        DBController::GetInstance()->infos.Add(Info, res->AccountType);
    }
    ui->tableWidget->UpdateTable();
    UpdateGroupsScrean();
}

void AdminWindow::DeleteAccount()
{
    auto data = ui->AccountDisp->getAccountForDelete();
    if(data == NULL)
    {
        return;
    }
    if(data->getAdditionalInfo() != NULL)
    {
        DBController::GetInstance()->infos.Delete(data->getAdditionalInfo()->_id);
    }
    DBController::GetInstance()->accs.Delete(data->_id);
    ui->tableWidget->UpdateTable();
    ui->AccountDisp->setAccountDB(NULL);
}

void AdminWindow::UpdateAccount()
{
    auto data = ui->AccountDisp->getAccountForDBUpdate();
    if(data._Myfirst._Val == NULL)
    {
        return;
    }
    DBController::GetInstance()->accs.Update(data._Myfirst._Val);

    if(data._Get_rest()._Myfirst._Val != NULL)
    {
        DBController::GetInstance()->infos.Delete(data._Get_rest()._Myfirst._Val->_id);
    }

    if(data._Get_rest()._Get_rest()._Myfirst._Val != NULL)
    {
        auto Info = data._Get_rest()._Get_rest()._Myfirst._Val;
        Info->setOwner(data._Myfirst._Val);
        DBController::GetInstance()->infos.Add(data._Get_rest()._Get_rest()._Myfirst._Val, data._Myfirst._Val->AccountType);
    }
    ui->tableWidget->UpdateTable();
}

void AdminWindow::AddNewGroup()
{
    Group* grp = new Group(ui->GroupNameEdit->text().toStdString());
    if(ui->CuratorComboBox->currentIndex() != 0)
    {
        auto iter = allCurators.begin();
        for(int i = ui->CuratorComboBox->currentIndex();i > 1;i--) {iter++;}
        group->setCurator(*iter);
    }
    UpdateGroupsScrean(DBController::GetInstance()->grps.Add(grp));
}

void AdminWindow::DeleteGroup()
{
    if(group != NULL)
    {
        DBController::GetInstance()->grps.Delete(group->_id);
    }
    group = NULL;
    UpdateGroupsScrean(NULL);
}

void AdminWindow::UpdateGroup()
{
    if(group != NULL)
    {
        group->Name = ui->GroupNameEdit->text().toStdString();
        if(ui->CuratorComboBox->currentIndex() != 0)
        {
            auto iter = allCurators.begin();
            for(int i = ui->CuratorComboBox->currentIndex();i > 1;i--) {iter++;}
            group->setCurator(*iter);
        }
        else group->setCurator(NULL);
    }
    UpdateGroupsScrean(group);
}

void AdminWindow::AddClass()
{
    if(teacher == NULL)
    {
        return;
    }

    if(ui->ClassEdit->text().toStdString().empty())
    {
        return;
    }

    teacher->lessonsTypes.push_back(ui->ClassEdit->text().toStdString());
    DBController::GetInstance()->infos.Update(teacher);
    ui->ClassesTable->UpdateTable();
}

void AdminWindow::DeleteClass()
{
    if(teacher == NULL)
    {
        return;
    }

    if(ui->ClassEdit->text().toStdString().empty())
    {
        return;
    }

    if(teacher->lessonsTypes.empty())
    {
        return;
    }

    auto iterToDelete = teacher->lessonsTypes.begin();
    for(auto iter = teacher->lessonsTypes.begin(); iter != teacher->lessonsTypes.end(); iter++)
    {
        if((*iter).compare(ui->ClassEdit->text().toStdString()) == 0)
        {
            iterToDelete = iter;
        }
    }
    teacher->lessonsTypes.erase(iterToDelete);
    DBController::GetInstance()->infos.Update(teacher);
    ui->ClassesTable->UpdateTable();
}

void AdminWindow::AddAccToGroup(AccountDB *acc)
{
    if(group == NULL)
    {
        return;
    }

    for(auto stud : group->getStudents())
    {
        if(stud->getOwner()->_id == acc->_id)
        {
            return;
        }
    }

    group->addStudent(dynamic_cast<StudentInfoDB*>(acc->getAdditionalInfo()));
    UpdateGroupsScrean();
}

void AdminWindow::DeleteAccFromGroup(AccountDB *acc)
{
    if(group != NULL)
    {
        group->deleteStudent(dynamic_cast<StudentInfoDB*>(acc->getAdditionalInfo()));
    }
    UpdateGroupsScrean();
}

void AdminWindow::AddSchedule(std::string str, int RowIndex)
{
    if(schGroupDB == NULL || !schGroupDB->isExist)
    {
        return;
    }
    if(teacher == NULL || !teacher->isExist)
    {
        return;
    }
    auto write = new  Schedule(ui->Calendar->selectedDate(), str, static_cast<Schedule::Para>(RowIndex));
    write->setGroup(schGroupDB);
    write->setTeacher(teacher);
    DBController::GetInstance()->sched.Add(write);
    UpdateScheduleScrean();
}

void AdminWindow::DeleteSchedule(ScheduleDB *acc)
{
    if(acc == NULL)
    {
        return;
    }
    DBController::GetInstance()->sched.Delete(acc->_id);
    UpdateScheduleScrean();
}

void AdminWindow::UpdateGroupsScrean(GroupDB* grp)
{
    SetGroup(grp);
    ui->GroupedAccs->UpdateTable();
    ui->NotAssignedAccs->UpdateTable();
    ui->GroupsTable->UpdateTable();
}

void AdminWindow::DataChanged(QDate date)
{
    this->date = date;
    UpdateScheduleScrean();
}

void AdminWindow::UpdateScheduleScrean(AccountDB *teacher, GroupDB *grp)
{
    if(grp != NULL)
    {
        schGroupDB = grp;
    }
    if(teacher != NULL)
    {
        SetAccToSchedule(teacher);
    }
    ui->TaeachersTable->UpdateTable();
    ui->GroupsTableSch->UpdateTable();
    ui->ClassesTable->UpdateTable();
    if(schGroupDB != NULL)
    {
        ui->SchedulesTable->setSchedules(DBController::GetInstance()->sched.GetAllSchedulesinDate(date.toJulianDay(), schGroupDB));
    }
}


