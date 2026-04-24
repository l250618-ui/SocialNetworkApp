#pragma once
#include <string>
using namespace std;

class Entity;

class Comment {
    Entity* author;
    string text;
public:
    Comment();
    Comment(Entity* author, string text);
    Entity* getAuthor() const;
    string getText() const;
    void display() const;
    ~Comment();
};