
#include "QueryResultSet.hpp"


CQueryResultSet::CQueryResultSet() {

}
CQueryResultSet::~CQueryResultSet() {

}
void CQueryResultSet::Add(CQueryResult &result) {
    results.push_back(std::move(result));
}
void CQueryResultSet::Add(CQueryResult result) {
    results.push_back(std::move(result));
}

bool CQueryResultSet::MapNextResult(void* objectPtr) {

    if (current > (results.size() - 1) || results.empty())
        return false;

    std::cout << "CMapVector::MapNextResult generate objectPtr " << objectPtr << std::endl;

    for(auto& instruction : _instructions) {
        auto& result = results.at(current);

        if (result.GetType() == CQueryResultDataType::TEXT) {
            *(std::string*)((std::uint64_t)objectPtr + instruction.offset) = result.GetString();
            std::cout << " text: "  << result.GetString() << std::endl;
        }
        if (result.GetType() == CQueryResultDataType::INTEGER) {
            *(int*)((std::uint64_t)objectPtr + instruction.offset) = result.GetInt();
            std::cout << "id: " << result.GetInt();
        }
        current++;
        if (current > (results.size() - 1))
            break;
    }
    //std::cout << "CMapVector::MapNextResult end " << std::endl;

    return true;
}

