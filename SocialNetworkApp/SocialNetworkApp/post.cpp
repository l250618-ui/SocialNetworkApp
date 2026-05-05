#include "Post.h"
#include "Entity.h"
#include <iostream>
using namespace std;

Post::Post() {
    id = "";
    description = "";
    likeCount = 0;
    commentCount = 0;
    sharedBy = nullptr;
    
    likedBy = new Entity*[MAX_LIKES];
    for (int i = 0; i < MAX_LIKES; i++) {
        likedBy[i] = nullptr;
    }
    
    comments = new Comment*[MAX_COMMENTS];
    for (int i = 0; i < MAX_COMMENTS; i++) {
        comments[i] = nullptr;
    }
}

Post::Post(string id, string desc, Date date, Entity* sharedBy) {
    this->id = id;
    this->description = desc;
    this->sharedDate = date;
    this->sharedBy = sharedBy;
    this->likeCount = 0;
    this->commentCount = 0;
    
    likedBy = new Entity*[MAX_LIKES];
    for (int i = 0; i < MAX_LIKES; i++) {
        likedBy[i] = nullptr;
    }
    
    comments = new Comment*[MAX_COMMENTS];
    for (int i = 0; i < MAX_COMMENTS; i++) {
        comments[i] = nullptr;
    }
}

string Post::getID() const {
    return id;
}

string Post::getDescription() const {
    return description;
}

Date Post::getDate() const {
    return sharedDate;
}

void Post::setSharedByID(string id) {
    sharedByID = id;
}

Entity* Post::getSharedBy() const {
    return sharedBy;
}

int Post::getLikeCount() const {
    return likeCount;
}

int Post::getCommentCount() const {
    return commentCount;
}

Entity** Post::getLikedBy() const {
    return likedBy;
}

bool Post::likePost(Entity* user) {
    if (likeCount >= MAX_LIKES) {
        cout << "Maximum likes reached!" << endl;
        return false;
    }
    
    // check if user already liked this post
    for (int i = 0; i < likeCount; i++) {
        if (likedBy[i] == user) {
            cout << "You already liked this post!" << endl;
            return false;
        }
    }
    
    likedBy[likeCount] = user;
    likeCount++;
    return true;
}

void Post::addComment(Entity* author, string text) {
    if (commentCount >= MAX_COMMENTS) {
        cout << "Maximum comments reached!" << endl;
        return;
    }
    
    comments[commentCount] = new Comment(author, text);
    commentCount++;
}

void Post::displayLikedBy() const {
    cout << "Post Liked By:" << endl;
    for (int i = 0; i < likeCount; i++) {
        if (likedBy[i] != nullptr) {
            cout << likedBy[i]->getID() << " - " 
                 << likedBy[i]->getName() << endl;
        }
    }
}

void Post::displayComments() const {
    for (int i = 0; i < commentCount; i++) {
        if (comments[i] != nullptr) {
            comments[i]->display();
        }
    }
}

void Post::display() const {
    if (sharedBy != nullptr)
        cout << "--- " << sharedBy->getName() << " shared \"" << description << "\"" << endl;
    else
        cout << "--- " << description << endl;
    displayComments();
}

Post::~Post() {
    // delete each comment object
    for (int i = 0; i < commentCount; i++) {
        delete comments[i];
        comments[i] = nullptr;
    }
    delete[] comments;
    
    // do NOT delete the Entity objects inside likedBy
    // they are owned by the app, not by Post
    delete[] likedBy;
}

string Post::getSharedByID() const {
    return sharedByID;
}

void Post::setSharedBy(Entity* e) {
    sharedBy = e;
}