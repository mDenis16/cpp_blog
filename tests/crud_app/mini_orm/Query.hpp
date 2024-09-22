#pragma once
#include  <iostream>
#include <sqlite3.h>


#include "QueryResultSet.hpp"

template <typename ClassType, typename MemberType>
constexpr size_t offset_of(MemberType ClassType::*member) {
    return reinterpret_cast<size_t>(&(reinterpret_cast<ClassType*>(0)->*member));
}

class CQuery {
private:
    sqlite3 *mDB{nullptr};
    CQueryResultSet mSet;
    std::string EscapeQuery(const std::string& str) {
        int len = str.length();
        int newlen = len;

        // First pass: count how much the length needs to be increased.
        for (int i = 0; i < len; ++i) {
            switch (str[i]) {
                case '\'':
                case '"':
                    ++newlen;  // Increase the length for escaping
                break;
            }
        }

        // If the length hasn't changed, just return the original string.
        if (newlen == len) {
            return str;
        }

        // Otherwise, create a new string with the expanded length.
        std::string newstr;
        newstr.reserve(newlen);  // Preallocate space for efficiency.

        // Second pass: build the new string with escape characters.
        for (int i = 0; i < len; ++i) {
            switch (str[i]) {
                case '\'':
                    newstr += '\'';
                break;
                case '"':
                    newstr += '\\';
                break;
            }
            newstr += str[i];  // Always add the original character.
        }

        return newstr;
    }
public:

    CQuery(sqlite3* db) : mDB(db) {

    }
    CQuery() {

    }

    sqlite3_stmt *stmt;

    int current_index = 1;

    CQuery& Statement(std::string statement) {
        if (sqlite3_prepare_v2(mDB, statement.c_str(), -1, &stmt, NULL)) {
            throw "";
        }
        while (sqlite3_step(stmt) != SQLITE_DONE) {

            int num_cols = sqlite3_column_count(stmt);


            for (int i = 0; i < num_cols; i++)
            {
                switch (sqlite3_column_type(stmt, i))
                {
                    case (SQLITE3_TEXT):
                        mSet.Add(CQueryResult(reinterpret_cast< char const* >( sqlite3_column_text(stmt, i))));
                    break;
                    case (SQLITE_INTEGER):
                        mSet.Add(CQueryResult(sqlite3_column_int(stmt, i)));
                    break;
                    case (SQLITE_FLOAT):
                        mSet.Add(CQueryResult(sqlite3_column_double(stmt, i)));
                    break;
                    default:
                        break;
                }
            }
        }
        sqlite3_finalize(stmt);
        return *this;
    }
    CQuery& Bind() {
        return *this;
    }


    int LastInsertedId() {
        return (int)sqlite3_last_insert_rowid(mDB);
    }
    CQueryResultSet GetSet() {
        CQueryResultSet set;

        return set;
    }
    // Variadic template Bind
    template <typename T, typename... Args>
    CQuery& Bind(T&& first, Args&&... args) {
        std::cout << "Bind current index " <<  current_index << std::endl;
        if constexpr (std::is_same_v<std::decay_t<T>, int>)
            sqlite3_bind_int (stmt, current_index, static_cast<int>(first));
        else if constexpr (std::is_same_v<std::decay_t<T>, float> || std::is_same_v<std::decay_t<T>, double>)
            sqlite3_bind_double (stmt, current_index, static_cast<double>(first));
        else if constexpr (std::is_same_v<std::decay_t<T>, std::string>)
            sqlite3_bind_text(stmt, current_index, static_cast<std::string>(first).c_str(), -1, nullptr);
        else if constexpr (std::is_same_v<std::decay_t<T>, const char*>)
            sqlite3_bind_text(stmt, current_index, static_cast<const char*>(first), -1, nullptr);
        else {
            std::cout << "bind unknown type called" << std::endl;
        }

        auto old_index = current_index;
        current_index++;
        Bind(std::forward<Args>(args)...);
        return *this;
    }

};
