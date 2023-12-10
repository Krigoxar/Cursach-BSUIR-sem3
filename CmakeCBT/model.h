#ifndef MODEL_H
#define MODEL_H

#include <QDateTime>
#include <list>
#include <string>
#include <cstdlib>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/types.hpp>
//#include <bsoncxx/config/prelude.hpp>

using namespace bsoncxx;

enum AccountType{
    Admin,
    Teacher,
    Student,
};

std::string myto_string(enum AccountType val);

struct IDocable
{
public:
    virtual document::view toDoc() = 0;
protected:
    virtual builder::stream::document* Build(builder::stream::document* builder) = 0;
};

struct IFromDB : IDocable
{
protected:
public:
    int OwnedCount = 0;
    bool isExist = true;

    bsoncxx::document::value doc;
    bsoncxx::oid _id;
    IFromDB(document::value fromDoc);
    virtual builder::stream::document* Build(builder::stream::document* builder) override;
};

struct AdditionalInfoDB;
struct StudentInfoDB;
struct TeacherInfoDB;
struct AccountDB;
struct GroupDB;
struct StudentInfo;
struct TeacherInfo;
struct Account;

struct Group : public IDocable
{
protected:
    std::list<StudentInfoDB*> Students = std::list<StudentInfoDB*>();
    TeacherInfoDB* Curator = NULL;

    virtual builder::stream::document* Build(builder::stream::document* builder) override;
public:
    Group(std::string name);
    std::string Name;

    void setCurator(TeacherInfoDB* teacher);
    void setStudents(std::list<StudentInfoDB*> students);
    virtual document::view toDoc() override;
};

struct LogPass : public IDocable
{
protected:
    virtual builder::stream::document* Build(builder::stream::document* builder) override;
public:
    LogPass(std::string login, std::string password);
    std::string Login;
    std::string Password;

    virtual document::view toDoc() override;
};

struct Account : public LogPass
{
protected:
    AdditionalInfoDB* AdditionalInfo = NULL;

    virtual builder::stream::document* Build(builder::stream::document* builder) override;
public:
    Account(std::string login, std::string password, AccountType accountType);
    AccountType AccountType;

    void setAdditionalInfo(AdditionalInfoDB* adInf);
    virtual document::view toDoc() override;
};

struct AdditionalInfo : public IDocable
{
protected:
    virtual builder::stream::document* Build(builder::stream::document* builder) override;
    AccountDB* Owner = NULL;
public:
    std::string FirstName;
    std::string SecondName;

    void setOwner(AccountDB* acc);
    virtual document::view toDoc() override;
};

struct Schedule : public IDocable
{
protected:
    virtual builder::stream::document* Build(builder::stream::document* builder) override;
    TeacherInfoDB* Teacher;
    GroupDB* Group;
public:
    QDate Date;
    std::string Name;
    enum Para{
        First,
        Second,
        Third,
        Fourth,
        Fifth,
        Sixth,
        Seventh,
        Eighth,
    };
    enum Para ParaNumber;
    Schedule(QDate date, std::string name, enum Para paraNumber);

    void setTeacher(TeacherInfoDB* teacher);
    void setGroup(GroupDB* acc);
    virtual document::view toDoc() override;
};

struct StudentInfo : public AdditionalInfo
{
protected:
    GroupDB* Group = NULL;

    virtual builder::stream::document* Build(builder::stream::document* builder) override;
public:
    StudentInfo(std::string firstName, std::string secondName, int curs);
    int Curs;

    void setGroup(GroupDB* acc);
    virtual document::view toDoc() override;
};

struct TeacherInfo : public AdditionalInfo
{
protected:
    virtual builder::stream::document* Build(builder::stream::document* builder) override;
public:
    TeacherInfo(std::string fritstName, std::string secondName, std::string faculty);
    std::string Faculty;
    std::list<std::string> lessonsTypes = std::list<std::string>();

    virtual document::view toDoc() override;
};

struct AccountDB : public Account, public IFromDB
{
public:
    AccountDB(document::value fromDoc);

    AdditionalInfoDB* getAdditionalInfo();

    virtual document::view toDoc() override;
};

struct ScheduleDB : public Schedule, public IFromDB
{
public:
    TeacherInfoDB* getTeacher();
    GroupDB* getGroup();

    ScheduleDB(document::value fromDoc);

    virtual document::view toDoc() override;
};

struct GroupDB : public Group, public IFromDB
{
public:

    TeacherInfoDB* getCurator();
    std::list<StudentInfoDB*> getStudents();
    bool addStudent(StudentInfoDB* stud);
    bool deleteStudent(StudentInfoDB* stud);

    GroupDB(document::value fromDoc);

    virtual document::view toDoc() override;
};

struct AdditionalInfoDB : public IFromDB
{
protected:
    virtual builder::stream::document* Build(builder::stream::document* builder) override;
public:
    AdditionalInfoDB(document::value fromDoc);

    virtual document::view toDoc() override;
};

struct TeacherInfoDB : public  AdditionalInfoDB, public TeacherInfo
{
public:
    TeacherInfoDB(document::value fromDoc);

    AccountDB* getOwner();
    virtual document::view toDoc() override;
};

struct StudentInfoDB : public  AdditionalInfoDB, public StudentInfo
{
public:
    StudentInfoDB(document::value fromDoc);

    GroupDB* getGroup();
    AccountDB* getOwner();
    virtual document::view toDoc() override;
};

#endif // MODEL_H
