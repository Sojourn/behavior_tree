#pragma once

#include "allocator.h"

using namespace behavior_tree;

Allocator& GlobalAllocator::instance() {
    static GlobalAllocator self;
    return self;
}
