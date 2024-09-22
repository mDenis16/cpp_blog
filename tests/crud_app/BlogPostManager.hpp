#pragma once
#include <memory>
#include <optional>
#include <iostream>
class CDatabase;

class CBlogPostManager {
private:
    std::shared_ptr<CDatabase> mDatabase{};
    int id{};
    std::string title{};
    std::string content{};
    int posterId{};
public:
    CBlogPostManager( std::shared_ptr<CDatabase> database );



    std::optional<CBlogPost> GetById(int id);
    std::vector<CBlogPost> GetAll();

    int Add(CBlogPost post);

    ~CBlogPostManager();
};


