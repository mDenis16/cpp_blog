#include <iostream>
#include "Database.hpp"
#include "mini_orm/Query.hpp"
#include "models/User.hpp"
#include <any>

#include "App.hpp"
#include <fstream>
#include <sstream>
#include "libs/picohttpparser.h"

int main(){

  /*  std::ifstream t("request.txt");
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string req(buffer.str());

     const char *method, *path;
    int pret, minor_version;
    struct phr_header headers[100];
    size_t buflen = 0, prevbuflen = 0, method_len, path_len, num_headers;
    ssize_t rret;

    /* parse the request */
/*    num_headers = sizeof(headers) / sizeof(headers[0]);
    pret = phr_parse_request(req.data(), req.size(), &method, &method_len, &path, &path_len,
                             &minor_version, headers, &num_headers, prevbuflen);
*/

    CApp app{};
   app.Main();
}
