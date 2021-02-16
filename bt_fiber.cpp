#include "bt_fiber.h"

using namespace bt;

Fiber::Fiber(size_t stack_capacity, Allocator& allocator)
    : fiber_status_(Status::failure)
    , idle_(true)
    , stack_capacity_(stack_capacity)
    , stack_size_(0)
    , stack_(allocator.allocate_array<Task*>(stack_capacity))
    , allocator_(allocator)
{
}

Fiber::~Fiber() {
    if (fiber_status_ == Status::running) {
        stop();
    }

    allocator_.deallocate_array(stack_, stack_capacity_);
}

void Fiber::start(Task& task) {
    assert(fiber_status_ != Status::running);
    assert(idle_);
    assert(stack_size_ == 0);

    fiber_status_ = Status::running;
    spawn(task);
}

Status Fiber::run() {
    assert(fiber_status_ == Status::running);
    assert(idle_);
    assert(stack_size_ > 0);

    // idle_ is used to check for reentrancy
    {
        idle_ = false;

        while (stack_size_) {
            Task& task = active_task();
            assert(task.status() == Status::running);

            Status task_status = task.run(*this);
            if (task_status == Status::running) {
                if (&task == &active_task()) {
                    // the task is still the active task (didn't spawn a child task), and must be waiting
                    break;
                }
            }
            else {
                task.set_status(task_status);
                task.stop();

                pop_task();
                if (stack_size_ == 0) {
                    fiber_status_ = task_status; // complete
                }
            }
        }

        idle_ = true;
    }

    return fiber_status_;
}

void Fiber::stop() {
    assert(fiber_status_ == Status::running);
    assert(idle_);

    while (stack_size_) {
        active_task().stop();
        pop_task();
    }

    fiber_status_ = Status::failure;
    assert(stack_size_ == 0);
}

Status Fiber::status() const {
    return fiber_status_;
}

void Fiber::spawn(Task& task) {
    assert(task.status() != Status::running);

    push_task(task);
    task.set_status(Status::running);
    task.start(*this);
}

Task& Fiber::active_task() {
    assert(stack_size_ > 0);

    return *stack_[stack_size_ - 1];
}

void Fiber::push_task(Task& task) {
    assert(stack_size_ < stack_capacity_);

    stack_[stack_size_++] = &task;
}

void Fiber::pop_task() {
    assert(stack_size_ > 0);

    stack_size_ -= 1;
    stack_[stack_size_] = nullptr;
}
