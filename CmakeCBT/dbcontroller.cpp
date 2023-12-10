#include "dbcontroller.h"

DBController* DBController::singleton = nullptr;

DBController::DBController()
{

    try
    {
        const auto uri = mongocxx::uri{"mongodb+srv://User:User@cluster.jthkjil.mongodb.net/?retryWrites=true&w=majority"};

        mongocxx::options::client client_options;
        const auto api = mongocxx::options::server_api{mongocxx::options::server_api::version::k_version_1};
        client_options.server_api_opts(api);

        client = mongocxx::client { uri, client_options };
        DB = client["CBTBase"];

        accs = AccountsController(DB["Accounts"]);
        grps = GroupsController(DB["Groups"]);
        infos = InfosController(DB["Infos"]);
        sched = ScheduleController(DB["Schedules"]);

        const auto ping_cmd = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("ping", 1));
        DB.run_command(ping_cmd.view());
    }
    catch (const std::exception& e)
    {
        errors::Error(e.what());
    }
}

DBController* DBController::GetInstance()
{
    if(singleton == nullptr){
        singleton = new DBController();
    }
    return singleton;
}



void DBController::Test()
{
    qDebug() << bsoncxx::to_json(make_document(kvp("as", "as")));
}

bool DBController::AccountsController::IsAccountExist(LogPass passLog) {return Coll.find_one(passLog.toDoc()).has_value();}

AccountDB* DBController::AccountsController::FindFullAccount(LogPass passLog)
{
    AccountDB* ac = new AccountDB(Coll.find_one(passLog.toDoc()).value());
    Cash.push_back(ac);
    return ac;
}

template<typename DBType, typename WritеType>
DBController::Controller<DBType, WritеType>::Controller() {}
template<typename DBType, typename WritеType>
DBController::Controller<DBType, WritеType>::Controller(mongocxx::collection collection) : Coll(collection) {}

DBController::AccountsController::AccountsController() : Controller<AccountDB, Account>() {}
DBController::AccountsController::AccountsController(mongocxx::collection collection) : Controller(collection) {}

DBController::InfosController::InfosController() : Controller<AdditionalInfoDB, AdditionalInfo>() {}
DBController::InfosController::InfosController(mongocxx::collection collection) : Controller(collection) {}

DBController::GroupsController::GroupsController() : Controller<GroupDB, Group>() {}
DBController::GroupsController::GroupsController(mongocxx::collection collection) : Controller(collection) {}

DBController::ScheduleController::ScheduleController() : Controller<ScheduleDB, Schedule>() {}
DBController::ScheduleController::ScheduleController(collection collection) : Controller(collection) {}
