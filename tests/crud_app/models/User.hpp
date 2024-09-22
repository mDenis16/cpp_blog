#pragma once
#include <nlohmann/json.hpp>
class CUser {
public:
    CUser() {

    }
    ~CUser() {

    }
    int id;

    std::string username;
    std::string email;
};
inline void to_json(nlohmann::json& j, const CUser& p) {
    j = nlohmann::json{
                                                { "id", p.id },
                                                { "username", p.username },
                                                { "email", p.email }
    };
}


inline void from_json(const nlohmann::json& j, CUser& p) {
    j.at("id").get_to(p.id);
    j.at("username").get_to(p.username);
    j.at("email").get_to(p.email);
}