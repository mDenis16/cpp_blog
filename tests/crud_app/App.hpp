#pragma once

class CHttpRequest;
class CDatabase;
class CBlogPostManager;
class CUserManager;
class CHttpServer;

class CApp {
public:
    std::unique_ptr<CBlogPostManager> mBlogPostManager;
    std::unique_ptr<CUserManager> mUserManager;
    std::shared_ptr<CDatabase> mDatabase;
    std::unique_ptr<CHttpServer> mServer;

    CApp();
    ~CApp();



    void Main();
};
