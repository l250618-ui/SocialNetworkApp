#include "PostManager.h"
#include "ActivityPost.h"
#include "Memory.h"
#include "Post.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "User.h"
#include "Page.h"
using namespace std;

PostManager::PostManager() {
    posts = new Post*[MAX_POSTS]; // dynamically creates an array of pointers to post objects
    for (int i = 0; i < MAX_POSTS; i++) {
        posts[i] = nullptr; // every object is initialized to nullptr
    }
    postCount = 0;
}

// it reads each post from the file and gets its details like ID, date, and description.
// ihen it creates the correct type of post and links it to the user or page who posted it.
// it also adds likes to each post.
// at the end, it connects memory posts to their original posts.
void PostManager::loadPosts(User** users, int userCount, Page** pages, int pageCount) {
    ifstream file("Posts.txt"); // file is opened for reading
    if (!file.is_open()) { // if file isnt found, execution is stopped
        cout << "Error: Posts.txt not found." << endl;
        return;
    }
    string line; // temporary variable to store each line from file
    getline(file, line); // skip first line which shows number of posts
    while (getline(file, line)) { // read file line by line till the end

        if (line.empty() || line == "\r") // this skips blank lines
            // line.empty checks if string has zero characters
            // \r checks if string is exactly one character long and 
            // tells the cursor to move back to the beginning of the current line without moving down to a new line
            continue; 

        // type + id is extracted
        istringstream ss1(line); // istringstream lets you treat a string like an input stream
        // ss1 is the variable name of the stream object
        // the stream is initialized with contents of the string line
        int type;
        string id;
        ss1 >> type >> id; // extracts values from stream
        // >> reads data separated by whitespace

        // date
        getline(file, line); // reads entire line from file
        while (line.empty() || line == "\r") 
            getline(file, line); // reads next non empty line

        // extracts date and creates date object
        istringstream ss2(line); // line is string variable where data is stored 
        int d, m, y;
        ss2 >> d >> m >> y; // extracts numbers 
        Date date(d, m, y);

        // description
        getline(file, line); // reads next line from file
        while (line.empty() || line == "\r") 
            getline(file, line); // checks if its empty and skips if it is
        string desc = line; // line is copied into desc

        // activity 
        int actType = 0;
        string actValue = ""; // these are default values
        if (type == 2) { // only runs for activity post
            // if type isnt 2 then activity section never runs and default values exist in that case
            getline(file, line); // reads line
            while (line.empty() || line == "\r") 
                getline(file, line); // checks if its empty and skips
            istringstream ss3(line); // istringstream lets you treat a string like an input stream
        // ss3 is the variable name of the stream object
        // line is string variable where data is stored
            ss3 >> actType; // extracts activity type number 
            getline(ss3, actValue); // extracts remaining text (activity value)
            // reads the rest of the stream to actvalue

            if (!actValue.empty() && actValue[0] == ' ') // removes leading space
                // checks if string is not empty and first character is a space 
                actValue = actValue.substr(1); // substr(1) means create a substring starting from index 1 so first blank space is skipped
        }

        // sharedBy
        // reads ID of user/page who posted
        getline(file, line); 
        while (line.empty() || line == "\r") 
            getline(file, line);
        string sharedByID = line;

        // next line (likers OR original ID)
        getline(file, line);
        while (line.empty() || line == "\r") 
            getline(file, line);
        string likersLine = line;

        // find owner
        Entity* owner = nullptr; // pointer to an entity object to store found user/page

        // search in users
        for (int i = 0; i < userCount; i++) // loop through all users
            if (users[i]->getID() == sharedByID) { // compare IDs
                // getID is current and sharedBy is the one being looked for
                owner = users[i]; // store pointer to the matching user
               // now owner points to that user object
                break; // user found so loop ends
            }

        // if not found, search in page
        if (!owner)
            for (int i = 0; i < pageCount; i++)
                if (pages[i]->getID() == sharedByID) {
                    owner = pages[i]; // save page pointer if found
                    break;
                }

        // post is skipped if no owner is found
        if (!owner) 
            continue;

        // post is created
        Post* p = nullptr;

        if (type == 1) { // creates normal post
            p = new Post(id, desc, date, owner);
        }
        else if (type == 2) { // creates activity post
            p = new ActivityPost(id, desc, date, owner, actType, actValue);
        }
        else if (type == 3) {
            // first line stored original ID
            string originalID = likersLine;

            // next line is actual likers
            getline(file, line); // reads line
            while (line.empty() || line == "\r") 
                getline(file, line); // if line is empty, we skip
            likersLine = line; // stores line in likers

            // finds og post
            Post* original = getPost(originalID);
            // creates memory post
            p = new Memory(id, desc, date, owner, original);
        }

        // likes
        if (p != nullptr) { // only if post was created
            istringstream ssL(likersLine);
            string liker; // prepare to read likers

            while (ssL >> liker && liker != "-1") { // loop until -1
                Entity* e = nullptr; // this is a pointer for liker

                for (int i = 0; i < userCount; i++) // we search for liker in user
                    if (users[i]->getID() == liker) {
                        e = users[i];
                        break;
                    }

                if (!e) // if we dont find in user, then in pages
                    // we search pages because IDs can belong to either users or pages, not just users
                    for (int i = 0; i < pageCount; i++)
                        if (pages[i]->getID() == liker) {
                            e = pages[i];
                            break;
                        }

                if (e) // if found, like is added to post
                    p->likePost(e);
            }

            if (postCount < MAX_POSTS)
                posts[postCount++] = p; // add post to array and increment count
        }
    }

    file.close();
}

void PostManager::loadComments(User** users, int userCount, Page** pages, int pageCount) {
    ifstream file("Comments.txt"); // opens file to read comments
    if (!file.is_open()) { // if file isnt found, execution is stopped
        cout << "Error: Comments.txt not found." << endl;
        return;
    }
    string line;// temporary variable to store each line from file
    getline(file, line); // skip first line which shows number of posts

    while (getline(file, line)) { // read line by line
        if (!line.empty() && line.back() == '\r') // hidden r can break comparisons and parsing
            // line empty checks if string is empty
            // line back returns last char of the string and checks whether last character is carriage return
            // \r unwanted hidden character from file
            line.pop_back(); // removes last character which is \r
        if (line.empty()) 
            continue;

        // extracts commentID, postID (which post this belongs to) and authorID (who wrote it)
        istringstream ss(line);
        string cid, postID, authorID, text;
        ss >> cid >> postID >> authorID;
        getline(ss, text); // reads remaining part of the line which is actual comment
        // removes leading spaces
        while (!text.empty() && text[0] == ' ') // checks that string isnt empty and the first character is blank
            text.erase(0, 1); // erase(start,count) so removes first character

        // finds post object using ID 
        Post* p = getPost(postID);
        if (p != nullptr) { // if post exists
            Entity* author = nullptr; // find author
            // this stores who wrote the comment 
            for (int i = 0; i < userCount; i++) // search in users
                if (users[i]->getID() == authorID) { 
                    author = users[i]; 
                    break; 
                }
            if (!author) // if author isnt found, search in pages
                // we search pages because IDs can belong to either users or pages, not just users
                for (int i = 0; i < pageCount; i++)
                    if (pages[i]->getID() == authorID) { 
                        author = pages[i]; 
                        break; 
                    }
            if (author)  // if author found, add comment to the post
                p->addComment(author, text);
        }
    }
    file.close();
}

// function searches for a post by its ID
Post* PostManager::getPost(string postID) const {
    for (int i = 0; i < postCount; i++) {
        if (posts[i] != nullptr && posts[i]->getID() == postID) { // making sure post exists and ID matches
            return posts[i];
        }
    }
    return nullptr; // if not found
}

bool PostManager::likePost(string postID, Entity* user) {
    Post* p = getPost(postID); // find post
    if (p == nullptr) {
        cout << "Post not found." << endl;
        return false;
    }
    return p->likePost(user); // call like function
}

// add posts
void PostManager::addPost(Post* p) {
    if (postCount < MAX_POSTS) { // check limit and add
        posts[postCount++] = p;
    }
}

bool PostManager::commentOnPost(string postID, Entity* author, string text) {
    Post* p = getPost(postID); // find post
    if (p == nullptr) {
        cout << "Post not found." << endl;
        return false;
    }
    p->addComment(author, text); // add comment
    return true;
}

void PostManager::viewPost(string postID) const {
    Post* p = getPost(postID); // find post
    if (p == nullptr) {
        cout << "Post not found." << endl;
        return;
    }
    p->display(); // show full post
}

void PostManager::viewLikedList(string postID) const {
    Post* p = getPost(postID); // find post
    if (p == nullptr) {
        cout << "Post not found." << endl;
        return;
    }
    p->displayLikedBy(); // view the people its liked by
}

int PostManager::getPostCount() const {
    return postCount; // returns total number of posts
}

Post** PostManager::getAllPosts() const {
    return posts; // returns full array of posts
}
//
Post* PostManager::getPostByIndex(int i) const {
    if (i >= 0 && i < postCount) // checks valid index
        return posts[i]; // returns
    return nullptr; // if invalid
}
PostManager::~PostManager() {
    for (int i = 0; i < postCount; i++) {
        delete posts[i]; // delete all post objects
        posts[i] = nullptr;
    }
    delete[] posts; // delete posts array
}