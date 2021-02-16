#pragma once

#include "bt_allocator.h"
#include "bt_task.h"

namespace bt {

    class Task;

    class Fiber {
        Fiber(Fiber&&) = delete;
        Fiber(const Fiber&) = delete;
        Fiber& operator=(Fiber&&) = delete;
        Fiber& operator=(const Fiber&) = delete;

    public:
        Fiber(size_t stack_capacity, Allocator& allocator);
        ~Fiber();

        void start(Task& task);
        Status run();
        Status status() const;
        void stop();

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
