//
// Created by Denis Topor on 20.09.2024.
//

#include "HttpRequest.hpp"

CHttpRequest::CHttpRequest() {

}
CHttpRequest::~CHttpRequest() {

}

std::shared_ptr<CHttpRequest> CHttpRequest::parseFromBuffer(void* buffer, size_t size, int con) {
    //std::cout << "CHttpRequest::parseFromBuffer " << std::endl;

    const char *method;
    size_t method_len;
    const char *path;
    size_t path_len;
    int minor_version;
    struct phr_header headers[100];
    size_t num_headers;

    static int requestId = 0;

    auto ret =  phr_parse_request((const char*)(buffer), size, &method, &method_len, &path, &path_len,
                      &minor_version, headers, &num_headers, 0);



    auto request = std::make_shared<CHttpRequest>();

    request->setPath(std::string(path, path_len));
    request->setVersion(minor_version);
    request->setConnection(con);
    request->setTypeString(std::string(method, method_len));

    request->setId(requestId);

    requestId++;
    for (size_t i = 0; i < num_headers; ++i) {
        auto& header = headers[i];
        request->addHeader(std::string(header.name, header.name_len), std::string(header.value, header.value_len));
    }

    return request;
}


void CHttpRequest::Return() {

}

void CHttpRequest::InvokeAsync(std::function<CHttpResponse(std::shared_ptr<CHttpRequest> &request)> callback) {
    
}
