#pragma once

#include "../bt_node.h"
#include "../bt_task.h"
#include "../bt_fiber.h"

namespace bt {

    class ParallelNode : public Node {
        class ParallelTask : public Task {
        public:
            ParallelTask(const ParallelNode& node, Allocator& allocator)
                : child_count_(node.child_count()) // we need to know this before the children are bound
                , child_fibers_(nullptr)
                , allocator_(allocator)
            {
                // allocate a fiber for each subtree of this node
                child_fibers_ = allocator_.allocate_array<Fiber>(child_count_, [&](size_t child_index, void* storage) {
                    // calculate the maximum stack size based on the height of the subtree
                    size_t child_stack_capacity = node.child(child_index).height() + 1;
                    new(storage) Fiber(child_stack_capacity, allocator_);
                });
            }

            ~ParallelTask() {
                allocator_.deallocate_array(child_fibers_, child_count_);
            }

            void start(Fiber&) override {
                for (size_t child_index = 0; child_index < child_count_; ++child_index) {
                    child_fiber(child_index).start(child(child_index));
                }
            }

            Status run(Fiber&) override {
                Status merged_status = Status::success;

                for (size_t child_index = 0; child_index < child_count_; ++child_index) {
                    Fiber& fiber = child_fiber(child_index);

                    Status status = fiber.status();
                    if (status == Status::running) {
                        status = fiber.run();
                    }

                    if (status == Status::running) {
                        merged_status = status;
                    }
                    else if (status == Status::failure) {
                        merged_status = status;
                        break;
                    }
                }

                return merged_status;
            }

            void stop() override {
                for (size_t child_index = 0; child_index < child_count_; ++child_index) {
                    Fiber& fiber = child_fiber(child_count_ - child_index - 1);
                    if (fiber.status() == Status::running) {
                        fiber.stop();
                    }
                }
            }

            Fiber& child_fiber(size_t child_index) {
                assert(child_index < child_count_);

                return child_fibers_[child_index];
            }

        private:
            size_t     child_count_;
            Fiber*     child_fibers_;
            Allocator& allocator_;
        };

    public:
        Task& make_task(PropertyMap& properties, Allocator& allocator) const override {
            if (child_count() == 0) {
                throw std::runtime_error("ParallelNode must have at least one child");
            }

            return allocator.allocate<ParallelTask>(*this, allocator);
        }
    };

};
