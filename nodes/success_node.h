#pragma once

#include <stdexcept>
#include "behavior_tree/node.h"

namespace behavior_tree {

    class SuccessNode : public Node {
        class SuccessTask : public Task {
        public:
            Status run(Fiber&) override {
                return Status::success;
            }
        };

    public:
        Task& make_task(PropertyMap&, Allocator& allocator) const override {
            if (child_count() > 0) {
                throw std::runtime_error("SuccessNode must not have any children");
            }

            return allocator.allocate<SuccessTask>();
        }
    };

}
