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

void PostManager::loadPosts(User** users, int userCount, Page** pages, int pageCount) {
    ifstream file("Posts.txt");
    string line;
    getline(file, line); // skip count line "12"

    while (getline(file, line)) {
        // trim \r
        if (!line.empty() && line.back() == '\r') line.pop_back();
        // skip blank lines
        if (line.empty()) continue;

        // line1: type and id
        istringstream ss1(line);
        int type;
        string id;
        ss1 >> type >> id;

        // line2: date
        getline(file, line);
        if (!line.empty() && line.back() == '\r') line.pop_back();
        istringstream ss2(line);
        int day, month, year;
        ss2 >> day >> month >> year;
        Date date(day, month, year);

        // line3: description
        getline(file, line);
        if (!line.empty() && line.back() == '\r') line.pop_back();
        string desc = line;

        // line4: actType + actValue (only if type == 2)
        int actType = 0;
        string actValue = "";
        if (type == 2) {
            getline(file, line);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            istringstream ss3(line);
            ss3 >> actType;
            getline(ss3, actValue);
            size_t start = actValue.find_first_not_of(" \t");
            if (start != string::npos) actValue = actValue.substr(start);
        }

        // line5: sharedByID
        getline(file, line);
        if (!line.empty() && line.back() == '\r') line.pop_back();
        string sharedByID = line;

        // line6: likers until -1
        getline(file, line);
        if (!line.empty() && line.back() == '\r') line.pop_back();

        // find owner
        Entity* owner = nullptr;
        for (int i = 0; i < userCount; i++)
            if (users[i]->getID() == sharedByID) { owner = users[i]; break; }
        if (!owner)
            for (int i = 0; i < pageCount; i++)
                if (pages[i]->getID() == sharedByID) { owner = pages[i]; break; }

        // create post
        Post* p = nullptr;
        if (type == 1) {
            p = new Post(id, desc, date, owner);
        }
        else if (type == 2) {
            p = new ActivityPost(id, desc, date, owner, actType, actValue);
        }

        // parse likers line
        if (p != nullptr) {
            istringstream ssL(line);
            string liker;
            while (ssL >> liker && liker != "-1") {
                Entity* e = nullptr;
                for (int i = 0; i < userCount; i++)
                    if (users[i]->getID() == liker) { e = users[i]; break; }
                if (!e)
                    for (int i = 0; i < pageCount; i++)
                        if (pages[i]->getID() == liker) { e = pages[i]; break; }
                if (e) p->likePost(e);
            }
            posts[postCount++] = p;
        }
    }
    file.close();
}

void PostManager::loadComments(User** users, int userCount, Page** pages, int pageCount) {
    ifstream file("Comments.txt");
    string line;
    getline(file, line); // skip count line "13"

    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        istringstream ss(line);
        string cid, postID, authorID, text;
        ss >> cid >> postID >> authorID;
        getline(ss, text);
        size_t start = text.find_first_not_of(" \t");
        if (start != string::npos) text = text.substr(start);

        Post* p = getPost(postID);
        if (p != nullptr) {
            Entity* author = nullptr;
            for (int i = 0; i < userCount; i++)
                if (users[i]->getID() == authorID) { author = users[i]; break; }
            if (!author)
                for (int i = 0; i < pageCount; i++)
                    if (pages[i]->getID() == authorID) { author = pages[i]; break; }
            if (author) p->addComment(author, text);
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