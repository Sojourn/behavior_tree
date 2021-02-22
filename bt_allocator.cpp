#pragma once

#include "bt_allocator.h"

using namespace bt;

Allocator& GlobalAllocator::instance() {
    static GlobalAllocator self;
    return self;
}
