#include <iostream>

#include "bt_behavior_tree.h"
#include "bt_property_map.h"
#include "nodes/bt_root_node.h"
#include "nodes/bt_selector_node.h"
#include "nodes/bt_parallel_node.h"
#include "nodes/bt_success_node.h"
#include "nodes/bt_failure_node.h"
#include "nodes/bt_condition_node.h"

using namespace bt;

std::unique_ptr<BehaviorTree> make_behavior_tree(PropertyMap& properties, Allocator& allocator) {
    RootNode root;

    {
        auto& parallel = root.add_child<ParallelNode>();
        parallel.add_child<ConditionNode<bool>>("flag1", Operator::equal, true);
        parallel.add_child<ConditionNode<bool>>("flag2", Operator::equal, true);
    }

    return std::make_unique<BehaviorTree>(root, properties, allocator);
}

int main() {
    GlobalAllocator allocator;
    PropertyMap properties;

    int position = 500;
    bool flag1 = true;
    bool flag2 = false;

    properties.export_mutable_property("position", position);
    properties.export_mutable_property("flag1", flag1);
    properties.export_mutable_property("flag2", flag2);

    auto behavior_tree = make_behavior_tree(properties, allocator);

    Status status = behavior_tree->run();
    switch (status) {
        case Status::running: {
            std::cout << "running" << std::endl;
            break;
        }
        case Status::success: {
            std::cout << "success" << std::endl;
            break;
        }
        case Status::failure: {
            std::cout << "failure" << std::endl;
            break;
        }
    }

    return 0;
}
