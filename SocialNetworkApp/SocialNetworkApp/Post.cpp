#pragma once
#include <string>
#include "Date.h"
#include "Comment.h"
using namespace std;

class Entity;

const int MAX_COMMENTS = 10;
const int MAX_LIKES = 10;

class Post {
protected:
    string id;
    string description;
    Date sharedDate;
    int likeCount;
    Entity** likedBy;
    int commentCount;
    Comment** comments;
    Entity* sharedBy;
public:
    Post();
    Post(string id, string desc, Date date, Entity* sharedBy);
    string getID() const;
    string getDescription() const;
    Date getDate() const;
    Entity* getSharedBy() const;
    bool likePost(Entity* user);
    void addComment(Entity* author, string text);
    void displayLikedBy() const;
    void displayComments() const;
    int getLikeCount() const;
    int getCommentCount() const;
    Entity** getLikedBy() const;
    virtual void display() const;
    virtual ~Post();
};