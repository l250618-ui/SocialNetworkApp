#include "Comment.h"
#include "Entity.h"
#include <iostream>
using namespace std;

Comment::Comment() {
    author = nullptr;
    text = "";
}

Comment::Comment(Entity* author, string text) {
    this->author = author;
    this->text = text;
}

Entity* Comment::getAuthor() const {
    return author;
}

string Comment::getText() const {
    return text;
}

void Comment::display() const {
    if (author != nullptr) {
        cout << author->getName() << " wrote: \"" << text << "\"" << endl;
    }
}

Comment::~Comment() {
    // author is owned by User/Page, not by Comment
}