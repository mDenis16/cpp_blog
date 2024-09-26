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
void parseFormData(const std::string& data, const std::string& boundary) {
    // Split the data by boundary
    std::size_t startPos = 0;
    std::size_t boundaryPos = 0;
    std::string line;

    while ((boundaryPos = data.find(boundary, startPos)) != std::string::npos) {
        // Extract content between boundaries
        std::size_t contentStart = startPos + boundary.length() + 2; // Skip boundary and \r\n
        std::size_t contentEnd = boundaryPos - 2; // Remove \r\n before boundary
        std::string content = data.substr(contentStart, contentEnd - contentStart);

        if (!content.empty()) {
            // Parse headers and content
            std::istringstream contentStream(content);
            std::string contentDisposition;
            std::getline(contentStream, contentDisposition); // First line is Content-Disposition

            // Extract name from Content-Disposition
            std::size_t namePos = contentDisposition.find("name=\"");
            if (namePos != std::string::npos) {
                namePos += 6;
                std::size_t nameEnd = contentDisposition.find("\"", namePos);
                std::string name = contentDisposition.substr(namePos, nameEnd - namePos);

                // Skip two empty lines to get the actual value
                std::string value;
                std::getline(contentStream, line); // Skip empty line
                std::getline(contentStream, value); // The value

                // Output the name-value pair
                std::cout << "Field name: " << name << "\n";
                std::cout << "Field value: " << value << "\n";
            }
        }

        // Move past the boundary
        startPos = boundaryPos + boundary.length();
    }
}

void CApp::Main() {



     mServer->RouteHandler.AddRoute("/api/users", {EHttpRequestType::GET,
         [this](std::shared_ptr<CHttpRequest>& request) {
       return [this]() {
            auto users = mUserManager->GetAll();

           return  CHttpResponse{users};
       };
   }});

    mServer->RouteHandler.AddRoute("/api/auth/login", {EHttpRequestType::POST,
        [this](std::shared_ptr<CHttpRequest>& request) {
            auto body = request->Body();
        return CHttpResponse{ body.GetString() };
  }});
    /*
    *mServer->InvokeAsync([this]() {
            auto users = mUserManager->GetAll();
            return CHttpResponse();
        }, request);
     **/



    /*mServer->OnGet( "/posts", [this](CHttpRequest* request) {
        mServer->InvokeAsync([request, this]() {
        auto posts= mBlogPostManager->GetAll();
            auto response = std::make_shared<CHttpResponse>();
            response->setStatus(200);
            response->addHeader("Content-Type", "application/json");
            response->addHeader("Cache-Control", "private, no-cache, no-store, must-revalidate, max-age=0");
            response->addHeader("Server", "MyServer");
            response->addHeader("Pragma", "no-cache");

           nlohmann::json j(posts);

          response->setBody(j.dump());

        request->setResponse(response);
    }, request);
        return false;
    });*/
  //  mServer->OnGet( "/users",std::bind(&CApp::GetUsers,
    //        this, std::placeholders::_1));

    if (mDatabase->Open())
        mServer->Listen();
    else
        std::cout << "Failed to open database " << std::endl;
}

