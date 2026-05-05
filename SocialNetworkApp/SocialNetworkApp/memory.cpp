// Memory.cpp
// so this file handles the Memory class which is a post that remembers old posts
// basically a Memory IS a Post but it also holds a pointer to the original post

#include "Memory.h"
#include "Entity.h"
#include <iostream>
using namespace std;

// default constructor just set originalPost to null so it doesnt point to garbage
Memory::Memory() {
    originalPost = nullptr; // always initialize pointers!!! learned this the hard way
}


// passes the base stuff to Post and then stores the original post separately
// sharedBy is who is resharing this memory, original is the old post being remembered
Memory::Memory(string id, string desc, Date date, Entity* sharedBy, Post* original)
    : Post(id, desc, date, sharedBy) { // calling base class constructor first
    originalPost = original;
}

// simple getter for the original post
// returning a pointer so caller can use it but not delete it
Post* Memory::getOriginalPost() const {
    return originalPost;
}

// display function
// shows who shared the memory and then shows the original post below it
// also calculates how many years ago the original post was made
void Memory::display() const {
    // print the memory header with name and date
    cout << "~~~ " << sharedBy->getName() << " shared a memory ~~~ " << "...(" << sharedDate.toString() << ")" << endl;

    // print the description of the memory post
    cout << "\"" << description << "\"" << endl;

    // only show original post info if it actually exists (null check is important)
    if (originalPost != nullptr) {
        // calculate difference in years between now and original post date
        int yearsAgo = sharedDate.getYear() - originalPost->getDate().getYear();
        
        // only display "X Years Ago" if its actually in the past
        if (yearsAgo > 0) {
            cout << "(" << yearsAgo << " Years Ago)" << endl;
        }

        // display the original post using polymorphism (virtual function)
        originalPost->display();
    }
}

// destructor - we dont delete originalPost here bc we dont own it
// whoever created it is responsible for deleting it (PostManager probably)
Memory::~Memory() {
}