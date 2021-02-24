#pragma once

#include <cstdint>
#include "behavior_tree/node.h"
#include "behavior_tree/task.h"

namespace bt {

    enum class Operator : uint8_t {
        equal,
        not_equal,
        less_than,
        less_than_or_equal,
        greater_than,
        greater_than_or_equal,
    };

    template<typename T>
    class ConditionNode : public Node {
        class ConditionTask : public Task {
        public:
            ConditionTask(const T& lhs, Operator op, const T& rhs)
                : lhs_(lhs)
                , rhs_(rhs)
                , op_(op)
            {
            }

            Status run(Fiber&) override {
                bool result = false;
                switch (op_) {
                    case Operator::equal: {
                        result = lhs_ == rhs_;
                        break;
                    }
                    case Operator::not_equal: {
                        result = lhs_ != rhs_;
                        break;
                    }
                    case Operator::less_than: {
                        result = lhs_ < rhs_;
                        break;
                    }
                    case Operator::less_than_or_equal: {
                        result = lhs_ <= rhs_;
                        break;
                    }
                    case Operator::greater_than: {
                        result = lhs_ > rhs_;
                        break;
                    }
                    case Operator::greater_than_or_equal: {
                        result = lhs_ >= rhs_;
                        break;
                    }
                    default: {
                        throw std::runtime_error("Unsupported operator");
                    }
                }

                return result ? Status::success : Status::failure;
            }

        private:
            const T& lhs_;
            T        rhs_;
            Operator op_;
        };

    public:
        ConditionNode(const std::string& property_name, Operator op, const T& value)
            : property_name_(property_name)
            , value_(value)
            , op_(op)
        {
        }

        Task& make_task(PropertyMap& properties, Allocator& allocator) const override {
            if (child_count() > 0) {
                throw std::runtime_error("ConditionNode must not have any children");
            }

            return allocator.allocate<ConditionTask>(properties.import_property<T>(property_name_), op_, value_);
        }

    private:
        std::string property_name_;
        T           value_;
        Operator    op_;
    };

}
