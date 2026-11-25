#pragma once

namespace uge{
    struct Component{
    private:
        bool deleted = false;
    public:
        virtual ~Component() = default;
        void Delete() {deleted = true;}
        const bool& isDeleted() {return deleted; }
    };
}