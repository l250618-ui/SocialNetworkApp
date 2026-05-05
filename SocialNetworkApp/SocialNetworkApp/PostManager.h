#pragma once
#include "Post.h"
#include "Entity.h"
#include <string>
using namespace std;

class User;
class Page;

class PostManager {
   Post** posts;
    int postCount;          
    static const int MAX_POSTS = 100; 
public:
    PostManager();
    void loadPosts(User** users, int userCount, Page** pages, int pageCount);
    void loadComments(User** users, int userCount, Page** pages, int pageCount);
    Post* getPost(string postID) const;
    bool likePost(string postID, Entity* user);
    bool commentOnPost(string postID, Entity* author, string text);
     void addPost(Post* p);      
    void viewPost(string postID) const;
    void viewLikedList(string postID) const;
    int getPostCount() const;
    Post** getAllPosts() const;
    Post* getPostByIndex(int i) const;
    ~PostManager();
};