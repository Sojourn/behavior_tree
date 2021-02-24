#pragma once

#include <vector>
#include <string_view>
#include <cstddef>
#include <cassert>

namespace behavior_tree {

    class Fiber;

    enum class Status {
        running,
        success,
        failure,
    };

    std::string_view to_string(Status status);

    class Task {
        friend class Fiber;
        friend class BehaviorTree;

        Task(Task&&) = delete;
        Task(const Task&) = delete;
        Task& operator=(Task&&) = delete;
        Task& operator=(const Task&) = delete;

    public:
        Task()
            : status_(Status::success)
            , children_(nullptr)
            , child_count_(0)
        {
        }

        Status status() {
            return status_;
        }

        Task& child(size_t child_index=0) {
            assert(child_index < child_count());
            return *children_[child_index];
        }

        size_t child_count() const {
            return child_count_;
        }

        virtual void start(Fiber& fiber) {}
        virtual Status run(Fiber& fiber) = 0;
        virtual void stop() {}

    private:
        void set_status(Status status) {
            status_ = status;
        }

        void set_children(Task** children, size_t child_count) {
            children_ = children;
            child_count_ = child_count;
        }

    private:
        Status status_;
        Task** children_;
        size_t child_count_;
    };

}
