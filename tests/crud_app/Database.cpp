#include  "Database.hpp"

CDatabase::CDatabase(std::string path) : mPath(path) {

    std::cout << path << std::endl;
}
CDatabase::~CDatabase() {

}

bool CDatabase::Open() {
    sqlite3_open(mPath.c_str(), &mDB);
    return mDB != nullptr;
}
