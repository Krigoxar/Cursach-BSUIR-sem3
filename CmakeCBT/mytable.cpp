#include "mytable.h"
#include <QtDebug>

std::list<AccountDB*> AccountsTable::getAccounts()
{
    return DBController::GetInstance()->accs.GetAll();
}

AccountsTable::AccountsTable(QWidget *parent) : QTableWidget(parent)
{
    connect(this, &AccountsTable::currentCellChanged,
            this, &AccountsTable::ActiveRowChanged);
}

void AccountsTable::UpdateTable()
{
    //DBController* inst = DBController::GetInstance();
    //inst->Test();

    accs = getAccounts();
    this->setRowCount((int)accs.size());
    int r = 0;
    for(auto acc : accs)
    {
        if(acc == NULL)
        {
            continue;
        }

        PayloadedItem* item;
        AdditionalInfoDB* info = acc->getAdditionalInfo();

        if(acc->AccountType == AccountType::Student)
        {
            StudentInfoDB* infoStud = dynamic_cast<StudentInfoDB*>(info);
            item = new PayloadedItem(infoStud->FirstName);
            item->Payload = acc;
            this->setItem(r, 0, item);
            item = new PayloadedItem(infoStud->SecondName);
            this->setItem(r, 1, item);
        }
        else if(acc->AccountType == AccountType::Teacher)
        {
            TeacherInfoDB* infoTeach = dynamic_cast<TeacherInfoDB*>(info);
            item = new PayloadedItem(infoTeach->FirstName);
            item->Payload = acc;
            this->setItem(r, 0, item);
            item = new PayloadedItem(infoTeach->SecondName);
            this->setItem(r, 1, item);
        }
        else if(acc->AccountType == AccountType::Admin)
        {
            item = new PayloadedItem("ADMIN");
            item->Payload = acc;
            this->setItem(r, 0, item);
            item = new PayloadedItem(acc->_id.to_string());
            this->setItem(r, 1, item);
        }
        r++;
    }
}

void AccountsTable::ActiveRowChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    auto res = this->item(currentRow, 0);
    //res->data(0);
    if(res != NULL)
    {
        emit AccountDBChanged((AccountDB*)dynamic_cast<PayloadedItem*>(res)->Payload);
    }
}

std::list<AccountDB*> UngroupedAccountsTable::getAccounts()
{
    auto accs = DBController::GetInstance()->accs.GetAll();
    std::list<AccountDB*> res = std::list<AccountDB*>();
    for(auto acc: accs)
    {
        StudentInfoDB* stud;
        if((stud = dynamic_cast<StudentInfoDB*>(acc->getAdditionalInfo())) != NULL)
        {
            if(stud->getGroup() == NULL)
            {
                res.push_back(acc);
            }
        }
    }
    return res;
}

void ScheduleTable::UpdateTable()
{
    for (int i = 0; i < SchedulesArrayLenght; ++i)
    {
        if(schedules[i] == NULL)
        {
            PayloadedItem* item = new PayloadedItem("");
            item->Payload = NULL;
            this->setItem(i, 1, item);
        }
        else
        {
            PayloadedItem* item = new PayloadedItem(schedules[i]->Name);
            item->Payload = schedules[i];
            this->setItem(i, 1, item);
        }
    }
    this->setItem(0, 0, new QTableWidgetItem("9.00 - 10.20"));
    this->setItem(1, 0, new QTableWidgetItem("10.35 - 11.55"));
    this->setItem(2, 0, new QTableWidgetItem("12.25 - 13.45"));
    this->setItem(3, 0, new QTableWidgetItem("14.00 - 15.20"));
    this->setItem(4, 0, new QTableWidgetItem("15.50 - 17.10"));
    this->setItem(5, 0, new QTableWidgetItem("17.25 - 18.45"));
    this->setItem(6, 0, new QTableWidgetItem("19.00 - 20.20"));
    this->setItem(7, 0, new QTableWidgetItem("20.40 - 22.00"));
}

std::list<AccountDB *> TeachersTable::getAccounts()
{
    return DBController::GetInstance()->accs.GetAllTeachers();
}

std::list<AccountDB*> GroupedAccountsTable::getAccounts()
{
    std::list<AccountDB*> res = std::list<AccountDB*>();
    if(group == NULL)
    {
        return res;
    }
    if(group->isExist == false)
    {
        return res;
    }
    auto infos = group->getStudents();
    for(auto info: infos)
    {
        res.push_back(info->getOwner());
    }
    return res;
}

void GroupedAccountsTable::dropEvent(QDropEvent *event)
{
    auto src = dynamic_cast<QTableWidget*>(event->source());
    emit AddAcc((AccountDB*)dynamic_cast<PayloadedItem*>(src->item(src->currentRow(), 0))->Payload);
}

void UngroupedAccountsTable::dropEvent(QDropEvent *event)
{
    auto src = dynamic_cast<QTableWidget*>(event->source());
    emit DelAcc((AccountDB*)dynamic_cast<PayloadedItem*>(src->item(src->currentRow(), 0))->Payload);
}

void GroupedAccountsTable::setGroup(GroupDB* grp)
{
    group = grp;
    this->UpdateTable();
}

UngroupedAccountsTable::UngroupedAccountsTable(QWidget *parent) : AccountsTable(parent)
{
    setAcceptDrops(true);
}

GroupedAccountsTable::GroupedAccountsTable(QWidget *parent) : AccountsTable(parent)
{
    setAcceptDrops(true);
}

TeachersTable::TeachersTable(QWidget *parent): AccountsTable(parent)
{
    TeachersTable::connect(this, &TeachersTable::currentCellChanged,
                           this, &TeachersTable::ActiveRowChanged);
}

std::list<std::string> PosibleLessonsTable::getLessonsTypes()
{
    if(teacher == NULL)
    {
        return std::list<std::string>();
    }
    else
    {
        return teacher->lessonsTypes;
    }
}

PosibleLessonsTable::PosibleLessonsTable(QWidget *parent) : QTableWidget(parent) {}

ScheduleTable::ScheduleTable(QWidget *parent) : QTableWidget(parent)
{
    this->setAcceptDrops(true);
    this->setRowCount(8);
    for (int i = 0; i < SchedulesArrayLenght; ++i)
    {
        schedules[i] = NULL;
    }
}

void ScheduleTable::dropEvent(QDropEvent *event)
{
    auto row = this->rowAt(event->position().y());
    auto src = dynamic_cast<QTableWidget*>(event->source());
    emit AddSchedule(dynamic_cast<PayloadedItem*>(src->item(src->currentRow(), 0))->text().toStdString(), row);
}

std::list<GroupDB *> GroupsDBTable::getGroups()
{
    return grpController->GetAll();
}

GroupsDBTable::GroupsDBTable(QWidget *parent)
{
    grpController = &DBController::GetInstance()->grps;
    GroupsDBTable::connect(this, &GroupsDBTable::currentCellChanged,
                           this, &GroupsDBTable::ActiveRowChanged);
}

void GroupsDBTable::UpdateTable()
{
    grps = getGroups();
    this->setRowCount((int)grps.size());
    int r = 0;
    for(auto grp : grps)
    {
        PayloadedItem* item = new PayloadedItem(grp->Name);
        item->Payload = grp;
        this->setItem(r, 0, item);
        r++;
    }
}

void PosibleLessonsTable::setTeacher(TeacherInfoDB *teach)
{
    teacher = teach;
    UpdateTable();
}

void PosibleLessonsTable::UpdateTable()
{
    std::list<std::string> Lessons = getLessonsTypes();
    this->setRowCount((int)Lessons.size());
    int r = 0;
    for(auto lesson : Lessons)
    {
        PayloadedItem* item = new PayloadedItem(lesson);
        item->Payload = NULL;
        this->setItem(r, 0, item);
        r++;
    }
}

void PosibleLessonsTable::dropEvent(QDropEvent *event)
{
    auto src = dynamic_cast<QTableWidget*>(event->source());
    PayloadedItem* item = dynamic_cast<PayloadedItem*>(src->item(src->currentRow(), 1));
    emit DeleteSchedule((ScheduleDB*)item->Payload);
}

void GroupsDBTable::ActiveRowChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(currentRow >= 0)
    {
        auto res = this->item(currentRow, 0);
        emit AccountDBChanged((GroupDB*)(dynamic_cast<PayloadedItem*>(res)->Payload));
    }
}

PayloadedItem::PayloadedItem() : QTableWidgetItem(1001) {}
PayloadedItem::PayloadedItem(std::string str) : QTableWidgetItem(str.c_str(), 1001) {}

void ScheduleTable::setSchedules(std::list<ScheduleDB*> scheds)
{
    for (int i = 0; i < SchedulesArrayLenght; ++i)
    {
        schedules[i] = NULL;
    }
    for(auto sched : scheds)
    {
        schedules[sched->ParaNumber] = sched;
    }
    UpdateTable();
}

