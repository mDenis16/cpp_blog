#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>



class CHttpResponse {
private:
    int mStatusCode;
    std::string mReasonPhrase;
    std::map<std::string, std::string> mHeaders;
    std::string mBody;
public:
    CHttpResponse(nlohmann::json j);
    CHttpResponse();
    ~CHttpResponse();

    std::vector<uint8_t> Serialize();
    void addHeader(std::string key, std::string value);
    void removeHeader(std::string key);


    void setBody(std::string body) {
        mBody = body;
    }
    void setStatus(int statusCode) {
        mStatusCode = statusCode;
    }
    CHttpResponse& operator=(const CHttpResponse& other) {
        if (this != &other) { // Check for self-assignment
            mStatusCode = other.mStatusCode;
            mReasonPhrase = other.mReasonPhrase;
            mHeaders = other.mHeaders; // std::map assignment is fine
            mBody = other.mBody;
        }
        return *this;
    }
};
