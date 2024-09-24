
#include "HttpResponse.hpp"
#include <sstream>
CHttpResponse::CHttpResponse(nlohmann::json jobject) {
    setBody(jobject.dump());
    addHeader("Content-Type", "application/json");
    setStatus(200);
    addHeader("Cache-Control", "private, no-cache, no-store, must-revalidate, max-age=0");
}
CHttpResponse::CHttpResponse() {

}
CHttpResponse::~CHttpResponse() {

}

std::vector<uint8_t> CHttpResponse::Serialize() {
    std::stringstream ss;
    ss << "HTTP/1.1 " << mStatusCode << " " << mReasonPhrase << "\r\n";

    for (const auto& header : mHeaders) {
        ss << header.first << ": " << header.second << "\r\n";
    }

    ss <<  "\r\n";

    ss << mBody;

    const auto& buff = ss.str();

    return std::vector<uint8_t>(buff.begin(), buff.end());
}

void CHttpResponse::addHeader(std::string key, std::string value) {
    mHeaders[key] = value;
}
void CHttpResponse::removeHeader(std::string key) {
    std::map<std::string,std::string>::iterator iter = mHeaders.find(key) ;
    if( iter != mHeaders.end() )
        mHeaders.erase(iter);
    else
        std::cout << "Couldn't find header " << key << std::endl;
}

