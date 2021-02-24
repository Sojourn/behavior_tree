#include <stdexcept>
#include "task.h"

using namespace behavior_tree;

std::string_view behavior_tree::to_string(Status status) {
    using namespace std::literals;

    switch (status) {
        case Status::running:
            return "running"sv;
        case Status::success:
            return "success"sv;
        case Status::failure:
            return "failure"sv;
    }

    throw std::runtime_error("Invalid status");
}

Task::Task()
    : status_(Status::success)
    , children_(nullptr)
    , child_count_(0)
{
}

Status Task::status() const {
    return status_;
}

Task& Task::child(size_t child_index) {
    assert(child_index < child_count());
    return *children_[child_index];
}

size_t Task::child_count() const {
    return child_count_;
}

void Task::set_status(Status status) {
    status_ = status;
}

void Task::set_children(Task** children, size_t child_count) {
    children_ = children;
    child_count_ = child_count;
}
