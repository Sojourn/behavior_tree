#pragma once

#include "allocator.h"

using namespace bt;

Allocator& GlobalAllocator::instance() {
    static GlobalAllocator self;
    return self;
}
