#include "User.h"
#include "ActivityPost.h"
#include "Page.h"
#include "Memory.h"
#include "PostManager.h"
#include "Date.h"
#include "Post.h"
#include "SocialNetworkingApp.h"
#include "Memory.h"
#include "Comment.h"
#include "Post.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

SocialNetworkingApp::SocialNetworkingApp() {
	users = new User * [MAX_USERS];
	for (int i = 0; i < MAX_USERS; i++) {
		users[i] = nullptr;
	}
	userCount = 0; 
	pages = new Page * [MAX_PAGES];
	for (int i = 0; i < MAX_PAGES; i++) {
		pages[i] = nullptr;
	}
	pageCount = 0;
	currentUser = nullptr;
	postManager = new PostManager();
}

SocialNetworkingApp::~SocialNetworkingApp() {
	for (int i = 0; i < MAX_USERS; i++) {
		delete users[i];
	}
	for (int i = 0; i < MAX_PAGES; i++) {
		delete pages[i];
	}
	delete[] users;
	delete[] pages;
	delete postManager; 
}

void SocialNetworkingApp::loadPages() {
	ifstream fin("Pages.txt");
	int count;
	fin >> count;
	fin.ignore();
	for (int i = 0; i < count; i++) {
		string line;
		getline(fin, line);
		if (!line.empty() && line.back() == '\r') line.pop_back();
		istringstream ss(line);
		string id, name;
		ss >> id;
		getline(ss >> ws, name);
		pages[pageCount++] = new Page(id, name);
	}
	fin.close();
}

void SocialNetworkingApp::loadUsers() {
	// PASS 1
	ifstream fin("Users.txt");
	int count;
	fin >> count;
	fin.ignore(); // skip rest of that line
	string line;
	while (getline(fin, line)) {
		if (!line.empty() && line.back() == '\r') line.pop_back();
		istringstream ss(line);
		string id, name1, name2;
		ss >> id >> name1 >> name2;
		string name = name1 + " " + name2;
		users[userCount] = new User(id, name);
		userCount++;
	}
	fin.close();

	// PASS 2
	ifstream fin2("Users.txt");
	int i = 0;
	while (getline(fin2, line)) {
		if (!line.empty() && line.back() == '\r') line.pop_back();
		istringstream ss(line);
		string id, name1, name2, token;
		ss >> id >> name1 >> name2;

		// friends until -1
		while (ss >> token && token != "-1") {
			users[i]->addFriend(findUser(token));
		}

		// pages until -1
		while (ss >> token && token != "-1") {
			users[i]->addLikedPage(findPage(token));
		}

		i++;
	}
	fin2.close();
}


User* SocialNetworkingApp::findUser(string id) const {
	for (int i = 0; i < userCount; i++) {
		if (users[i]->getID() == id) {
			return users[i];
		}
	}
	return nullptr;
}
Page* SocialNetworkingApp::findPage(string id) const {
	for (int i = 0; i < pageCount; i++) {
		if (pages[i]->getID() == id) {
			return pages[i];
		}
	}
	return nullptr;
}


void SocialNetworkingApp::setDate(int d, int m, int y) {
	currentDate.setDate(d, m, y);
}
void SocialNetworkingApp::setCurrentUser() {
	string id;
	cout << "Enter user ID (e.g. u7): ";
	cin >> id;
	currentUser = findUser(id);  
	if (currentUser == nullptr) {
		cout << "User not found! Defaulting to u7 for testing." << endl;
		currentUser = findUser("u7");  
	}
	else {
		cout << currentUser->getName() << " set as current user." << endl;
	}
}

void SocialNetworkingApp::viewHome() const {
	cout << currentUser->getName() << " -- Home Page" << endl;

	// friends' posts
	User** friends = currentUser->getFriends();
	int friendCount = currentUser->getFriendCount();
	for (int f = 0; f < friendCount; f++) {
		if (friends[f] == nullptr) continue;
		Post** ft = friends[f]->getTimeline();
		int fc = friends[f]->getTimelineCount();
		for (int i = 0; i < fc; i++) {
			if (ft[i]->getDate().isWithin24Hours(currentDate))
				ft[i]->display();
		}
	}

	// liked pages' posts
	Page** likedPages = currentUser->getLikedPages();
	int pageCount = currentUser->getLikedPageCount();
	for (int p = 0; p < pageCount; p++) {
		if (likedPages[p] == nullptr) continue;
		Post** pt = likedPages[p]->getPosts();
		int pc = likedPages[p]->getPostCount();
		for (int i = 0; i < pc; i++) {
			if (pt[i]->getDate().isWithin24Hours(currentDate))
				pt[i]->display();
		}
	}
}

	// friends' posts
	User** friends = currentUser->getFriends();
	int friendCount = currentUser->getFriendCount();
	for (int f = 0; f < friendCount; f++) {
		if (friends[f] == nullptr) continue;
		Post** ft = friends[f]->getTimeline();
		int fc = friends[f]->getTimelineCount();
		for (int i = 0; i < fc; i++) {
			if (ft[i]->getDate().isWithin24Hours(currentDate))
				ft[i]->display();
		}
	}
}
void SocialNetworkingApp::viewFriendList() const {
	currentUser->displayFriends();
}
void SocialNetworkingApp::viewLikedPages() const {
	currentUser->displayLikedPages();
}
void SocialNetworkingApp::viewProfile() const {
	currentUser->display();
}
void SocialNetworkingApp::viewPage() const {
	string id;
	cout << "Enter page ID: ";
	cin >> id;
	Page* p = findPage(id);
	p->display();
}

void SocialNetworkingApp::seeYourMemories() const {
	cout << "\n--- Your Memories ---" << endl;
	Post** allPosts = postManager->getAllPosts();
	int count = postManager->getPostCount();
	for (int i = 0; i < count; i++) {
		if (allPosts[i]->getSharedBy() == currentUser) {
			Date postDate = allPosts[i]->getDate();
			if (postDate.getDay() == currentDate.getDay() &&
				postDate.getMonth() == currentDate.getMonth() &&
				postDate.getYear() != currentDate.getYear()) {
				allPosts[i]->display();
			}
		}
	}
}

void SocialNetworkingApp::linkPostEntities() {
	for (int i = 0; i < postManager->getPostCount(); i++) {
		Post* p = postManager->getPostByIndex(i);
		string sharedByID = p->getSharedByID();
		if (sharedByID == "") continue;

		User* u = findUser(sharedByID);
		if (u != nullptr) {
			p->setSharedBy(u);
			u->addPost(p);
			continue;  // it's a user, skip page check
		}

		Page* pg = findPage(sharedByID);
		if (pg != nullptr) {
			p->setSharedBy(pg);
			pg->addPost(p);  // <-- this was missing
		}
	}
}

void SocialNetworkingApp::shareMemory() {
	string postID;
	cout << "Enter post ID to share as memory: ";
	cin >> postID;
	Post* p = postManager->getPost(postID);
	if (p == nullptr) {
		cout << "Post not found." << endl;
		return;
	}
	Memory* m = new Memory(p->getID(), p->getDescription(), currentDate, currentUser, p);
	currentUser->addPost(m);
	cout << "Memory shared!" << endl;
}

void SocialNetworkingApp::likePost() {
	string postID;
	cout << "Enter post ID to like: ";
	cin >> postID;
	postManager->likePost(postID, currentUser);
}

void SocialNetworkingApp::commentOnPost() {
	string postID, text;
	cout << "Enter post ID to comment on: ";
	cin >> postID;
	cin.ignore();
	cout << "Enter comment: ";
	getline(cin, text);
	postManager->commentOnPost(postID, currentUser, text);
}

void SocialNetworkingApp::viewPost() const {
	string postID;
	cout << "Enter post ID: ";
	cin >> postID;
	postManager->viewPost(postID);
}

void SocialNetworkingApp::viewLikedList() const {
	string postID;
	cout << "Enter post ID: ";
	cin >> postID;
	postManager->viewLikedList(postID);
}

void SocialNetworkingApp::Run() {
	loadPages();
	loadUsers();
	postManager->loadPosts(users, userCount, pages, pageCount);
	linkPostEntities();
	postManager->loadComments(users, userCount, pages, pageCount);
	setDate(15, 11, 2017);
	setCurrentUser();   

	int choice;
	do {
		cout << "\n--- Menu ---\n";
		cout << "1. View Home\n";
		cout << "2. View Friend List\n";
		cout << "3. View Liked Pages\n";
		cout << "4. View Profile\n";
		cout << "5. View Page\n";
		cout << "6. See Your Memories\n";
		cout << "7. Share Memory\n";
		cout << "0. Exit\n";
		cout << "Enter choice: ";
		cin >> choice;

		switch (choice) {
		case 1: viewHome(); break;
		case 2: viewFriendList(); break;
		case 3: viewLikedPages(); break;
		case 4: viewProfile(); break;
		case 5: viewPage(); break;
		case 6: seeYourMemories(); break;
		case 7: shareMemory(); break;
		case 0: cout << "Goodbye!\n"; break;
		default: cout << "Invalid choice.\n";
		}
	} while (choice != 0);
}
