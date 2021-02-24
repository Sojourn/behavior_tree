#include <stdexcept>
#include "task.h"

using namespace behavior_tree;

std::string_view behavior_tree::to_string(Status status) {
    using namespace std::literals;

    switch (status) {
        case Status::running:
            return "running"sv;
        case Status::success:
            return "success"sv;
        case Status::failure:
            return "failure"sv;
    }

    throw std::runtime_error("Invalid status");
}
