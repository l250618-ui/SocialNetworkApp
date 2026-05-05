#include "PostManager.h"
#include "ActivityPost.h"
#include "Memory.h"
#include "User.h"    
#include "Page.h"    
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

void PostManager::loadPosts(User** users, int userCount, Page** pages, int pageCount) {
    ifstream file("Posts.txt");
    if (!file.is_open()) {
        cout << "Failed to open Posts.txt" << endl;
        return;
    }

    int totalPosts;
    file >> totalPosts;

    int type;
    string postID;

    while (file >> type >> postID) {
        int day, month, year;
        file >> day >> month >> year;
        file.ignore();

        string desc;
        getline(file, desc);

        int actType = 0;
        string actValue = "";

        if (type == 2) {
            file >> actType;
            file.ignore();
            getline(file, actValue);
        }

        string sharedByID;
        getline(file, sharedByID);
        if (!sharedByID.empty() && sharedByID.back() == '\r')
            sharedByID.pop_back();

        string likersLine;
        getline(file, likersLine);

        Date date(day, month, year);
        Post* newPost = nullptr;

        if (type == 1) {
            newPost = new Post(postID, desc, date, nullptr);
        }
        else if (type == 2) {
            newPost = new ActivityPost(postID, desc, date, nullptr, actType, actValue);
        }

        if (newPost == nullptr) continue;
        newPost->setSharedByID(sharedByID);

        // resolve and store likers
        istringstream ls(likersLine);
        string likerID;
        while (ls >> likerID && likerID != "-1") {
            Entity* liker = nullptr;
            for (int i = 0; i < userCount; i++) {
                if (users[i]->getID() == likerID) {
                    liker = users[i];
                    break;
                }
            }
            if (liker == nullptr) {
                for (int i = 0; i < pageCount; i++) {
                    if (pages[i]->getID() == likerID) {
                        liker = pages[i];
                        break;
                    }
                }
            }
            if (liker != nullptr)
                newPost->likePost(liker);
        }

        posts[postCount++] = newPost;
    }
    file.close();
}

void PostManager::loadComments(User** users, int userCount, Page** pages, int pageCount) {
    ifstream file("Comments.txt");
    if (!file.is_open()) {
        cout << "Failed to open Comments.txt" << endl;
        return;
    }

    int total;
    file >> total;
    file.ignore();

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();

        istringstream ss(line);
        string cID, postID, authorID, text;
        ss >> cID >> postID >> authorID;
        getline(ss >> ws, text);  // rest of line is the comment text

        Post* p = getPost(postID);
        if (p == nullptr) continue;

        // resolve authorID to Entity*
        Entity* author = nullptr;
        for (int i = 0; i < userCount; i++) {
            if (users[i]->getID() == authorID) {
                author = users[i];
                break;
            }
        }
        if (author == nullptr) {
            for (int i = 0; i < pageCount; i++) {
                if (pages[i]->getID() == authorID) {
                    author = pages[i];
                    break;
                }
            }
        }

        p->addComment(author, text);
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
    return posts[i];
}

PostManager::~PostManager() {
    for (int i = 0; i < postCount; i++) {
        delete posts[i];
        posts[i] = nullptr;
    }
    delete[] posts;
}