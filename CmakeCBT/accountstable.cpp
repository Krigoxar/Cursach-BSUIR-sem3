#include "accountstable.h"

AccountsTable::AccountsTable(DBController::AccountsController* controller) : QTableWidget(0, 3)
{
    Controller = controller;
    QStringList list = QStringList();
    list.push_back("ID");
    list.push_back("First name");
    list.push_back("Second name");
    this->setHorizontalHeaderLabels(list);
}
