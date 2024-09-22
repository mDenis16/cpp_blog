#include <iostream>
#include <nlohmann/json.hpp>
#include "models/User.hpp"
#include "UserManager.hpp"

#include "Database.hpp"

CUserManager::CUserManager(std::shared_ptr<CDatabase> database) : mDatabase(database) {

}

CUserManager::~CUserManager() {

}

std::vector<CUser> CUserManager::GetAll() {

    std::vector<CUser> users;

    auto set = mDatabase->Query("SELECT id, email, username FROM USERS").GetSet();


    set
    .Member(0, &CUser::id)
    .Member(1, &CUser::email)
    .Member(2, &CUser::username)
    .Map(users);


    return users;
}


std::optional<CUser> CUserManager::Login(std::string username, std::string email, std::string password) {
    auto set = mDatabase->Query("SELECT id, username, email FROM USERS WHERE password = ?").GetSet();

    CUser user{};

    if (set
    .Member(0, &CUser::id)
    .Member(1, &CUser::username)
    .Member(2, &CUser::email)
    .Map(user)) {
        return user;
    }

    return{};
}
bool CUserManager::Register(std::string username, std::string password) {

}