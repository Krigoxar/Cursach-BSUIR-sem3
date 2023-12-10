#include "model.h"
#include "dbcontroller.h"

std::string myto_string(enum AccountType val){
    switch (val) {
    case AccountType::Admin:
        return std::string("Admin");
        break;
    case AccountType::Teacher:
        return std::string("Teacher");
        break;
    case AccountType::Student:
        return std::string("Student");
        break;
    default:
        return std::string("");
        break;
    }
}


document::view Group::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    Group::Build(builder);
    return builder->view();
}

document::view AdditionalInfo::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    AdditionalInfo::Build(builder);
    return builder->view();
}

document::view StudentInfo::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    AdditionalInfo::Build(builder);
    StudentInfo::Build(builder);
    return builder->view();
}

document::view TeacherInfo::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    AdditionalInfo::Build(builder);
    TeacherInfo::Build(builder);
    return builder->view();
}

document::view GroupDB::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    IFromDB::Build(builder);
    Group::Build(builder);
    return builder->view();
}

document::view ScheduleDB::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    IFromDB::Build(builder);
    Schedule::Build(builder);
    return builder->view();
}

document::view StudentInfoDB::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    IFromDB::Build(builder);
    AdditionalInfo::Build(builder);
    StudentInfo::Build(builder);
    return builder->view();
}

document::view TeacherInfoDB::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    IFromDB::Build(builder);
    AdditionalInfo::Build(builder);
    TeacherInfo::Build(builder);
    return builder->view();
}

document::view Schedule::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    Schedule::Build(builder);
    return builder->view();
}

document::view LogPass::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    LogPass::Build(builder);
    return builder->view();
}

document::view Account::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    LogPass::Build(builder);
    Account::Build(builder);
    return builder->view();
}

document::view AccountDB::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    IFromDB::Build(builder);
    LogPass::Build(builder);
    Account::Build(builder);
    return builder->view();
}

builder::stream::document *Schedule::Build(builder::stream::document *builder)
{
    *builder << "Name" << Name
             << "Date" << Date.toJulianDay()
             << "Para" << (int)ParaNumber;
    if(Teacher == NULL || !Teacher->isExist)
    {
        *builder << "Teacher" << NULL;
    }
    else
    {
        *builder << "Teacher" << Teacher->_id;
    }

    if(Group == NULL || !Group->isExist)
    {
        *builder << "Group" << NULL;
    }
    else
    {
        *builder << "Group" << Group->_id;
    }
    return builder;
}

builder::stream::document *AdditionalInfo::Build(builder::stream::document *builder)
{

    *builder << "FirstName" << FirstName
             << "SecondName" << SecondName;
    if(Owner == NULL || !Owner->isExist)
    {
        *builder << "Owner" << NULL;
    }
    else
    {
        *builder << "Owner" << Owner->_id;
    }
    return builder;
}


builder::stream::document* IFromDB::Build(builder::stream::document* builder)
{
    *builder << "_id" << _id;
    return builder;
}
builder::stream::document* LogPass::Build(builder::stream::document* builder)
{
    *builder << "Login" << Login
            << "Password" << Password;
    return builder;
}

builder::stream::document* Account::Build(builder::stream::document* builder)
{
    *builder << "AccountType" << AccountType;
    if(AdditionalInfo == NULL || !AdditionalInfo->isExist)
    {
        *builder << "AdditionalInfo" << NULL;
    }
    else
    {
        *builder << "AdditionalInfo" << AdditionalInfo->_id;
    }
    return builder;
}

builder::stream::document* TeacherInfo::Build(builder::stream::document* builder)
{
    *builder << "Faculty" << Faculty;
    builder::stream::array arr;
    for(auto lessonType : lessonsTypes)
    {
        arr << lessonType;
    }
    *builder << "LessonsTypes" << arr;
    return builder;
}

builder::stream::document* StudentInfo::Build(builder::stream::document* builder)
{
    *builder << "Curs" << Curs;
    if(Group == NULL || !Group->isExist)
    {
        *builder << "Group" << NULL;
    }
    else
    {
        *builder << "Group" << Group->_id;
    }
    return builder;
}

builder::stream::document* Group::Build(builder::stream::document* builder)
{
    *builder << "Name" << Name;
    if(Curator == NULL || !Curator->isExist)
    {
        *builder << "Curator" << NULL;
    }
    else
    {
        *builder << "Curator" << Curator->_id;
    }
    if(Students.empty())
    {
        *builder << "Members" << NULL;
    }
    else
    {
        builder::stream::array arr;
        for(auto iter = Students.begin(); iter != Students.end(); iter++)
        {
            if((*iter)->isExist)
            {
                arr << (*iter)->_id;
            }
        }
        *builder << "Members" << arr;
    }

    return builder;
}

builder::stream::document *AdditionalInfoDB::Build(builder::stream::document *builder)
{
    return builder;
}

document::view AdditionalInfoDB::toDoc()
{
    builder::stream::document* builder = new builder::stream::document();
    IFromDB::Build(builder);
    AdditionalInfoDB::Build(builder);
    return builder->view();
}

LogPass::LogPass(std::string login, std::string password) : Login(login), Password(password) {}

Account::Account(std::string login, std::string password, enum AccountType accountType) : LogPass(login, password)
{
    AccountType = accountType;
}

Group::Group(std::string name)
{
    Name = name;
}

void Account::setAdditionalInfo(AdditionalInfoDB *adInf)
{
    AdditionalInfo = adInf;
    if(AdditionalInfo != NULL)
    {
        AdditionalInfo->OwnedCount++;
    }
}


void Group::setCurator(TeacherInfoDB *teacher)
{
    Curator = teacher;
    if(Curator != NULL)
    {
        Curator->OwnedCount++;
    }
}

void Group::setStudents(std::list<StudentInfoDB *> students)
{
    Students = students;
    for(auto stud: students)
    {
        stud->OwnedCount++;
    }
}

void StudentInfo::setGroup(GroupDB *group)
{
    Group = group;
    if(group != NULL)
    {
        Group->OwnedCount++;
    }
}

void AdditionalInfo::setOwner(AccountDB *acc)
{
    Owner = acc;
    if(Owner != NULL)
    {
        Owner->OwnedCount++;
    }
}

StudentInfo::StudentInfo(std::string firstName, std::string secondName, int curs)
{
    FirstName = firstName;
    SecondName = secondName;
    Curs = curs;
}

TeacherInfo::TeacherInfo(std::string firstName, std::string secondName, std::string faculty)
{
    FirstName = firstName;
    SecondName = secondName;
    Faculty = faculty;
}

IFromDB::IFromDB(document::value fromDoc) :
    doc(fromDoc),
    _id(fromDoc["_id"].get_oid().value) {}


bool GroupDB::addStudent(StudentInfoDB *stud)
{
    Students.push_back(stud);
    DBController::GetInstance()->grps.Update(this);
    stud->setGroup(this);
    DBController::GetInstance()->infos.Update((AdditionalInfoDB*)stud);
    stud->OwnedCount++;
    return true;
}

bool GroupDB::deleteStudent(StudentInfoDB *stud)
{
    for (auto var = Students.begin(); var != Students.end(); ++var)
    {
        if((*var)->_id == stud->_id)
        {
            Students.erase(var);
            DBController::GetInstance()->grps.Update(this);
            stud->setGroup(NULL);
            DBController::GetInstance()->infos.Update(stud);
            stud->OwnedCount--;
            return true;
        }
    }
    return false;
}


std::list<StudentInfoDB *> GroupDB::getStudents()
{
    Students.clear();
    if(doc["Members"].type() == bsoncxx::type::k_array)
    {
        for (auto studId : doc["Members"].get_array().value)
        {
            auto res = dynamic_cast<StudentInfoDB*>(DBController::GetInstance()->infos.GetFromDB(studId.get_value().get_oid().value, AccountType::Student));
            if(res == NULL)
            {
                continue;
            }
            if(res->isExist)
            {
                Students.push_back(res);
            }
        }
    }

    return Students;//todo;
}

TeacherInfoDB* GroupDB::getCurator()
{
    if(Curator == NULL)
    {
        auto DB = DBController::GetInstance();
        if(doc["Curator"].type() != bsoncxx::type::k_oid)
        {
            return NULL;
        }
        Curator = dynamic_cast<TeacherInfoDB*>(DB->infos.GetFromDB(doc["Curator"].get_oid().value, AccountType::Teacher));
        if(Curator == NULL)
        {
            return Curator;
        }
    }

    if(Curator->isExist)
    {
        return Curator;
    }
    else
    {
        Curator->OwnedCount--;
        if(Curator->OwnedCount == 0)
        {
            delete Curator;
        }
        Curator = NULL;
    }
    return Curator;
}

AccountDB *TeacherInfoDB::getOwner()
{
    if(Owner == NULL)
    {
        auto DB = DBController::GetInstance();
        if(doc["Owner"].type() != bsoncxx::type::k_oid)
        {
            return NULL;
        }
        Owner = DB->accs.GetFromDB(doc["Owner"].get_oid().value);
        if(Owner == NULL)
        {
            return Owner;
        }
    }

    if(Owner->isExist)
    {
        return Owner;
    }
    else
    {
        Owner->OwnedCount--;
        if(Owner->OwnedCount == 0)
        {
            delete Owner;
        }
        Owner = NULL;
    }
    return Owner;
}

AccountDB *StudentInfoDB::getOwner()
{
    if(Owner == NULL)
    {
        auto DB = DBController::GetInstance();
        if(doc["Owner"].type() != bsoncxx::type::k_oid)
        {
            return NULL;
        }
        Owner = DB->accs.GetFromDB(doc["Owner"].get_oid().value);
        if(Owner == NULL)
        {
            return Owner;
        }
    }

    if(Owner->isExist)
    {
        return Owner;
    }
    else
    {
        Owner->OwnedCount--;
        if(Owner->OwnedCount == 0)
        {
            delete Owner;
        }
        Owner = NULL;
    }
    return Owner;
}

GroupDB *StudentInfoDB::getGroup()
{
    if(Group == NULL)
    {
        auto DB = DBController::GetInstance();
        if(doc["Group"].type() != bsoncxx::type::k_oid)
        {
            return NULL;
        }
        Group = DB->grps.GetFromDB(doc["Group"].get_oid().value);
        if(Group == NULL)
        {
            return Group;
        }
    }

    if(Group->isExist)
    {
        return Group;
    }
    else
    {
        Group->OwnedCount--;
        if(Group->OwnedCount == 0)
        {
            delete Group;
        }
        Group = NULL;
    }
    return Group;
}

GroupDB* ScheduleDB::getGroup()
{
    if(Group == NULL)
    {
        auto DB = DBController::GetInstance();
        if(doc["Group"].type() != bsoncxx::type::k_oid)
        {
            return NULL;
        }
        Group = DB->grps.GetFromDB(doc["Group"].get_oid().value);
        if(Group == NULL)
        {
            return Group;
        }
    }

    if(Group->isExist)
    {
        return Group;
    }
    else
    {
        Group->OwnedCount--;
        if(Group->OwnedCount == 0)
        {
            delete Group;
        }
        Group = NULL;
    }
    return Group;
}

AdditionalInfoDB* AccountDB::getAdditionalInfo()
{
    if(this->AdditionalInfo == NULL)
    {
        auto el = doc["AdditionalInfo"];
        if(el.type() != bsoncxx::type::k_oid)
        {
            return NULL;
        }

        auto DB = DBController::GetInstance();
        AdditionalInfo = DB->infos.GetFromDB(el.get_oid().value, AccountType);
        if(AdditionalInfo == NULL)
        {
            return AdditionalInfo;
        }
    }

    if(AdditionalInfo->isExist)
    {
        return AdditionalInfo;
    }
    else
    {
        AdditionalInfo->OwnedCount--;
        if(AdditionalInfo->OwnedCount == 0)
        {
            delete AdditionalInfo;
        }
        AdditionalInfo = NULL;
    }
    return AdditionalInfo;
}

TeacherInfoDB *ScheduleDB::getTeacher()
{    if(this->Teacher == NULL)
    {
        auto el = doc["AdditionalInfo"];
        if(el.type() != bsoncxx::type::k_oid)
        {
            return NULL;
        }

        auto DB = DBController::GetInstance();
        Teacher = dynamic_cast<TeacherInfoDB*>(DB->infos.GetFromDB(el.get_oid().value, AccountType::Teacher));
        if(Teacher == NULL)
        {
            return Teacher;
        }
    }

    if(Teacher->isExist)
    {
        return Teacher;
    }
    else
    {
        Teacher->OwnedCount--;
        if(Teacher->OwnedCount == 0)
        {
            delete Teacher;
        }
        Teacher = NULL;
    }
    return Teacher;
}

ScheduleDB::ScheduleDB(document::value fromDoc) :
    Schedule(QDate::fromJulianDay(fromDoc["Date"].get_int64()),
            (std::string)fromDoc["Name"].get_string(),
             static_cast<enum Para>((int)fromDoc["Para"].get_int32())),
    IFromDB(fromDoc) {}

GroupDB::GroupDB(document::value fromDoc) :
    Group((std::string)fromDoc["Name"].get_string()),
    IFromDB(fromDoc) {}

AccountDB::AccountDB(document::value fromDoc) :
    Account((std::string)fromDoc["Login"].get_string(),
            (std::string)fromDoc["Password"].get_string(),
              static_cast<enum AccountType>((int)fromDoc["AccountType"].get_int32())),
    IFromDB(fromDoc) {}

TeacherInfoDB::TeacherInfoDB(document::value fromDoc) :
    AdditionalInfoDB(fromDoc),
    TeacherInfo((std::string)fromDoc["FirstName"].get_string(),
                  (std::string)fromDoc["SecondName"].get_string(),
                  (std::string)fromDoc["Faculty"].get_string())
{
    auto arr = doc["LessonsTypes"];
    if(arr)
    {
        for (auto studId : arr.get_array().value)
        {
            auto res = studId.get_value().get_string().value;
            lessonsTypes.push_back(std::string(res));
        }
    }
}

StudentInfoDB::StudentInfoDB(document::value fromDoc) :
    AdditionalInfoDB(fromDoc),
    StudentInfo((std::string)fromDoc["FirstName"].get_string(),
                  (std::string)fromDoc["SecondName"].get_string(),
                  (int)fromDoc["Curs"].get_int32()) {}

AdditionalInfoDB::AdditionalInfoDB(document::value fromDoc) :
    IFromDB(fromDoc) {}

Schedule::Schedule(QDate date, std::string name, enum Para paraNumber) : Date(date), Name(name), ParaNumber(paraNumber){}

void Schedule::setTeacher(TeacherInfoDB *teacher)
{
    this->Teacher = teacher;
    if(teacher != NULL)
    {
        this->Teacher->OwnedCount++;
    }
}

void Schedule::setGroup(GroupDB *acc)
{
    this->Group = acc;
    if(acc != NULL)
    {
        this->Group->OwnedCount++;
    }
}
