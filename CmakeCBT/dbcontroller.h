#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H
#include <string>
#include <cstdint>
#include <iostream>
#include <vector>


#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>

#include "errors.h"
#include "model.h"

using namespace mongocxx;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;



class DBController
{
public:
    static DBController* GetInstance();

    template<typename DBType, typename WritеType>
    class Controller
    {
    protected:
        std::list<DBType*> Cash = std::list<DBType*>();
        bool IsChanged = true;
        bool IsAlredyInCash(bsoncxx::oid oid, DBType **res)
        {
            for (auto iter = Cash.begin(); iter != Cash.end(); ++iter)
            {
                if((*iter)->isExist == false)
                {
                    (*iter)->OwnedCount--;
                    if((*iter)->OwnedCount == 0)
                    {
                        Cash.remove(*iter);
                    }
                    continue;
                }
                if((*iter)->_id == oid)
                {
                    *res = *iter;
                    return true;
                }
            }
            return false;
        }
    public:
        collection Coll;
        virtual DBType* GetFromDB(bsoncxx::oid oid)
        {
            DBType* res;
            if(IsAlredyInCash(oid, &res))
            {
                return res;
            }
            else
            {
                auto qeRes = this->Coll.find_one(make_document(kvp("_id", oid)));
                if(!qeRes.has_value())
                {
                    return NULL;
                }

                DBType*  acc = new DBType(bsoncxx::document::value((qeRes.value())));
                Cash.push_back(acc);
                acc->OwnedCount++;
                return acc;
            }
        }
        int GetCount()
        {
            return Coll.count_documents(make_document());
        }

        virtual std::list<DBType*>GetAll()
        {
            if(!IsChanged)
            {
                return std::list<DBType*>(Cash);
            }

            std::list<DBType*> res = std::list<DBType*>();
            auto qres = Coll.find(make_document());
            for(auto doc : qres)
            {
                DBType* inst;
                if(IsAlredyInCash(doc["_id"].get_oid().value, &inst))
                {
                    res.push_back(inst);
                }
                else
                {
                    inst = new DBType(bsoncxx::document::value(doc));
                    res.push_back(inst);
                    Cash.push_back(inst);
                }
            }
            IsChanged = false;
            return res;
        }

        virtual DBType* Add(WritеType* element)
        {
            IsChanged = true;
            DBType* res;
            mongocxx::stdx::optional<result::insert_one> queryRes = Coll.insert_one(element->toDoc());
            delete element;
            if(!queryRes.has_value())
            {
                return NULL;
            }

            if(queryRes.value().result().inserted_count() != 1)
            {
                //throw std::exception("Things getting crazy. Document writen multiple or zero times.");
            }

            auto doc = Coll.find_one(make_document(kvp("_id", queryRes.value().inserted_id()))).value();

            res = new DBType(std::move(doc));
            res->OwnedCount++;
            Cash.push_back(res);

            return res;
        }

        bool Delete(bsoncxx::oid oid)
        {
            IsChanged = true;
            auto iterForDelete = Cash.begin();
            for(auto i = Cash.begin(); i != Cash.end();i++)
            {
                if((*i)->_id == oid)
                {
                    iterForDelete = i;
                    break;
                }
            }
            ((DBType*)*iterForDelete)->OwnedCount--;
            if(((DBType*)*iterForDelete)->OwnedCount == 0)
            {
                delete ((DBType*)*iterForDelete);
            }
            else
            {
                ((DBType*)*iterForDelete)->isExist = false;
            }
            Cash.remove(*iterForDelete);
            return Coll.delete_one(make_document(kvp("_id", oid)))->deleted_count() == 1;
        }

        bool Update(DBType* el)
        {
            IFromDB* temp = el;
            qDebug() << bsoncxx::to_json(temp->toDoc());
            auto a = Coll.replace_one(make_document(kvp("_id", temp->_id)), temp->toDoc());
            temp->doc = bsoncxx::document::value(Coll.find_one(make_document(kvp("_id", temp->_id))).value());
            return a.value().matched_count() == 1;
        }
        Controller();
        Controller(mongocxx::collection collection);
    };

    class AccountsController : public Controller<AccountDB, Account>
    {
    private:
    public:
        AccountsController();
        AccountsController(mongocxx::collection collection);
        virtual std::list<AccountDB*>GetAllTeachers()
        {
            std::list<AccountDB*> res = std::list<AccountDB*>();
            auto qres = Coll.find(make_document(kvp("AccountType", 1)));
            for(auto doc : qres)
            {
                AccountDB* inst;
                if(IsAlredyInCash(doc["_id"].get_oid().value, &inst))
                {
                    res.push_back(inst);
                }
                else
                {
                    inst = new AccountDB(bsoncxx::document::value(doc));
                    res.push_back(inst);
                    Cash.push_back(inst);
                    inst->OwnedCount++;
                }
            }
            return res;
        }
        bool IsAccountExist(LogPass passLog);
        AccountDB* FindFullAccount(LogPass passLog);
    };
    AccountsController accs;

    class GroupsController : public Controller<GroupDB, Group>
    {
    private:
    public:
        GroupsController();
        GroupsController(mongocxx::collection collection);

    };
    GroupsController grps;

    class ScheduleController : public Controller<ScheduleDB, Schedule>
    {
    private:
    public:
        ScheduleController();
        virtual std::list<ScheduleDB*>GetAllSchedulesinDate(int JulianDate, GroupDB* group)
        {
            std::list<ScheduleDB*> res = std::list<ScheduleDB*>();
            auto qres = Coll.find(make_document(kvp("Date", JulianDate), kvp("Group", group->_id)));
            for(auto doc : qres)
            {
                ScheduleDB* inst;
                if(IsAlredyInCash(doc["_id"].get_oid().value, &inst))
                {
                    res.push_back(inst);
                }
                else
                {
                    inst = new ScheduleDB(bsoncxx::document::value(doc));
                    res.push_back(inst);
                    Cash.push_back(inst);
                    inst->OwnedCount++;
                }
            }
            return res;
        }
        ScheduleController(mongocxx::collection collection);

    };
    ScheduleController sched;

    class InfosController : public Controller<AdditionalInfoDB, AdditionalInfo>
    {
    private:
    public:
        AdditionalInfoDB* GetFromDB(bsoncxx::oid oid, AccountType type)
        {
            AdditionalInfoDB* res;
            if(IsAlredyInCash(oid, &res))
            {
                return res;
            }
            else
            {
                auto qeRes = this->Coll.find_one(make_document(kvp("_id", oid)));
                AdditionalInfoDB*  acc = NULL;

                if(!qeRes.has_value())
                {
                    return NULL;
                }

                if(AccountType::Admin != type)
                {
                    if(AccountType::Student == type)
                    {
                        acc = new StudentInfoDB(bsoncxx::document::value((qeRes.value())));
                    }
                    else if(AccountType::Teacher == type)
                    {
                        acc = new TeacherInfoDB(bsoncxx::document::value((qeRes.value())));
                    }
                    acc->OwnedCount++;
                    Cash.push_back(acc);
                }

                return acc;
            }
        }
        virtual std::list<AdditionalInfoDB*> GetAll() override
        {
            if(!IsChanged)
            {
                return std::list<AdditionalInfoDB*>(Cash);
            }

            std::list<AdditionalInfoDB*> res = std::list<AdditionalInfoDB*>();
            auto qres = Coll.find(make_document());
            for(auto doc : qres)
            {
                AdditionalInfoDB* inst;

                if(doc["Owner"].type() != bsoncxx::type::k_oid)
                {
                    continue;
                }

                if(IsAlredyInCash(doc["_id"].get_oid().value, &inst))
                {
                    res.push_back(inst);
                }
                else
                {
                    inst = new AdditionalInfoDB(bsoncxx::document::value(doc));
                    res.push_back(inst);
                    Cash.push_back(inst);
                }
            }
            IsChanged = false;
            return res;
        }
        AdditionalInfoDB* Add(AdditionalInfo* accInf, AccountType type)
        {
            IsChanged = true;
            AdditionalInfoDB* res = NULL;
            document::view temp = accInf->toDoc();
            delete accInf;
            auto insQeRes = Coll.insert_one(temp);

            if(!insQeRes.has_value())
            {
                return res;
            }

            auto qeRes = this->Coll.find_one(make_document(kvp("_id",  insQeRes.value().inserted_id())));

            if(!qeRes.has_value())
            {
                //throw std::exception("Was written, but can't find. Probably due to loss of internet");
            }

            if(AccountType::Admin != type)
            {
                AccountDB* tempAcc;
                if(AccountType::Student == type)
                {
                    StudentInfoDB* temp = new StudentInfoDB(bsoncxx::document::value(qeRes.value()));
                    tempAcc = temp->getOwner();
                    if(temp->getGroup())
                    {
                        temp->getGroup()->addStudent(temp);
                    }
                    res = temp;
                }
                else if(AccountType::Teacher == type)
                {
                    TeacherInfoDB* temp = new TeacherInfoDB(bsoncxx::document::value(qeRes.value()));
                    tempAcc = temp->getOwner();
                    res = temp;
                }
                tempAcc->setAdditionalInfo(res);
                DBController::GetInstance()->accs.Update(tempAcc);


                Cash.push_back(res);
                res->OwnedCount++;
            }

            return res;
        }
        InfosController();
        InfosController(mongocxx::collection collection);
    };
    InfosController infos;
    void Test();

private:
    DBController();
    DBController( const DBController& );
    DBController& operator=( DBController& );

    static DBController* singleton;

    mongocxx::instance instance {};
    mongocxx::client client;

    mongocxx::database DB;
};

#endif // DBCONTROLLER_H
