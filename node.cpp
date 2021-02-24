#include "node.h"
#include <algorithm>
#include <stdexcept>

using namespace behavior_tree;

Node::~Node() {
}

size_t Node::height() const {
    size_t max_height = 0;
    for (auto&& child: children_) {
        max_height = std::max(max_height, child->height() + 1);
    }

    return max_height;
}

size_t Node::child_count() const {
    return children_.size();
}

Node& Node::child(size_t child_index) {
    assert(child_index < children_.size());
    return *children_[child_index];
}

const Node& Node::child(size_t child_index) const {
    assert(child_index < children_.size());
    return *children_[child_index];
}

void Node::add_child(std::unique_ptr<Node> child) {
    children_.push_back(std::move(child));
}
