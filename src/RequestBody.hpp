#pragma once
#include <vector>
#include <iostream>
class CRequestBody {
    std::string mBuffer;
public:
    CRequestBody();

    ~CRequestBody();

    void Resize(size_t size ) {mBuffer.resize(size);}
    void Set(void* ptr, size_t size) {
        memcpy(mBuffer.data(), ptr, size);
    }

    void Append(std::string& chunk) {
        mBuffer.append(chunk);
    }
    std::string GetString() {
        return std::string(mBuffer.begin(), mBuffer.end());
    }
    void ParseFormData(const std::string& data, const std::string& boundary);
};
