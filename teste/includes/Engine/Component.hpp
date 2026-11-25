#pragma once

namespace uge{
    struct Component{
    private:
        bool deleted = false;
    public:
        virtual void Delete() {deleted = true;}
        const bool& isDeleted() {return deleted; }
    };
}