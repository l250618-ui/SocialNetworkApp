#include "PostManager.h"
#include "ActivityPost.h"
#include "Memory.h"
#include "Post.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "User.h"
#include "Page.h"
using namespace std;

PostManager::PostManager() {
    posts = new Post*[MAX_POSTS];
    for (int i = 0; i < MAX_POSTS; i++) {
        posts[i] = nullptr;
    }
    postCount = 0;
}

void PostManager::loadPosts(User** users, int userCount, Page** pages, int pageCount){ 
 ifstream file("Posts.txt");
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string type, id, sharedByID, desc, day, month, year, actType, actValue;
        
        getline(ss, type, ',');
        getline(ss, id, ',');
        getline(ss, sharedByID, ',');
        getline(ss, desc, ',');
        getline(ss, day, ',');
        getline(ss, month, ',');
        getline(ss, year, ',');
        
        Date date(stoi(day), stoi(month), stoi(year));
        
        if (type == "P") {
            // simple post - sharedBy will be set later by app
           Entity* owner = nullptr;
for (int i = 0; i < userCount; i++)
    if (users[i]->getID() == sharedByID) { owner = users[i]; break; }
if (!owner)
    for (int i = 0; i < pageCount; i++)
        if (pages[i]->getID() == sharedByID) { owner = pages[i]; break; }
Post* p = new Post(id, desc, date, owner);
posts[postCount++] = p;
        }
        else if (type == "A") {
            getline(ss, actType, ',');
            getline(ss, actValue, ',');
           Entity* owner = nullptr;
      for (int i = 0; i < userCount; i++)
    if (users[i]->getID() == sharedByID) { owner = users[i]; break; }
      if (!owner)
    for (int i = 0; i < pageCount; i++)
        if (pages[i]->getID() == sharedByID) { owner = pages[i]; break; }
        ActivityPost* p = new ActivityPost(id, desc, date, owner, stoi(actType), actValue);
          posts[postCount++] = p;
        }
    }
    file.close();
}

void PostManager::loadComments(User** users, int userCount, Page** pages, int pageCount) {
       ifstream file("Comments.txt");
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string postID, authorID, text;
        
        getline(ss, postID, ',');
        getline(ss, authorID, ',');
        getline(ss, text, ',');
        
        Post* p = getPost(postID);
if (p != nullptr) {
    Entity* author = nullptr;
    for (int i = 0; i < userCount; i++)
        if (users[i]->getID() == authorID) { author = users[i]; break; }
    p->addComment(author, text);
}
    }
    file.close();
}

Post* PostManager::getPost(string postID) const {
    for (int i = 0; i < postCount; i++) {
        if (posts[i] != nullptr && posts[i]->getID() == postID) {
            return posts[i];
        }
    }
    return nullptr;
}

bool PostManager::likePost(string postID, Entity* user) {
    Post* p = getPost(postID);
    if (p == nullptr) {
        cout << "Post not found." << endl;
        return false;
    }
    return p->likePost(user);
}
void PostManager::addPost(Post* p) {
    if (postCount < MAX_POSTS) {
        posts[postCount++] = p;
    }
}

bool PostManager::commentOnPost(string postID, Entity* author, string text) {
    Post* p = getPost(postID);
    if (p == nullptr) {
        cout << "Post not found." << endl;
        return false;
    }
    p->addComment(author, text);
    return true;
}

void PostManager::viewPost(string postID) const {
    Post* p = getPost(postID);
    if (p == nullptr) {
        cout << "Post not found." << endl;
        return;
    }
    p->display();
}

void PostManager::viewLikedList(string postID) const {
    Post* p = getPost(postID);
    if (p == nullptr) {
        cout << "Post not found." << endl;
        return;
    }
    p->displayLikedBy();
}

int PostManager::getPostCount() const {
    return postCount;
}

Post** PostManager::getAllPosts() const {
    return posts;
}
Post* PostManager::getPostByIndex(int i) const {
    if (i >= 0 && i < postCount) return posts[i];
    return nullptr;
}
PostManager::~PostManager() {
    for (int i = 0; i < postCount; i++) {
        delete posts[i];
        posts[i] = nullptr;
    }
    delete[] posts;
}