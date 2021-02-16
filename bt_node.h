#pragma once

#include <memory>
#include <vector>
#include "bt_task.h"
#include "bt_allocator.h"
#include "bt_property_map.h"

namespace bt {

    class Node {
        Node(Node&&) = delete;
        Node(const Node&) = delete;
        Node& operator=(Node&&) = delete;
        Node& operator=(const Node&) = delete;

    public:
        Node() = default;
        virtual ~Node();

        size_t height() const;
        size_t child_count() const;
        Node& child(size_t child_index);
        const Node& child(size_t child_index) const;
        void add_child(std::unique_ptr<Node> child);

        template<typename T, typename... Args>
        Node& add_child(Args&&... args);

        template<typename F>
        void visit(F&& f);

        template<typename F>
        void visit(F&& f) const;

        virtual Task& make_task(PropertyMap& properties, Allocator& allocator) const = 0;

    private:
        std::vector<std::unique_ptr<Node>> children_;
    };

    template<typename T, typename... Args>
    Node& Node::add_child(Args&&... args) {
        children_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        return *children_.back();
    }

    template<typename F>
    void Node::visit(F&& f) {
        f(*this);
        for (auto&& child: children_) {
            child->visit(f);
        }
    }

    template<typename F>
    void Node::visit(F&& f) const {
        f(*this);
        for (auto&& child: children_) {
            child->visit(f);
        }
    }

}
