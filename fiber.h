#pragma once

#include "bt_allocator.h"
#include "task.h"

namespace bt {

    class Task;

    // This class executes tasks, and can be embedded within tasks to implement parallel control flow paths.
    class Fiber {
        Fiber(Fiber&&) = delete;
        Fiber(const Fiber&) = delete;
        Fiber& operator=(Fiber&&) = delete;
        Fiber& operator=(const Fiber&) = delete;

    public:
        Fiber(size_t stack_capacity, Allocator& allocator);
        ~Fiber();

        // Start the fiber with an initial task.
        void start(Task& task);

        // Ticks active tasks until they stop making progress. Returns the status of the initial task.
        Status run();

        // Returns the status of the initial task.
        Status status() const;

        // Forcefully stops a fiber. This should not be called from within run.
        void stop();

        // Spawn a child task.
        void spawn(Task& task);

    private:
        Task& active_task();
        void push_task(Task& task);
        void pop_task();

    private:
        Status     fiber_status_;
        bool       idle_;
        size_t     stack_size_;
        size_t     stack_capacity_;
        Task**     stack_;
        Allocator& allocator_;
    };

}
