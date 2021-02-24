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
        Task();

        Status status() const;
        Task& child(size_t child_index = 0);
        size_t child_count() const;

        virtual void start(Fiber& fiber) {}
        virtual Status run(Fiber& fiber) = 0;
        virtual void stop() {}

    private:
        void set_status(Status status);
        void set_children(Task** children, size_t child_count);

    private:
        Status status_;
        Task** children_;
        size_t child_count_;
    };

}
