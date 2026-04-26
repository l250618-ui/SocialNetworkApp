#include "PostManager.h"
#include "ActivityPost.h"
#include "Memory.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

const int MAX_POSTS = 100;

PostManager::PostManager() {
    posts = new Post*[MAX_POSTS];
    for (int i = 0; i < MAX_POSTS; i++) {
        posts[i] = nullptr;
    }
    postCount = 0;
}

void PostManager::loadPosts() {
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
            Post* p = new Post(id, desc, date, nullptr);
            posts[postCount++] = p;
        }
        else if (type == "A") {
            getline(ss, actType, ',');
            getline(ss, actValue, ',');
            ActivityPost* p = new ActivityPost(id, desc, date, nullptr, stoi(actType), actValue);
            posts[postCount++] = p;
        }
    }
    file.close();
}

void PostManager::loadComments() {
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
            p->addComment(nullptr, text);
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

PostManager::~PostManager() {
    for (int i = 0; i < postCount; i++) {
        delete posts[i];
        posts[i] = nullptr;
    }
    delete[] posts;
}