#include <iostream>
#include <nlohmann/json.hpp>
#include "models/BlogPost.hpp"
#include "models/User.hpp"
#include "BlogPostManager.hpp"
#include "UserManager.hpp"
#include <HttpServer.hpp>
#include "Database.hpp"
#include <sstream>
#include "App.hpp"

#include "../../src/HttpServer.hpp"


CApp::CApp() {
    mDatabase = std::make_shared<CDatabase>("test.sqlite3");
    mBlogPostManager = std::make_unique<CBlogPostManager>(mDatabase);
    mUserManager = std::make_unique<CUserManager>(mDatabase);
    mServer = std::make_unique<CHttpServer>(8081, 100);
}

CApp::~CApp() {

}
/*
bool CApp::GetUsers(CHttpRequest* request) {

    mServer->InvokeAsync([request, this]() {
        auto users = mUserManager->GetAll();

         CHttpResponse response{};
             response.setStatus(200);
             response.addHeader("Content-Type", "application/json");
             response.addHeader("Cache-Control", "private, no-cache, no-store, must-revalidate, max-age=0");
             response.addHeader("Server", "MyServer");
             response.addHeader("Pragma", "no-cache");


            nlohmann::json j;
            for(auto& user : users) {
                nlohmann::json userj;
                user.ToJson(userj);
                j.push_back(userj);
            }

           response.setBody(j.dump());

         request->setResponse(response);
    }, request);

    return false;
}*/

void CApp::Main() {

    mServer->OnGet( "/posts", [this](CHttpRequest* request) {
        mServer->InvokeAsync([request, this]() {
        auto posts= mBlogPostManager->GetAll();
         CHttpResponse response{};
            response.setStatus(200);
            response.addHeader("Content-Type", "application/json");
            response.addHeader("Cache-Control", "private, no-cache, no-store, must-revalidate, max-age=0");
            response.addHeader("Server", "MyServer");
            response.addHeader("Pragma", "no-cache");



           nlohmann::json j(posts);

          response.setBody(j.dump());

        request->setResponse(response);
    }, request);
        return false;
    });
  //  mServer->OnGet( "/users",std::bind(&CApp::GetUsers,
    //        this, std::placeholders::_1));

    if (mDatabase->Open())
        mServer->Listen();
    else
        std::cout << "Failed to open database " << std::endl;
}

