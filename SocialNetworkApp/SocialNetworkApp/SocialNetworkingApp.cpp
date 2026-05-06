#include "User.h"
#include "ActivityPost.h"
#include "Page.h"
#include "Memory.h"
#include "PostManager.h"
#include "Date.h"
#include "Post.h"
#include "SocialNetworkingApp.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

SocialNetworkingApp::SocialNetworkingApp() {
    users = new User * [MAX_USERS];
    for (int i = 0; i < MAX_USERS; i++) users[i] = nullptr;
    userCount = 0;

    pages = new Page * [MAX_PAGES];
    for (int i = 0; i < MAX_PAGES; i++) pages[i] = nullptr;
    pageCount = 0;

    currentUser = nullptr;
    postManager = new PostManager();
}

SocialNetworkingApp::~SocialNetworkingApp() {
    for (int i = 0; i < userCount; i++) delete users[i];
    for (int i = 0; i < pageCount; i++) delete pages[i];
    delete[] users;
    delete[] pages;
    delete postManager;
}

void SocialNetworkingApp::loadPages() {
    ifstream fin("Pages.txt");
    string line;
    getline(fin, line); 
    while (getline(fin, line)) {
        if (line.empty() || line == "\r") continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        istringstream ss(line);
        string id, name;
        ss >> id;
        getline(ss, name);
        size_t start = name.find_first_not_of(" \t");
        if (start != string::npos) name = name.substr(start);
        pages[pageCount++] = new Page(id, name);
    }
    fin.close();
}

void SocialNetworkingApp::loadUsers() {
    string line;

    // PASS 1, create User objects
    ifstream fin("Users.txt");
    getline(fin, line); // skip count
    while (getline(fin, line)) {
        if (line.empty() || line == "\r") continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        istringstream ss(line);
        string id, token, name = "";
        ss >> id;
        while (ss >> token) {
            if (token == "-1" || token[0] == 'u' || token[0] == 'p') break;
            if (!name.empty()) name += " ";
            name += token;
        }
        users[userCount++] = new User(id, name);
    }
    fin.close();

    // PASS 2, link friends and liked pages
    ifstream fin2("Users.txt");
    getline(fin2, line); 
    int i = 0;
    while (getline(fin2, line)) {
        if (line.empty() || line == "\r") continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        istringstream ss(line);
        string id, token;
        ss >> id;
        while (ss >> token) {
            if (token == "-1" || token[0] == 'u' || token[0] == 'p') break;
        }
        while (token != "-1") {
            User* f = findUser(token);
            if (f) users[i]->addFriend(f);
            ss >> token;
        }
        while (ss >> token && token != "-1") {
            Page* p = findPage(token);
            if (p) users[i]->addLikedPage(p);
        }
        i++;
    }
    fin2.close();
}

void SocialNetworkingApp::linkPostEntities() {
    Post** allPosts = postManager->getAllPosts();
    int count = postManager->getPostCount();
    for (int i = 0; i < count; i++) {
        Post* p = allPosts[i];
        if (p == nullptr) continue;
        Entity* sharedBy = p->getSharedBy();
        if (sharedBy == nullptr) continue;
        User* u = dynamic_cast<User*>(sharedBy);
        if (u) {
            u->addPost(p);
        }
        else {
            Page* pg = dynamic_cast<Page*>(sharedBy);
            if (pg) pg->addPost(p);
        }
    }
}

User* SocialNetworkingApp::findUser(string id) const {
    for (int i = 0; i < userCount; i++)
        if (users[i]->getID() == id) return users[i];
    return nullptr;
}

Page* SocialNetworkingApp::findPage(string id) const {
    for (int i = 0; i < pageCount; i++)
        if (pages[i]->getID() == id) return pages[i];
    return nullptr;
}

void SocialNetworkingApp::setDate(int d, int m, int y) {
    currentDate.setDate(d, m, y);
}

void SocialNetworkingApp::setCurrentUser() {
    string id;
    cout << "Enter user ID: ";
    cin >> id;
    currentUser = findUser(id);
    if (currentUser)
        cout << currentUser->getName() << " successfully set as Current User" << endl;
    else
        cout << "User not found." << endl;
}

void SocialNetworkingApp::viewHome() const {
    cout << "-----------------------------------------------" << endl;
    cout << currentUser->getName() << "                     - Home Page" << endl;
    cout << "-----------------------------------------------" << endl;

    Post** allPosts = postManager->getAllPosts();
    int count = postManager->getPostCount();

    User** friends = currentUser->getFriends();
    int friendCount = currentUser->getFriendCount();

    Page** likedPages = currentUser->getLikedPages();
    int likedPageCount = currentUser->getLikedPageCount();

    for (int i = 0; i < count; i++) {
        Post* p = allPosts[i];
        if (p == nullptr) continue;
        if (!p->getDate().isWithin24Hours(currentDate)) continue;

        Entity* sharedBy = p->getSharedBy();
        if (sharedBy == nullptr) continue;

        
        for (int j = 0; j < friendCount; j++) {
            if (friends[j] == sharedBy) {
                p->display();
                goto next; 
            }
        }
        
        for (int j = 0; j < likedPageCount; j++) {
            if (likedPages[j] == sharedBy) {
                p->display();
                goto next;
            }
        }
    next:;
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
    if (p) p->display();
    else cout << "Page not found." << endl;
}

void SocialNetworkingApp::seeYourMemories() const {
    cout << "We hope you enjoy looking back and sharing your memories." << endl;

    Post** allPosts = postManager->getAllPosts();
    int count = postManager->getPostCount();
    bool found = false;

    
    for (int i = 0; i < count; i++) {
        Post* p = allPosts[i];
        if (p == nullptr) 
            continue;
        if (p->getSharedBy() != currentUser) 
            continue;
        if (!p->getDate().isSameDayAndMonth(currentDate)) 
            continue;
        if (p->getDate().getYear() == currentDate.getYear()) 
            continue; 

        int yearsAgo = currentDate.getYear() - p->getDate().getYear();
        cout << "On this Day" << endl;
        cout << yearsAgo << " Years Ago" << endl;
        p->display();
        found = true;
    }

    if (!found) cout << "No memories found for today." << endl;
}


void SocialNetworkingApp::shareMemory() {
    string postID;
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
    postManager->addPost(m);
    currentUser->addPost(m); // FIX 6: was missing — memory must appear on user's timeline
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
    cout << "Enter post ID: ";
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
    postManager->loadComments(users, userCount, pages, pageCount);
    linkPostEntities();      
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
        cout << "8. Like a Post\n";
        cout << "9. Comment on a Post\n";
        cout << "10. View a Post\n";
        cout << "11. View Liked List\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:  viewHome(); break;
        case 2:  viewFriendList(); break;
        case 3:  viewLikedPages(); break;
        case 4:  viewProfile(); break;
        case 5:  viewPage(); break;
        case 6:  seeYourMemories(); break;
        case 7:  shareMemory(); break;
        case 8:  likePost(); break;
        case 9:  commentOnPost(); break;
        case 10: viewPost(); break;
        case 11: viewLikedList(); break;
        case 0:  cout << "Goodbye!\n"; break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

