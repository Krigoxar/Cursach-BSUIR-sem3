#ifndef DISPLAYWIDGETS_H
#define DISPLAYWIDGETS_H

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include "model.h"
#include "dbcontroller.h"

class DisplayAccountWidget : public QWidget
{
    QFormLayout* layout = new QFormLayout(this);
    AccountDB* Acc;
    AccountType Type;

    QLineEdit* editLogin = new QLineEdit();
    QLineEdit* editPassword = new QLineEdit();
    QLineEdit* editFirstName;
    QLineEdit* editSecondName;
    QComboBox* editType = new QComboBox();
    QLineEdit* editCurs;
    QComboBox* editGroup;
    QLineEdit* editFaculty;

    void setType(AccountType type);
    void setLayoutAcc();

    GroupDB* getGroupFromEdit();
public:
    void setAccountDB(AccountDB* acc);
    std::tuple<Account*, AdditionalInfo*> getNewAccount();
    std::tuple<AccountDB*, AdditionalInfoDB*, AdditionalInfo*> getAccountForDBUpdate();
    AccountDB* getAccountForDelete();
    DisplayAccountWidget(QWidget* parent = 0);
public slots:
    void OnCurrentIndexChanged(int index);

};

#endif // DISPLAYWIDGETS_H
