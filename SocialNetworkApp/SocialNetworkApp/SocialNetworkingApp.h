#pragma once
#include "User.h"
#include "Page.h"
#include "PostManager.h"
#include "Date.h"

const int MAX_USERS = 20;
const int MAX_PAGES = 12;

class SocialNetworkingApp {
    User** users;
    int userCount;
    Page** pages;
    int pageCount;
    User* currentUser;
    Date currentDate;
    PostManager* postManager;

    // Private file loaders (Partner B)
    void loadUsers();       // 2-pass
    void loadPages();

    // Private helpers
    User* findUser(string id) const;
    Page* findPage(string id) const;

public:
    SocialNetworkingApp();

    // Partner B
    void setDate(int d, int m, int y);
    void setCurrentUser();
    void viewHome() const;
    void viewFriendList() const;
    void viewLikedPages() const;
    void viewProfile() const;
    void viewPage() const;
    void seeYourMemories() const;
    void shareMemory();
    void linkPostEntities();

    // Routed to PostManager (Partner A owns implementation)
    void likePost();
    void commentOnPost();
    void viewPost() const;
    void viewLikedList() const;

    void Run();             // Main loop
    ~SocialNetworkingApp();
};