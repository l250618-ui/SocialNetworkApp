#pragma once
#include <string>
using namespace std;

class Entity {
protected:
    string id;
    string name;
public:
    Entity();
    Entity(string id, string name);
    string getID() const;
    string getName() const;
    virtual void display() const = 0;
    virtual ~Entity();
};