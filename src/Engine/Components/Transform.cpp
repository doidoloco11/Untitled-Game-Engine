#include "Engine/Component.hpp"
#include <Engine/Components/Transform.hpp>

#include <pstl/glue_algorithm_defs.h>
#include <vector>
#include <algorithm>

void Transform::setParent(Transform *parent){
    if (this->parent) {
        std::vector<Transform*>& c = this->parent->children;
        c.erase(std::remove(c.begin(), c.end(), this), c.end());
    }
    this->parent = parent;
    if (parent){
        parent->children.push_back(this);
    }
}

void Transform::Delete(){
    Component::Delete();
    
    if (parent){
        std::vector<Transform*>& c = parent->children;
        c.erase(std::remove(c.begin(), c.end(), this), c.end());
    }

    for (Transform*& child : children) {
        child->parent = nullptr;
    }
    children.clear();
}