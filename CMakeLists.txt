cmake_minimum_required(VERSION 3.28)
project(cpp_blog)

set(CMAKE_CXX_STANDARD 20)
find_package(SQLite3 REQUIRED)

include_directories(${SQLITE3_INCLUDE_DIRS} src/ libs/json/single_include//)




add_executable(cpp_blog
        src/HttpServer.cpp
        src/HttpRequest.cpp
        src/HttpResponse.cpp
        src/libs/picohttpparser.c
        src/DispatchQueue.cpp
        src/DispatchTask.cpp

        tests/crud_app/Database.cpp
        tests/crud_app/Database.hpp
        tests/crud_app/UserManager.cpp
        tests/crud_app/App.cpp
        tests/crud_app/BlogPostManager.cpp
        tests/crud_app/main.cpp
        tests/crud_app/mini_orm/Query.cpp
        tests/crud_app/mini_orm/Query.hpp
        tests/crud_app/mini_orm/QueryResultSet.cpp
        tests/crud_app/mini_orm/QueryResultSet.hpp
        tests/crud_app/mini_orm/QueryResult.cpp
        tests/crud_app/mini_orm/QueryResult.hpp
        src/RouteHandler.cpp
        src/RouteHandler.hpp
        src/RequestBody.cpp
        src/RequestBody.hpp
)



target_link_libraries (cpp_blog SQLite3)