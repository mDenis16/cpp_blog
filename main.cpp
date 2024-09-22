#include <iostream>
#include <fstream>
#include <future>
#include <sstream>

#include "src/HttpRequest.hpp"
#include "src/HttpServer.hpp"
#include  "src/libs/picohttpparser.h"
#include <sqlite3.h>



int main()
{

    sqlite3 *db;
    sqlite3_stmt *stmt;

    auto ret = sqlite3_open("test.sqlite3", &db);

    if (ret != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n",
        sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }


    static int ruta2 = 0;

     CHttpServer server(8081, 100);
    server.OnGet("/get-users", [&](CHttpRequest* request) {


         CHttpResponse response{};
              response.setStatus(200);
              response.addHeader("Content-Type", "text/html");
              response.addHeader("Cache-Control", "private, no-cache, no-store, must-revalidate, max-age=0");
              response.addHeader("Server", "MyServer");
              response.addHeader("Pragma", "no-cache");
            //  response.addHeader("Keep-Alive", "timeout=5, max=1000");

            std::stringstream ss;
            ss << "<html><body> Ruta " << ruta2++ <<  " </body></html>";

              response.setBody(ss.str());


            request->setResponse(response);

        return true;
    });

    server.OnGet("/home", [&](CHttpRequest* request) {


        server.InvokeAsync([request, db]() {


            static int global = 0;
            global++;
            CHttpResponse response{};
              response.setStatus(200);
              response.addHeader("Content-Type", "text/html");
              response.addHeader("Server", "MyServer");
            //  response.addHeader("Keep-Alive", "timeout=5, max=1000");



            char * errormsg;

            int ret = 0;
            int userCount = 0;
    int res = sqlite3_exec(db,
                       "SELECT * FROM USERS",
                       [](void* count, int columns, char** data, char** columnNames) -> int
                       {
                           std::cout << " got user " << data[1] << std::endl;
                           (*((int*)(count)))++;
                           return 0;
                       },
                       &userCount,
                       &errormsg);

            std::stringstream ss;
           ss << "<html><body>userCount "  << userCount << "!</body></html>";

             response.setBody(ss.str());

            request->setResponse(response);

            std::cout << "userCount " << userCount << std::endl;

        }, request);


        return false;
    });

    std::thread t([&]() {
            server.RunDispatcher();
        });

    t.detach();
    server.Listen();
    return 0;
}
