#pragma once
#include <iostream>
#include <sqlite3.h>

#include "mini_orm/Query.hpp"

class CDatabaseExecutionCtx {
public:
    std::function<void(char** data)> _callback;
    CDatabaseExecutionCtx(std::function<void(char** data)> cb){ _callback = cb; };
    ~CDatabaseExecutionCtx(){};
};
class CDatabase {
private:

    sqlite3_stmt *mSTMT{nullptr};
    std::string mPath{};

private:


public:
    sqlite3 *mDB{nullptr};
    CDatabase(std::string path);
    ~CDatabase();


    inline CQuery Query() {
        return CQuery{mDB};
    }
    inline CQuery Query(std::string query) {
        auto object =  CQuery{mDB};
        return object.Statement(query);
    }

    bool Open();
};