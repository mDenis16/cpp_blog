#pragma once
#include <vector>

#include "QueryResult.hpp"

class CMapInstruction {
public:
    int index;
    size_t offset;
};
class CQueryResultSet {
public:
    CQueryResultSet();
    ~CQueryResultSet();

    std::vector<CQueryResult> results;

    void Add(CQueryResult result);
    void Add(CQueryResult& result);


    CQueryResult& Get(int index) {
        if (index >= results.size() || index < 0) {
            throw std::runtime_error("Access invalid index on CQueryResultSet.");
        }
        return results.at(index);
    }

    std::vector<CMapInstruction> _instructions;

    template <typename ClassType, typename MemberType>
    CQueryResultSet& Member(int index, MemberType ClassType::*member) {
        _instructions.push_back({index, reinterpret_cast<size_t>(&(reinterpret_cast<ClassType*>(0)->*member))});
        return *this;
    }

    int current = 0;

    template<class T>
    void Map(std::vector<T>& vec) {
        while (true) {
            auto instance = T();
            if (MapNextResult(&instance))
                vec.push_back(instance);
            else
                break;
        }
    }
    template<class T>
    bool Map(T instance) {
        if (MapNextResult(&instance))
            return  true;

        return  false;
    }
    bool MapNextResult(void* objectPtr);

};

