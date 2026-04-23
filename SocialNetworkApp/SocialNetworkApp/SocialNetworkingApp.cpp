#pragma once
#include "User.h"
#include "Page.h"
#include "PostManager.h"
#include "Date.h"

class SocialNetworkApp {
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
    SocialNetworkApp();

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

    // Routed to PostManager (Partner A owns implementation)
    void likePost();
    void commentOnPost();
    void viewPost() const;
    void viewLikedList() const;

    void Run();             // Main loop
    ~SocialNetworkApp();
};