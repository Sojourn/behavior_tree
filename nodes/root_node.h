#pragma once

#include <stdexcept>
#include "../node.h"

namespace bt {

    class RootNode : public Node {
        class RootTask : public Task {
        public:
            void start(Fiber& fiber) override {
                fiber.spawn(child());
            }

            Status run(Fiber&) override {
                return child().status();
            }
        };

    public:
        Task& make_task(PropertyMap&, Allocator& allocator) const override {
            if (child_count() != 1) {
                throw std::runtime_error("RootNode must have exactly one child");
            }

            return allocator.allocate<RootTask>();
        }
    };

}
