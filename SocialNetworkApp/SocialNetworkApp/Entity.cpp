#include "Entity.h"

    Entity:: Entity() {
        id = "";
        name = "";
    }
    Entity::Entity(string id, string name) {
        this->id = id;
        this->name = name;
    }
    string Entity:: getID() const {
        return id;
    }
    string Entity::getName() const {
        return name;
    }
    Entity:: ~Entity() {

    }