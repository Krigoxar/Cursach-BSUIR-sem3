#include "displaywidgets.h"

void DisplayAccountWidget::setType(AccountType type)
{
    Type = type;
    setLayoutAcc();
}

void DisplayAccountWidget::setLayoutAcc()
{

    while(layout->rowCount() > 3)
    {
        layout->removeRow(3);
    }

    AdditionalInfoDB* addInf;

    if(Acc == NULL)
    {
        addInf = NULL;
    }
    else
    {
        addInf = Acc->getAdditionalInfo();
        editLogin->setText(Acc->Login.c_str());
        editPassword->setText(Acc->Password.c_str());
    }


    if(Type == AccountType::Student)
    {
        editFirstName = new QLineEdit();
        editSecondName = new QLineEdit();
        editCurs = new QLineEdit();
        editGroup = new QComboBox();
        auto grps = DBController::GetInstance()->grps.GetAll();
        editGroup->addItem("None");
        for(auto grp : grps)
        {
            editGroup->addItem(QString(grp->Name.c_str()));
        }
        editGroup->setCurrentText("None");

        layout->addRow("First name", editFirstName);
        layout->addRow("Second name", editSecondName);
        layout->addRow("Curs", editCurs);
        layout->addRow("Group", editGroup);

        if(addInf == NULL)
        {
            return;
        }
        StudentInfoDB* studInf = dynamic_cast<StudentInfoDB*>(addInf);
        if(Type == Acc->AccountType)
        {
            editFirstName->setText(studInf->FirstName.c_str());
            editSecondName->setText(studInf->SecondName.c_str());
            editCurs->setText(std::to_string(studInf->Curs).c_str());
            if(studInf->getGroup() != NULL)
            {
                editGroup->setCurrentText(studInf->getGroup()->Name.c_str());
            }
        }
    }
    else if(Type == AccountType::Teacher)
    {
        editFirstName = new QLineEdit();
        editSecondName = new QLineEdit();
        editFaculty = new QLineEdit();

        layout->addRow("First name", editFirstName);
        layout->addRow("Second name", editSecondName);
        layout->addRow("Faculty", editFaculty);

        if(addInf == NULL)
        {
            return;
        }
        TeacherInfoDB* teachInf = dynamic_cast<TeacherInfoDB*>(addInf);
        if(Type == Acc->AccountType)
        {
            editFirstName->setText(teachInf->FirstName.c_str());
            editSecondName->setText(teachInf->SecondName.c_str());
            editFaculty->setText(teachInf->Faculty.c_str());
        }

    }
}

GroupDB* DisplayAccountWidget::getGroupFromEdit()
{
    auto Grps = DBController::GetInstance()->grps.GetAll();
    foreach (auto grp, Grps) {
        if(grp->Name == editGroup->currentText().toStdString())
        {
            return grp;
        }
    }
    return NULL;
}

void DisplayAccountWidget::OnCurrentIndexChanged(int index)
{
    setType((AccountType)index);
}

void DisplayAccountWidget::setAccountDB(AccountDB* acc)
{
    Acc = acc;
    if(Acc == NULL)
    {
        editType->setCurrentIndex(AccountType::Admin);
        setType(AccountType::Admin);
    }
    else
    {
        editType->setCurrentIndex(Acc->AccountType);
        setType(acc->AccountType);
    }
}

std::tuple<Account*, AdditionalInfo*> DisplayAccountWidget::getNewAccount()
{
    Account* resAcc = new Account(editLogin->text().toStdString(), editPassword->text().toStdString(), ((AccountType)editType->currentIndex()));
    AdditionalInfo* resAddInf;

    StudentInfo* studInf;
    TeacherInfo* teachInf;
    switch (resAcc->AccountType)
    {
    case AccountType::Student:
        studInf = new StudentInfo(editFirstName->text().toStdString(), editSecondName->text().toStdString(), editCurs->text().toInt());
        if(getGroupFromEdit() != NULL)
        {
            studInf->setGroup(getGroupFromEdit());
        }
        resAddInf = studInf;
        break;
    case AccountType::Teacher:
        teachInf = new TeacherInfo(editFirstName->text().toStdString(), editSecondName->text().toStdString(), editFaculty->text().toStdString());
        resAddInf = teachInf;
        break;
    default:
        resAddInf = NULL;
        break;
    }

    return std::tuple<Account*, AdditionalInfo*>(resAcc, resAddInf);
}

std::tuple<AccountDB*, AdditionalInfoDB*, AdditionalInfo*> DisplayAccountWidget::getAccountForDBUpdate()
{
    AccountDB* resAcc = Acc;
    resAcc->Login = editLogin->text().toStdString();
    resAcc->Password = editPassword->text().toStdString();
    resAcc->AccountType = ((AccountType)editType->currentIndex());

    AdditionalInfoDB* prevAddInf = resAcc->getAdditionalInfo();

    AdditionalInfo* newAddInf = NULL;

    StudentInfo* studInf;
    TeacherInfo* teachInf;
    switch (resAcc->AccountType)
    {
    case AccountType::Student:
        studInf = new StudentInfo(editFirstName->text().toStdString(),
                                  editSecondName->text().toStdString(),
                                  editCurs->text().toInt());
        studInf->setGroup(getGroupFromEdit());
        newAddInf = studInf;
        break;
    case AccountType::Teacher:
        teachInf = new TeacherInfo(editFirstName->text().toStdString(),
                                   editSecondName->text().toStdString(),
                                   editFaculty->text().toStdString());
        newAddInf = teachInf;
        break;
    case AccountType::Admin:
        break;
    }

    return std::tuple<AccountDB*, AdditionalInfoDB*, AdditionalInfo*>(resAcc, prevAddInf, newAddInf);
}

AccountDB* DisplayAccountWidget::getAccountForDelete()
{
    return Acc;
}

DisplayAccountWidget::DisplayAccountWidget(QWidget* parent) : QWidget(parent)
{
    editType->addItem("Admin");
    editType->addItem("Teacher");
    editType->addItem("Student");
    this->connect(editType, &QComboBox::currentIndexChanged,
                  this,     &DisplayAccountWidget::OnCurrentIndexChanged);

    Acc = NULL;
    layout->addRow("Login", editLogin);
    layout->addRow("Password", editPassword);
    layout->addRow("Type", editType);
}
