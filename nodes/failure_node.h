#pragma once

#include <stdexcept>
#include "behavior_tree/node.h"

namespace behavior_tree {

    class FailureNode : public Node {
        class FailureTask : public Task {
        public:
            Status run(Fiber&) override {
                return Status::failure;
            }
        };

    public:
        Task& make_task(PropertyMap&, Allocator& allocator) const override {
            if (child_count() > 0) {
                throw std::runtime_error("FailureNode must not have any children");
            }

            return allocator.allocate<FailureTask>();
        }
    };

}
