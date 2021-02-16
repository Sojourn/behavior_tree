#pragma once

#include <new>
#include <memory>
#include <utility>
#include <stdexcept>
#include <cstddef>
#include <cassert>

namespace bt {

    class Allocator {
        Allocator(Allocator&&) = delete;
        Allocator(const Allocator&) = delete;
        Allocator& operator=(Allocator&&) = delete;
        Allocator& operator=(const Allocator&) = delete;

    public:
        Allocator() = default;
        virtual ~Allocator() {}

        template<typename T, typename... Args>
        T& allocate(Args&&... args) {
            return *new(allocate_storage(sizeof(T))) T(std::forward<Args>(args)...);
        }

        template<typename T>
        T* allocate_array(size_t size) {
            T* array = reinterpret_cast<T*>(allocate_storage(sizeof(T) * size));

            for (size_t i = 0; i < size; ++i) {
                new(&array[i]) T;
            }

            return array;
        }

        template<typename T, typename F>
        T* allocate_array(size_t size, F&& initializer) {
            T* array = reinterpret_cast<T*>(allocate_storage(sizeof(T) * size));

            for (size_t i = 0; i < size; ++i) {
                initializer(i, static_cast<void*>(&array[i]));
            }

            return array;
        }

        template<typename T>
        void deallocate(T& object) {
            object.~T();
            deallocate_storage(&object, sizeof(object));
        }

        template<typename T>
        void deallocate_array(T* array, size_t size) {
            for (size_t i = 0; i < size; ++i) {
                array[size - i - 1].~T(); // reverse
            }

            deallocate_storage(array, sizeof(T) * size);
        }

    private:
        virtual void* allocate_storage(size_t size) = 0;
        virtual void deallocate_storage(void* storage, size_t size) = 0;
    };

    class GlobalAllocator : public Allocator {
        void* allocate_storage(size_t size) override {
            void* storage = malloc(size);
            assert(storage);
            return storage;
        }

        void deallocate_storage(void* storage, size_t) override {
            assert(storage);
            free(storage);
        }
    };

    class ArenaAllocator : public Allocator {
    public:
        ArenaAllocator(Allocator& allocator)
            : allocator_(allocator)
            , capacity_(128 * 1024)
            , used_(0)
            , buffer_(allocator.allocate_array<uint8_t>(capacity_))
        {
        }

        ~ArenaAllocator() {
            allocator_.deallocate_array(buffer_, capacity_);
            buffer_ = nullptr;
        }

    private:
        template<size_t alignment>
        static size_t align_up(size_t size) {
            while (size % alignment) {
                ++size;
            }

            return size;
        }

        void* allocate_storage(size_t size) override {
            size = align_up<alignof(void*)>(size);

            if (capacity_ < (used_ + size)) {
                throw std::runtime_error("ArenaAllocator allocation failed");
            }

            uint8_t* storage = &buffer_[used_];
            used_ += size;
            return storage;
        }

        void deallocate_storage(void* storage, size_t) override {
            // pass
        }

    private:
        Allocator& allocator_;
        size_t     used_;
        size_t     capacity_;
        uint8_t*   buffer_;
    };

}
