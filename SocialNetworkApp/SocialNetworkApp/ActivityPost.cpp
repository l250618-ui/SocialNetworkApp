#include "ActivityPost.h"
#include "Entity.h"
#include <iostream>
using namespace std;

// default constructorsets everything to empty/zero
ActivityPost::ActivityPost() {
    activityType = 0;
    activityValue = "";
}


ActivityPost::ActivityPost(string id, string desc, Date date, Entity* sharedBy, int type, string value)
    : Post(id, desc, date, sharedBy) {
    activityType = type;
    activityValue = value;
}

// returning the ctual the number (1, 2, 3, or 4)
int ActivityPost::getActivityType() const {
    return activityType;
}

// returns the value e.g. "Happy", "birthday", "Kashmiri Chai" aas phrasesss
string ActivityPost::getActivityValue() const {
    return activityValue;
}

// converts the type number into a word for display
string ActivityPost::getActivityTypeString() const {
    if (activityType == 1) {
        return "feeling";        
    }
    else if (activityType == 2) {
        return "thinking about"; 
    }
    else if (activityType == 3) {
        return "making";         
    }
    else if (activityType == 4) {
        return "celebrating";   
    }
    return "";
}


void ActivityPost::display() const {
    // print who posted and what activity
    cout << "--- " << sharedBy->getName()
         << " is " << getActivityTypeString()
         << " " << activityValue << endl;

    // print the description text
    cout << "\"" << description << "\"" << " ... (" << sharedDate.toString() << ")" << endl;

    // print all comments
    displayComments();
}

// destructor - Post destructor handles memory cleanup
ActivityPost::~ActivityPost() {
}