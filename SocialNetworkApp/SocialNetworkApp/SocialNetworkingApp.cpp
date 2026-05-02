#include "User.h"
#include "ActivityPost.h"
#include "Page.h"
#include "Memory.h"
#include "PostManager.h"
#include "Date.h"
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
	string id, name;
	while (getline(fin, id)) {
		getline(fin, name);
		pages[pageCount] = new Page(id, name);
		pageCount++;
	}
	fin.close();
}

void SocialNetworkingApp::loadUsers() {
	// PASS 1
	ifstream fin("Users.txt");
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
	cout << "Enter user ID: ";
	cin.ignore();
	getline(cin, id);
	currentUser = findUser(id);
}
void SocialNetworkingApp::viewHome() const {
	Post** timeline = currentUser->getTimeline();
	int count = currentUser->getTimelineCount();
	for (int i = 0; i < count; i++) {
		if (timeline[i]->getDate().isWithin24Hours(currentDate)) {
			timeline[i]->display();
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
    Post** allPosts = postManager->getAllPosts();
    int count = postManager->getPostCount();
    bool found = false;

    for (int i = 0; i < count; i++) {
        Memory* m = dynamic_cast<Memory*>(allPosts[i]);
        if (m != nullptr && m->getSharedBy() == currentUser) {
            m->display();
            found = true;
        }
    }

    if (!found) {
        cout << "No memories found." << endl;
    }
}

void SocialNetworkingApp::shareMemory() {string postID;
    cout << "Enter post ID to share as memory: ";
    cin >> postID;

    Post* original = postManager->getPost(postID);
    if (original == nullptr) {
        cout << "Post not found." << endl;
        return;
    }

    string id, desc;
    cout << "Enter memory ID: ";
    cin >> id;
    cin.ignore();
    cout << "Enter description: ";
    getline(cin, desc);

    Memory* m = new Memory(id, desc, currentDate, currentUser, original);
    currentUser->addPost(m);
    cout << "Memory shared!" << endl;
	// implement with Partner A
}


void SocialNetworkingApp::Run() {
	loadPages();
	loadUsers();
	postManager->loadPosts();
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
