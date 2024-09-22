#pragma once
#include <iostream>
enum class CQueryResultDataType : uint8_t  {
    MIN = 0,
    INTEGER,
    REAL,
    TEXT,
    BLOB,
    MAX
};
/*
*INTEGER. The value is a signed integer, stored in 0, 1, 2, 3, 4, 6, or 8 bytes depending on the magnitude of the value.

REAL. The value is a floating point value, stored as an 8-byte IEEE floating point number.

TEXT. The value is a text string, stored using the database encoding (UTF-8, UTF-16BE or UTF-16LE).

BLOB. The value is a blob of data, stored exactly as it was input.
 */

class CQueryResult {
    CQueryResultDataType type;
    std::variant<int, double, std::string> data;
public:
    CQueryResult();
    CQueryResult(const char* string);
    CQueryResult(std::string text);
    CQueryResult(double value);
    CQueryResult(int value);
    ~CQueryResult();


    int GetInt () {
        return std::get<int>(data);
    }
    std::string GetString () {
        return std::get<std::string>(data);
    }
    double GetDouble () {
        return std::get<double>(data);
    }
    float GetFloat () {
        return (double)std::get<double>(data);
    }
    CQueryResultDataType GetType() { return type; }
};

