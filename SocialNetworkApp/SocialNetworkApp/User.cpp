#include "Entity.h"
#include "Page.h"
#include "Post.h"
#include "User.h"
#include <iostream>
using namespace std;

User::User() : Entity(), friends(nullptr), friendCount(0), likedPages(nullptr), likedPageCount(0), timeline(nullptr), timelineCount(0) {
	friends = new User * [MAX_FRIENDS];
	likedPages = new Page * [MAX_LIKED_PAGES];
	timeline = new Post * [MAX_TIMELINE];
}

User::User(string id, string name) : Entity(id, name), friends(nullptr), friendCount(0), likedPages(nullptr), likedPageCount(0), timeline(nullptr), timelineCount(0) {
	friends = new User * [MAX_FRIENDS];
	likedPages = new Page * [MAX_LIKED_PAGES];
	timeline = new Post * [MAX_TIMELINE];
}
void User::addFriend(User* u) {
	if (friendCount < MAX_FRIENDS) {
		friends[friendCount] = u;
		friendCount++;
	}
}
void User::addLikedPage(Page* p) {
	if (likedPageCount < MAX_LIKED_PAGES) {
		likedPages[likedPageCount] = p;
		likedPageCount++;
	}
}
void User::addPost(Post* post) {
	if (timelineCount < MAX_TIMELINE) {
		timeline[timelineCount] = post;
		timelineCount++;
	}
}

User** User::getFriends() const {
	return friends;
}
int User::getFriendCount() const {
	return friendCount;
}
Page** User::getLikedPages() const {
	return likedPages;
}
int User::getLikedPageCount() const {
	return likedPageCount;
}
Post** User::getTimeline() const {
	return timeline;
}
int User::getTimelineCount() const {
	return timelineCount;
}

void User::displayFriends() const {
	cout << getName() << " - Friend List" << endl;
	for (int i = 0; i < friendCount; i++) {
		cout << friends[i]->getID() << " - " << friends[i]->getName() << endl;
	}
}

void User::displayLikedPages() const {
	cout << getName() << " - Liked Pages" << endl;
	for (int i = 0; i < likedPageCount; i++) {
		cout << likedPages[i]->getID() << " - " << likedPages[i]->getName() << endl;
	}
}
void User::displayTimeline() const {
	for (int i = 0; i < timelineCount; i++) {
		timeline[i]->display();
	}
}
void User::display() const {
	cout << getName() << " - Time Line" << endl;
	displayTimeline();
}

User::~User() {
	delete[] friends;
	delete[] likedPages;
	delete[] timeline;
}