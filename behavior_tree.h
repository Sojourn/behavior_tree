#pragma once

#include <new>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include "allocator.h"
#include "property_map.h"
#include "fiber.h"
#include "nodes/root_node.h"

namespace behavior_tree {

    class BehaviorTree {
        BehaviorTree(BehaviorTree&&) = delete;
        BehaviorTree(const BehaviorTree&) = delete;
        BehaviorTree& operator=(BehaviorTree&&) = delete;
        BehaviorTree& operator=(const BehaviorTree&) = delete;

    public:
        BehaviorTree(const RootNode& root, PropertyMap& properties, Allocator& allocator = GlobalAllocator::instance());
        ~BehaviorTree();

        Status run();

    private:
        Task& make_task(size_t& task_index, const Node& node, PropertyMap& properties);

    private:
        ArenaAllocator allocator_;
        size_t         task_count_;
        Task**         tasks_;
        Fiber          fiber_;
        Status         status_;
    };

}
