#include "HttpRequest.hpp"
#include "Helpers.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <string>

CHttpRequest::CHttpRequest() {

}
CHttpRequest::~CHttpRequest() {

}

int CHttpRequest::ParseHeaders(std::string buffer) {

        const char *method, *path;
        int pret, minor_version;
        static struct phr_header headers[257400];
        size_t buflen = 0, prevbuflen = 0, method_len, path_len, num_headers;
        ssize_t rret;

        /* parse the request */
        num_headers = sizeof(headers) / sizeof(headers[0]);
       auto ret = phr_parse_request(buffer.data(), buffer.size(), &method, &method_len, &path, &path_len,
                                 &minor_version, headers, &num_headers, lastlen);

        std::cout << " ret" << ret << std::endl;
        if (ret > 0) {
                setPath(std::string(path, path_len));
                setVersion(minor_version);
                setConnection(con);
                setTypeString(std::string(method, method_len));


                for (size_t i = 0; i < num_headers; ++i) {
                        auto& header = headers[i];
                        auto header_name = std::string(header.name, header.name_len);

                        std::transform(header_name.begin(), header_name.end(), header_name.begin(), ::tolower);

                        addHeader(header_name,std::string(header.value, header.value_len));
                }


                if (mHeaders.find("content-length") != mHeaders.end()) {
                        std::stringstream sstream(mHeaders["content-length"]);
                        sstream >> mContentLength;
                        mReadState = EHttpRequestReadState::BODY;
                }
                if (mHeaders.find("transfer-encoding") != mHeaders.end()) {
                      if (mHeaders["transfer-encoding"] == "chunked") {
                              mReadState = EHttpRequestReadState::CHUNKED;
                              mContentLength = -1;
                              memset(&mDecoder, 0, sizeof(mDecoder));
                      }
                }
                mParsedHeaders = true;

                auto chunk = buffer.substr(ret);
                Body().Append(chunk);
        }
        return ret;
}



void CHttpRequest::Return() {

}

void CHttpRequest::InvokeAsync(std::function<CHttpResponse(std::shared_ptr<CHttpRequest> &request)> callback) {

}
bool CHttpRequest::OnRecv(void* buffer, size_t size) {
    std::cout << "OnRecv size: " << size << std::endl;

    // Append the received data to the raw buffer
    std::string receivedData(static_cast<char*>(buffer), size);
    mRawBuffer.append(receivedData);

    // Check if headers have already been parsed
    if (!HasParsedHeaders()) {
        headersEnd = ParseHeaders(mRawBuffer);
        lastlen = size;

        // If headers are not fully parsed, exit
        if (headersEnd == -1) {
            return false;
        }
    }

    // Handle request body based on the current read state
    switch (mReadState) {
        case EHttpRequestReadState::BODY:
            // If the body is completely received, stop further processing
            if (mRawBuffer.size() >= (mContentLength + headersEnd)) {
                std::cout << "Finished receiving request body" << std::endl;
                return false;
            }
            break;

        case EHttpRequestReadState::CHUNKED: {
            // If no new data has been added beyond the headers, continue waiting
            if (headersEnd == mRawBuffer.size()) {
                std::cout << "Still empty at this point" << std::endl;
                return true;
            }

            // Decode the chunked body
            size_t bytesDecoded = size;
            char* bodyData = &mRawBuffer[headersEnd + decodePos];
            int decodeResult = phr_decode_chunked(&mDecoder, bodyData, &bytesDecoded);

            // Update the decode position and adjust buffer size
            decodePos += bytesDecoded;
            mRawBuffer.resize(decodePos + headersEnd);

            // Handle decoding errors and completion
            if (decodeResult == -1) {
                std::cerr << "Parse error in chunked encoding" << std::endl;
                std::cerr << "Current buffer size: " << mRawBuffer.size() << std::endl;
                return false;
            } else if (decodeResult != -2) {
                std::cout << "Finished receiving chunks!" << std::endl;
            }
            break;
        }

        default:
            std::cerr << "Unknown read state encountered" << std::endl;
            return false;
    }

    return true;
}
EHttpRequestType CHttpRequest::MethodToType(std::string typeString) {
        if (typeString == "GET")
                return EHttpRequestType::GET;
        else if (typeString == "HEAD")
                return EHttpRequestType::HEAD;
        else if (typeString == "OPTIONS")
                return EHttpRequestType::OPTIONS;
        else if (typeString == "TRACE")
                return EHttpRequestType::TRACE;
        else if (typeString == "PUT")
                return EHttpRequestType::PUT;
        else if (typeString == "DELETE")
                return EHttpRequestType::DELETE;
        else if (typeString == "POST")
                return EHttpRequestType::POST;
        else if (typeString == "PATCH")
                return EHttpRequestType::PATCH;
        else if (typeString == "CONNECT")
                return EHttpRequestType::CONNECT;
        else
                return EHttpRequestType::MIN;
}

std::string CHttpRequest::extractBoundary(const std::string& contentType) {
        std::string boundary = "";
        std::string boundaryKey = "boundary=";

        // Find the position of boundary in the Content-Type header
        std::size_t boundaryPos = contentType.find(boundaryKey);
        if (boundaryPos != std::string::npos) {
                // Extract the boundary value
                boundaryPos += boundaryKey.length();
                boundary = contentType.substr(boundaryPos);
        }
        return boundary;
}

void CHttpRequest::ParseBody() {
       if (! mHeaders.contains("Content-Type")) return;

        auto contentType = mHeaders["Content-Type"];
        size_t pos = 0;

        if (contentType.find("multipart/form-data") != std::string::npos) {
              auto boundary =   extractBoundary(contentType);
                mBody.ParseFormData(mBody.GetString(), boundary);
        }
}