#pragma once
#include <iostream>
#include "libs/picohttpparser.h"
#include <map>
#include <optional>
#include "HttpResponse.hpp"
enum class EHttpRequestType : uint8_t {
    MIN = 0,
    GET,
    HEAD,
    OPTIONS,
    TRACE,
    PUT,
    DELETE,
    POST,
    PATCH,
    CONNECT
};

class CHttpRequest {
    int con{-1};
    std::vector<uint8_t> mBody{};
    std::map<std::string, std::string> mHeaders{};
    std::string mPath{};
    EHttpRequestType mType{EHttpRequestType::MIN};
    std::string mTypeString;
    std::shared_ptr<CHttpResponse> mResponse;
    int mVersion{-1};
    int Id{0};




public:
    CHttpRequest();
    ~CHttpRequest();

    static std::shared_ptr<CHttpRequest> parseFromBuffer(void* buffer, size_t size, int con);

    inline EHttpRequestType getType() { return mType; }



    void setId(int id) {
        Id = id;
    }
    int getId() {
        return Id;
    }
    inline std::shared_ptr<CHttpResponse>& getResponse() { return mResponse; }
    void setResponse(std::shared_ptr<CHttpResponse> response) { mResponse = response; }

    inline std::string& getPath(){ return mPath; }


    void Return();
    inline int GetConnection() { return con; }

    void InvokeAsync(std::function<CHttpResponse(std::shared_ptr<CHttpRequest>& request)> callback);


private:
    inline void addHeader(std::string value, std::string key) { mHeaders[key] = value; }
    inline void setPath(std::string path) { mPath = path; }
    inline void setVersion(int version){ mVersion = version; }
    inline void setTypeString(std::string typeString) { mTypeString = typeString; }
    inline void setConnection(int _con) { con = _con; }

};
