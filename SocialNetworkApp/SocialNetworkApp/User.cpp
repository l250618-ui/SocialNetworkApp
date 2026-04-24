#pragma once
#include "Entity.h"
#include <string>
using namespace std;

class Post;
class Page;

const int MAX_FRIENDS = 20;
const int MAX_LIKED_PAGES = 20;
const int MAX_TIMELINE = 50;

class User : public Entity {
    User** friends;
    int friendCount;
    Page** likedPages;
    int likedPageCount;
    Post** timeline;
    int timelineCount;
public:
    User();
    User(string id, string name);
    void addFriend(User* u);
    void addLikedPage(Page* p);
    void addPost(Post* post);
    User** getFriends() const;
    int getFriendCount() const;
    Page** getLikedPages() const;
    int getLikedPageCount() const;
    Post** getTimeline() const;
    int getTimelineCount() const;
    void displayFriends() const;
    void displayLikedPages() const;
    void displayTimeline() const;
    void display() const override;
    ~User();
};