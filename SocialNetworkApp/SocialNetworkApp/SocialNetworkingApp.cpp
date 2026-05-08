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

SocialNetworkingApp::SocialNetworkingApp() { // default constructor which initializes whole system
    users = new User * [MAX_USERS]; // creates dynamic array of user pointers
    for (int i = 0; i < MAX_USERS; i++) 
        users[i] = nullptr; // initializes all users to null
    userCount = 0;
    pages = new Page * [MAX_PAGES]; // creates dynamic array of page pointers
    for (int i = 0; i < MAX_PAGES; i++) 
        pages[i] = nullptr; // initializes all users to null
    pageCount = 0;
    
    currentUser = nullptr; // no user logged in yet
    postManager = new PostManager(); // creates postmanager
    // handles all posts likes comments
}

SocialNetworkingApp::~SocialNetworkingApp() {
    for (int i = 0; i < userCount; i++) 
        delete users[i]; // deletes user objects
    for (int i = 0; i < pageCount; i++) 
        delete pages[i]; // deletes page objects
    // deletes arrays
    delete[] users;
    delete[] pages;
    delete postManager; // deletes post manager
}

void SocialNetworkingApp::loadPages() {
    ifstream fin("Pages.txt");
    if (!fin.is_open()) { // if file isnt found, execution is stopped
        cout << "Error: Pages.txt not found." << endl;
        return;
    }
    string line; // temporary variable to store each line from file
    getline(fin, line); // skip count on first line
    while (getline(fin, line)) { // read line by line
        if (line.empty() || line == "\r")
            continue;
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        istringstream ss(line);
        string id, name;
        ss >> id;
        getline(ss, name); // after extracting id, read the rest
        while (!name.empty() && name[0] == ' ') // remove leading spaces
            name.erase(0, 1);
        pages[pageCount++] = new Page(id, name); // new page object
    }

    fin.close();
}

void SocialNetworkingApp::loadUsers() {
    string line;
    // PASS 1
    // create User objects (ID + name only)
    // Stops reading name when -1 OR another ID starts(u smthng or p smthng)
    ifstream fin("Users.txt");
    if (!fin.is_open()) { // if file isnt found, execution is stopped
        cout << "Error: Users.txt not found." << endl;
        return;
    }
    getline(fin, line); // skip count on first line
    while (getline(fin, line)) { // read line by line
        if (line.empty() || line == "\r") // if line is empty or has unwanted hidden \r we skip
            continue;
        if (!line.empty() && line.back() == '\r') 
            line.pop_back(); // if line isnt empty but end of line, theres an unwanted hidden r, we remove it as if affects comparisons and such stuff
        istringstream ss(line);
        //istringstream is the input stream where ss is the object and line has the data that is turned into input stream
        string id, token, name = ""; // token is temporary word holder
        ss >> id; // we extract id
        while (ss >> token) { // rest of the line is read word by word and each word gets stored into token
            if (token == "-1" || ((token[0] == 'u' || token[0] == 'p') && token.size() > 1 && isdigit(token[1]))) 
                // if token is -1, starts w user id or page id AND it has greater than one character AND that second character is a digit
                break; // we stop
            if (!name.empty()) 
                name += " "; // if name is not empty, we create a full name w spaces
            name += token; // otherwise we store it
        }
        users[userCount++] = new User(id, name); // user object is created
    }
    fin.close();

    // PASS 2
    // link friends and liked pages to user
    // Skip name again, Read friend IDs until - 1, Add each friend, Then read page IDs until - 1 and lastly Add liked pages
    ifstream fin2("Users.txt");
    if (!fin2.is_open()) { // if file isnt found, execution is stopped
        cout << "Error: Users.txt not found." << endl;
        return;
    }
    getline(fin2, line); // count line so ignored
    int i = 0; // this is the user index which is used to track which user we are processing
    while (getline(fin2, line)) { // read line by line
        if (line.empty() || line == "\r") // if line is empty or has unwanted r, we skip
            continue;
        if (!line.empty() && line.back() == '\r') // if line isnt empty and has unwanted r
            line.pop_back(); // we remove the r
        istringstream ss(line); //input stream with object ss and line which has data that is converted into stream
        string id, token;
        // token holds words temporarily
        ss >> id; // id is extracted
        while (ss >> token) { // rest of the words are extracted
            if (token == "-1" || ((token[0] == 'u' || token[0] == 'p') && token.size() > 1 && isdigit(token[1]))) 
                break; // if these conditions are met, we stop
            // skipping the name section
        }
        while (token != "-1") { 
            User* f = findUser(token); // finds actual user object
            if (f)  // if found
                users[i]->addFriend(f); // links them
            ss >> token;
        }
        while (ss >> token && token != "-1") {
            Page* p = findPage(token); // finds actual user page
            if (p) 
                users[i]->addLikedPage(p); // if found, links them
        }
        i++; // we move onto next user
    }
    fin2.close();
}

// Gets all posts from PostManage. For each post: checks who shared it
// if User → add post to user
// if Page → add post to page
// This connects posts to their owners
void SocialNetworkingApp::linkPostEntities() {
    Post** allPosts = postManager->getAllPosts(); // array of pointers to all posts
    int count = postManager->getPostCount(); // total number of posts
    for (int i = 0; i < count; i++) { // we loop through all posts and get current post p
        Post* p = allPosts[i];
        if (p == nullptr) 
            continue;
        Entity* sharedBy = p->getSharedBy(); // we get who shared the post
        // it returns a generic pointer as it could be user or page
        if (sharedBy == nullptr) 
            continue;
        // dynamic cast checks the REAL type of the object at runtime.
        // It checks ke is this Entity actually a User or not
        // If yes then returns valid pointer
        // If no then returns nullptr
        User* u = dynamic_cast<User*>(sharedBy);
        if (u) {
            u->addPost(p); // we add post to user
        }
        else {
            Page* pg = dynamic_cast<Page*>(sharedBy);
            if (pg) 
                pg->addPost(p); // we add post to page
        }
    }
}

User* SocialNetworkingApp::findUser(string id) const {
    for (int i = 0; i < userCount; i++)
        if (users[i]->getID() == id) 
            return users[i];
    return nullptr;
}

Page* SocialNetworkingApp::findPage(string id) const {
    for (int i = 0; i < pageCount; i++)
        if (pages[i]->getID() == id) 
            return pages[i];
    return nullptr;
}

void SocialNetworkingApp::setDate(int d, int m, int y) {
    currentDate.setDate(d, m, y);
}

void SocialNetworkingApp::setCurrentUser() {
    string id;
    while (true) {
        cout << "Enter user ID: ";
        cin >> id;
        for (int i = 0; i < id.length(); i++)
            id[i] = tolower(id[i]);
        currentUser = findUser(id);
        if (currentUser) {
            cout << currentUser->getName() << " successfully set as Current User" << endl;
            break;
        }
        cout << "User not found. Please try again." << endl;
    }
}

void SocialNetworkingApp::viewHome() const {
    cout << "-----------------------------------------------" << endl;
    cout << "          " << currentUser->getName() << "- Home Page" << endl;
    cout << "-----------------------------------------------" << endl;
    Post** allPosts = postManager->getAllPosts(); // gets all the posts in the system
    int count = postManager->getPostCount();
    User** friends = currentUser->getFriends(); // list of friends
    int friendCount = currentUser->getFriendCount();
    Page** likedPages = currentUser->getLikedPages(); // pages user likes
    int likedPageCount = currentUser->getLikedPageCount();
    // loops thro all posts
    for (int i = 0; i < count; i++) { // skips if null or not within 24 hrs
        Post* p = allPosts[i];
        if (p == nullptr) 
            continue;
        if (!p->getDate().isWithin24Hours(currentDate)) 
            continue;
        // finds who posted it which could be a user or a page
        Entity* sharedBy = p->getSharedBy();
        if (sharedBy == nullptr) 
            continue;
        for (int j = 0; j < friendCount; j++) {
            if (friends[j] == sharedBy) { // checks if post owner is in friend list
                p->display(); // if it is, display the post
                goto next; // this basically skips everything once post is found and moves to next  
            }
        }
        for (int j = 0; j < likedPageCount; j++) {
            if (likedPages[j] == sharedBy) {
                p->display();
                goto next;
            }
        }
    next:; // this is a label
        // it does nothing
        // goto next js jumps here
        // kinda a bookmark
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
    if (p) 
        p->display();
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
        if (p->getDate().getYear() >= currentDate.getYear())
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
    string id = "m" + to_string(postManager->getPostCount() + 1); // concatenates strings by making integer a string
    string desc;
    cin.ignore();
    cout << "Enter description: ";
    getline(cin, desc);
    Memory* m = new Memory(id, desc, currentDate, currentUser, original);
    postManager->addPost(m);
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
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

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

