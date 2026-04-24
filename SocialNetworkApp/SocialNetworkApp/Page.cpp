#include "Post.h"
#include "Entity.h"
#include "User.h"
#include "Page.h"
#include <iostream>
using namespace std;

Page::Page() : Entity(), owner(nullptr), posts(nullptr), postCount(0), likeCount(0) {
	posts = new Post * [MAX_PAGE_POSTS];
}

Page::Page(string id, string name, User* owner) : Entity(id, name), owner(owner), posts(nullptr), postCount(0), likeCount(0) {
	posts = new Post * [MAX_PAGE_POSTS];
}

void Page::addPost(Post* post) {
	if (postCount < MAX_PAGE_POSTS) {
		posts[postCount] = post;
		postCount++;
	}
}
Post** Page::getPosts() const {
	return posts;
}
int Page::getPostCount() const {
	return postCount;
}
int Page::getLikeCount() const {
	return likeCount;
}
void Page::incrementLikes() {
	likeCount++;
}
void Page::displayPosts() const {
	for (int i = 0; i < postCount; i++) {
		posts[i]->display();
	}
}
void Page::display() const {
	cout << "Page: " << getName() << " (ID: " << getID() << ")" << endl;
	displayPosts();
}

Page::~Page() {
	delete[] posts;
}