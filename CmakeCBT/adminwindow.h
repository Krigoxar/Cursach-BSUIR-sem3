#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include "mytable.h"
//#include "model.h"
//#include "dbcontroller.h"

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT
    GroupDB* group = NULL;
    TeacherInfoDB* teacher = NULL;
    GroupDB* schGroupDB = NULL;
    QDate date = QDate::currentDate().addDays(1);
    std::list<TeacherInfoDB*> allCurators = std::list<TeacherInfoDB*>();
    int initialWidth = 0;
    void UpdateGroupsScrean(GroupDB* grp = NULL);
    void UpdateScheduleScrean(AccountDB* teacher = NULL, GroupDB* grp = NULL);
public:
    explicit AdminWindow(QWidget *parent = nullptr);

    void resizeEvent(QResizeEvent *event) override;


    ~AdminWindow();
public slots:
    void ChangeCurentSize(int tab);
    void SetAccountToDisplayWidget(AccountDB* acc);
    void SetGroup(GroupDB* acc);
    void SetAccToSchedule(AccountDB* acc);

    void AddNewAccount();
    void DeleteAccount();
    void UpdateAccount();

    void AddNewGroup();
    void DeleteGroup();
    void UpdateGroup();

    void AddClass();
    void DeleteClass();

    void DataChanged(QDate date);
    void SetGroupSch(GroupDB* acc);

    void AddAccToGroup(AccountDB* acc);
    void DeleteAccFromGroup(AccountDB* acc);

    void AddSchedule(std::string str, int RowIndex);
    void DeleteSchedule(ScheduleDB* acc);

private:
    Ui::AdminWindow *ui;
};

#endif // ADMINWINDOW_H
