#pragma once
#include "Post.h"
#include <string>
using namespace std;

class ActivityPost : public Post {
    int activityType;
    string activityValue;
public:
    ActivityPost();
    ActivityPost(string id, string desc, Date date,
        Entity* sharedBy, int type, string value);
    int getActivityType() const;
    string getActivityValue() const;
    string getActivityTypeString() const;
    void display() const override;
    ~ActivityPost();
};