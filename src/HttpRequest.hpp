#pragma once
#include <iostream>
#include "libs/picohttpparser.h"
#include <map>
#include <optional>
#include "HttpResponse.hpp"
#include "RequestBody.hpp"

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

enum class EHttpRequestReadState : uint8_t
{
    REQUEST = 0x0,
    BODY,
    CHUNKED
};
class CHttpRequest {
    int con{-1};
    CRequestBody mBody;
    std::map<std::string, std::string> mHeaders{};
    std::string mPath{};
    EHttpRequestType mType{EHttpRequestType::MIN};
    std::string mTypeString;
    std::shared_ptr<CHttpResponse> mResponse;
    int mVersion{-1};
    int Id{0};
    phr_chunked_decoder mDecoder;




    bool mParsedHeaders{false};



public:
    EHttpRequestReadState mReadState;
    CHttpRequest();
    ~CHttpRequest();
    size_t mContentLength;
    size_t lastlen;
    size_t bodyStart{0};


    bool OnRecv(void* buffer, size_t size);

    bool HasParsedHeaders() {
        return mParsedHeaders;
    }
    int ParseHeaders(std::string buffer);

    inline EHttpRequestType getType() { return mType; }



    size_t decodePos = 0;
    size_t headersEnd;

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

    CRequestBody& Body(){ return mBody; }

    std::string mRawBuffer{};

private:
    inline void addHeader(std::string value, std::string key) { mHeaders[value] = key; }

    inline std::optional<std::string> GetHeader(std::string key) {
        if (mHeaders.contains(key))
            return mHeaders[key];
        return {};
    }
    inline void setPath(std::string path) { mPath = path; }
    inline void setVersion(int version){ mVersion = version; }
    inline void setTypeString(std::string typeString) {
        mTypeString = typeString;
        mType = MethodToType(typeString);
    }
    inline void setConnection(int _con) { con = _con; }
    inline static EHttpRequestType MethodToType(std::string typeString);

    std::string extractBoundary(const std::string& contentType);
    void ParseBody();
    std::vector<std::uint8_t> rawBuffer;
};
