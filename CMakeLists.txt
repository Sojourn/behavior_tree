cmake_minimum_required(VERSION 3.17)
project(behavior_tree)

set(CMAKE_CXX_STANDARD 20)

add_executable(behavior_tree main.cpp bt_node.h bt_node.cpp bt_behavior_tree.h nodes/bt_root_node.h nodes/bt_parallel_node.h nodes/bt_sequence_node.h nodes/bt_selector_node.h bt_property_map.h bt_task.h bt_allocator.h bt_fiber.h nodes/bt_success_node.h nodes/bt_failure_node.h bt_fiber.cpp nodes/bt_condition_node.h bt_task.cpp bt_behavior_tree.cpp bt_allocator.cpp)