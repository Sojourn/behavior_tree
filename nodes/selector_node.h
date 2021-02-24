#pragma once

#include <cstddef>
#include "behavior_tree/node.h"
#include "behavior_tree/fiber.h"

namespace behavior_tree {

    class SelectorNode : public Node {
        class SelectorTask : public Task {
        public:
            SelectorTask()
                : child_index_(0)
            {
            }

            void start(Fiber& fiber) override {
                child_index_ = 0;
                fiber.spawn(child(child_index_));
            }

            Status run(Fiber& fiber) override {
                Status status = child(child_index_).status();
                if (status == Status::failure) {
                    ++child_index_;
                    if (child_index_ < child_count()) {
                        status = Status::running;
                        fiber.spawn(child(child_index_));
                    }
                }

                return status;
            }

        private:
            size_t child_index_;
        };

    public:
        Task& make_task(PropertyMap& properties, Allocator& allocator) const override {
            if (child_count() == 0) {
                throw std::runtime_error("SelectorNode must have at least one child");
            }

            return allocator.allocate<SelectorTask>();
        }
    };

}
