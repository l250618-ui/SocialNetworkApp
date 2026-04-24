#pragma once
#include "Post.h"
#include "Entity.h"
#include <string>
using namespace std;

class PostManager {
    Post** posts;
    int postCount;
public:
    PostManager();
    void loadPosts();
    void loadComments();
    Post* getPost(string postID) const;
    bool likePost(string postID, Entity* user);
    bool commentOnPost(string postID, Entity* author, string text);
    void viewPost(string postID) const;
    void viewLikedList(string postID) const;
    int getPostCount() const;
    Post** getAllPosts() const;
    ~PostManager();
};