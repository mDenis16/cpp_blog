#pragma once

class CBlogPost {
public:
    CBlogPost() {

    }
    ~CBlogPost() {

    }
    int id;
    std::string title;
    std::string content;
    std::string posterName;
};




inline void to_json(nlohmann::json& j, const CBlogPost& p) {
    j = nlohmann::json{
                                            { "id", p.id },
                                            { "title", p.title },
                                            { "content", p.content },
                                             {"posterName", p.posterName}};
}


inline void from_json(const nlohmann::json& j, CBlogPost& p) {
    j.at("id").get_to(p.id);
    j.at("title").get_to(p.title);
    j.at("content").get_to(p.content);
    j.at("posterName").get_to(p.posterName);
}

/*


-- create
CREATE TABLE POSTS (
  id INTEGER PRIMARY KEY,
  title TEXT NOT NULL,
  content TEXT NOT NULL,
  posterId INTEGER NOT NULL
);

CREATE TABLE USERS (
  id INTEGER PRIMARY KEY,
  username TEXT NOT NULL,
  email TEXT NOT NULL,
  password TEXT NOT NULL
);


-- insert
INSERT INTO USERS (username, email, password) VALUES ('denis', 'denis@dev.com', 'test');
INSERT INTO USERS (username, email, password) VALUES ('lightsquare', 'lighty@dev.com', 'test');
INSERT INTO USERS (username, email, password) VALUES ('clau', 'clau@dev.com', 'test');
INSERT INTO USERS (username, email, password) VALUES ('nikeftw', 'nikeftw@dev.com', 'test');
INSERT INTO USERS (username, email, password) VALUES ('danbalan2gx', 'danbalan2gx@dev.com', 'test');
INSERT INTO USERS (username, email, password) VALUES ('leexx', 'leexx@dev.com', 'test');
INSERT INTO USERS (username, email, password) VALUES ('raul2k', 'raul2k@dev.com', 'test');
-- fetch

INSERT INTO POSTS (title, content, posterId) VALUES ('titleTest', 'contentTest', 0);

 */