#pragma once

struct Component{
private:
    bool deleted = false;
public:
    virtual void Delete() {deleted = true;}
    const bool& isDeleted() {return deleted; }
};