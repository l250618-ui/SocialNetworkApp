#pragma once
#include "Post.h"

class Memory : public Post {
    Post* originalPost;
public:
    Memory();
    Memory(string id, string desc, Date date,Entity* sharedBy, Post* original);
    Post* getOriginalPost() const;
    void display() const override;
    ~Memory();
};