#include <iostream>
#include <nlohmann/json.hpp>
#include "models/BlogPost.hpp"
#include "BlogPostManager.hpp"
#include "Database.hpp"

CBlogPostManager::CBlogPostManager( std::shared_ptr<CDatabase> database ) : mDatabase(database) {

}

CBlogPostManager::~CBlogPostManager() {

}


std::vector<CBlogPost> CBlogPostManager::GetAll() {

    std::vector<CBlogPost> posts;

    auto set = mDatabase->Query("SELECT posts.id, posts.title, posts.content, users.username FROM posts JOIN users ON posts.posterId = users.id;").GetSet();

    set
    .Member(0, &CBlogPost::id)
    .Member(1, &CBlogPost::title)
    .Member(2, &CBlogPost::content)
    .Member(3, &CBlogPost::posterName)
    .Map(posts);

    return posts;
}

std::optional<CBlogPost> CBlogPostManager::GetById(int id) {
    CBlogPost post; bool exists =false;

    auto set = mDatabase->Query("SELECT * FROM Posts WHERE id = ? LIMIT 1").GetSet();

    if (set
    .Member(0, &CBlogPost::id)
    .Member(1, &CBlogPost::title)
    .Member(2, &CBlogPost::content)
    .Member(3, &CBlogPost::posterName)
    .Map(post))
        return post;


    return {};
}
int CBlogPostManager::Add(CBlogPost post) {
    return mDatabase->Query("INSERT INTO Posts (title,content, posterId) VALUES( ?,	?);").LastInsertedId();
}