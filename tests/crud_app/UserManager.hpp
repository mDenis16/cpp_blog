#pragma once

class CDatabase;

class CUserManager {
private:
    std::shared_ptr<CDatabase> mDatabase;
public:
    CUserManager(std::shared_ptr<CDatabase> database);
    ~CUserManager();

    void AddUser(CUserManager user);
    void RemoveUser(CUserManager user);

    std::optional<CUser> Login(std::string username, std::string email, std::string password);
    bool Register(std::string username, std::string password);
    std::vector<CUser> GetAll();
};
