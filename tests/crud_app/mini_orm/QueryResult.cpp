#pragma once
#include "QueryResult.hpp"

CQueryResult::CQueryResult() {

}
CQueryResult::~CQueryResult() {

}
CQueryResult::CQueryResult(const char* string) {
    type = CQueryResultDataType::TEXT;
    data = std::string(string);
}
CQueryResult::CQueryResult(std::string text) {
    type = CQueryResultDataType::TEXT;
    data = text;
}
CQueryResult::CQueryResult(double value) {
    type = CQueryResultDataType::REAL;
    data = value;
}
CQueryResult::CQueryResult(int value) {
    type = CQueryResultDataType::INTEGER;
    data = value;
}
