#include <stdexcept>
#include "bt_task.h"

using namespace bt;

std::string_view bt::to_string(Status status) {
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
