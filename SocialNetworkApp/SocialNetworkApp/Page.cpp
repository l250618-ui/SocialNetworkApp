#pragma once
#include "Entity.h"
#include <string>
using namespace std;

class Post;
class User;

class Page : public Entity {
    User* owner;
    Post** posts;
    int postCount;
    int likeCount;
public:
    Page();
    Page(string id, string name, User* owner);
    void addPost(Post* post);
    Post** getPosts() const;
    int getPostCount() const;
    int getLikeCount() const;
    void incrementLikes();
    void displayPosts() const;
    void display() const override;
    ~Page();
};