#pragma once

#include <new>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include "bt_fiber.h"
#include "bt_property_map.h"
#include "nodes/bt_root_node.h"

namespace bt {

    class BehaviorTree {
    public:
        BehaviorTree(const RootNode& root, PropertyMap& properties, Allocator& allocator)
            : allocator_(allocator)
            , task_count_(node_count(root))
            , tasks_(allocator_.allocate_array<Task*>(task_count_))
            , fiber_(root.height() + 1, allocator_)
            , status_(Status::running)
        {
            size_t task_index = 1; // children of the root task start at 1
            tasks_[0] = &make_task(task_index, root, properties);
            fiber_.start(*tasks_[0]);
        }

        ~BehaviorTree() {
            if (fiber_.status() == Status::running) {
                fiber_.stop();
            }

            for (size_t i = 0; i < task_count_; ++i) {
                allocator_.deallocate(*tasks_[i]);
            }

            allocator_.deallocate_array(tasks_, task_count_);
        }

        Status run() {
            if (status_ == Status::running) {
                status_ = fiber_.run();
            }

            return status_;
        }

    private:
        static size_t node_count(const Node& root) {
            size_t count = 0;
            root.visit([&](const Node&) {
                count += 1;
            });

            return count;
        }

        Task& make_task(size_t& task_index, const Node& node, PropertyMap& properties) {
            PropertyMap property_scope(properties);

            Task& task = node.make_task(property_scope, allocator_);

            // make children and populate the tasks array
            size_t base_child_task_index = task_index;
            size_t child_count = node.child_count();
            task_index += child_count;
            for (size_t child_index = 0; child_index < child_count; ++child_index) {
                tasks_[base_child_task_index + child_index] = &make_task(task_index, node.child(child_index), property_scope);
            }

            task.set_children(&tasks_[base_child_task_index], child_count);

            return task;
        }

    private:
        ArenaAllocator allocator_;
        size_t         task_count_;
        Task**         tasks_;
        Fiber          fiber_;
        Status         status_;
    };

}
