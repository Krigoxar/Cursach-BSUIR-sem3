#ifndef MYTABLE_H
#define MYTABLE_H
#define SchedulesArrayLenght 8
#include <QTableWidget>
#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include "dbcontroller.h"
#include "errors.h"

class ImyQTableWidget
{
    virtual void UpdateTable() = NULL;
};

class AccountsTable : public QTableWidget, ImyQTableWidget
{
     Q_OBJECT
    std::list<AccountDB*> accs;
    virtual std::list<AccountDB*> getAccounts();
public:
    AccountsTable(QWidget *parent = NULL);
    void UpdateTable();
public slots:
    void ActiveRowChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
signals:
    void AccountDBChanged(AccountDB* acc);
};

class PayloadedItem : public QTableWidgetItem
{
public:
    void* Payload;
    PayloadedItem();
    PayloadedItem(std::string str);
};

class UngroupedAccountsTable : public AccountsTable
{
    Q_OBJECT
    virtual std::list<AccountDB*> getAccounts() override;
protected:
    virtual void dropEvent(QDropEvent *event) override;
public:
    UngroupedAccountsTable(QWidget *parent = 0);
signals:
    void DelAcc(AccountDB* acc);
};

class GroupedAccountsTable : public AccountsTable
{
    Q_OBJECT
    GroupDB* group = NULL;
    virtual std::list<AccountDB*> getAccounts() override;
protected:
    virtual void dropEvent(QDropEvent *event) override;
public:
    void setGroup(GroupDB* grp);
    GroupedAccountsTable(QWidget *parent = 0);
signals:
    void AddAcc(AccountDB* acc);
};

class TeachersTable : public AccountsTable
{
    Q_OBJECT
    virtual std::list<AccountDB*> getAccounts() override;
public:
    TeachersTable(QWidget *parent = 0);

};

class GroupsDBTable : public QTableWidget, ImyQTableWidget
{
    Q_OBJECT
    DBController::GroupsController* grpController;
    std::list<GroupDB*> grps;
    std::list<GroupDB*> getGroups();
public:
    GroupsDBTable(QWidget* parent = 0);
    void UpdateTable();
public slots:
    void ActiveRowChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
signals:
    void AccountDBChanged(GroupDB* acc);

};

class PosibleLessonsTable : public QTableWidget, ImyQTableWidget
{
    Q_OBJECT
    TeacherInfoDB* teacher = NULL;
    std::list<std::string> getLessonsTypes();
public:
    void UpdateTable();
    virtual void dropEvent(QDropEvent *event) override;
    PosibleLessonsTable(QWidget* parent = NULL);
    void setTeacher(TeacherInfoDB* teach);
signals:
    void DeleteSchedule(ScheduleDB* acc);
};

class ScheduleTable : public QTableWidget, ImyQTableWidget
{
    Q_OBJECT
    ScheduleDB* schedules[SchedulesArrayLenght];
public:
    void setSchedules(std::list<ScheduleDB*> scheds);
    void UpdateTable();
    ScheduleTable(QWidget* parent = NULL);
    virtual void dropEvent(QDropEvent *event) override;
signals:
    void AddSchedule(std::string str, int RowIndex);
};

#endif // MYTABLE_H
