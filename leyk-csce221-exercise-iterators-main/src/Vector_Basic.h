#ifndef VECTOR_BASIC_H
#define VECTOR_BASIC_H

#include <vector>
#include <cstddef> // size_t

template <class T>
class Vector_Basic {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;
public:
    // Can create a vector from an std::vector
    // DO NOT MODIFY THIS FUNCTION
    Vector_Basic(const std::vector<T>& v): array(new T[v.capacity()]), _capacity(v.capacity()), _size(0) {
        for (const auto& elem: v) {
            array[_size] = elem;
            _size++;
        }
    }

    // Should return an iterator pointing to the first element
    iterator begin() noexcept { /* TODO */ }

    // Should return an iterator pointing past the last element of the container
    // This is because you want to stop when you reach end
    iterator end() noexcept { /* TODO */ }

    // Iterator class for the Vector_Basic
    // It is essentially a pointer defined for the type with memory safety
    class iterator {
    public:
        // These are characteristics that can be accessed from an iterator
        // For example, you could get the type from an iterator called iter
        // by using iter::value_type
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        // Points to some element in the vector (or nullptr)
        T* _ptr;
    public:
        iterator() { /* TODO */ }
        explicit iterator(T* ptr) { /* TODO */ }

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        // This is used to access the element the iterator points to, like dereferencing a pointer
        [[nodiscard]] reference operator*() const noexcept { /* TODO */ }

        // This is used to access data members of the underlying elements
        // It should return the address of the element the iterator points to
        [[nodiscard]] pointer operator->() const noexcept { /* TODO */ }

        // Prefix Increment: ++a
        // The value returned should be the iterator moved forward in the container
        iterator& operator++() noexcept { /* TODO */ }

        // Postfix Increment: a++
        // The value returned should be the iterator before it was moved forward in the container
        iterator operator++(int) noexcept { /* TODO */ }

        // Prefix Decrement: --a
        // The value returned should be the iterator moved backwards in the container
        iterator& operator--() noexcept { /* TODO */ }

        // Postfix Decrement: a--
        // The value returned should be the iterator before it was moved backwards in the container
        iterator operator--(int) noexcept { /* TODO */ }

        // Should move the iterator forward by an offset and return the new iterator
        iterator& operator+=(difference_type offset) noexcept { /* TODO */ }
        
        // Should return a copy of the iterator moved forward by an offset
        // NOTE: The iterator itself should not be moved, only the copy
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept { /* TODO */ }
        
        // Should move the iterator backwards by an offset and return the new iterator
        iterator& operator-=(difference_type offset) noexcept { /* TODO */ }

        // Should return a copy of the iterator moved backwards by an offset
        // NOTE: The iterator itself should not be moved, only the copy
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept { /* TODO */ }

        // Should return the difference between two iterators. This should be the offset betweent the
        // elements in memory.
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { /* TODO */ }

        // Should return the element at the location offset from the iterator
        [[nodiscard]] reference operator[](difference_type offset) const noexcept { /* TODO */ }

        // Used for comparing iterators
        // NOTE: This should be used to compare the iterators, not the elements they point to
        // HINT: Because iterators mimic pointers, many if not all of these operations can be done
        //       on the underlying pointers
        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { /* TODO */ }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept { /* TODO */ }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept { /* TODO */ }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept { /* TODO */ }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { /* TODO */ }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { /* TODO */ }
    };
};

#endif
