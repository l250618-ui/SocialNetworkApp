#include "Memory.h"
#include "Entity.h"
#include <iostream>
using namespace std;

Memory::Memory() {
    originalPost = nullptr;
}

Memory::Memory(string id, string desc, Date date, Entity* sharedBy, Post* original)
    : Post(id, desc, date, sharedBy) {
    originalPost = original;
}

Post* Memory::getOriginalPost() const {
    return originalPost;
}

void Memory::display() const {
    cout << "~~~ " << sharedBy->getName()
         << " shared a memory ~~~ "
         << "…(" << sharedDate.toString() << ")" << endl;

    cout << "\"" << description << "\"" << endl;

    if (originalPost != nullptr) {
        int yearsAgo = sharedDate.getYear() - originalPost->getDate().getYear();
        
        if (yearsAgo > 0) {
            cout << "(" << yearsAgo << " Years Ago)" << endl;
        }

        originalPost->display();
    }
}

Memory::~Memory() {
}