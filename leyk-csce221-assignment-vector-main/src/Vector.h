#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() { /* TODO */ }

public:
    Vector() noexcept { /* TODO */ }
    Vector(size_t count, const T& value) { /* TODO */ }

    // explicit prevents accidental conversions like Vector<int> v = 5;
    explicit Vector(size_t count) { /* TODO */ }
    Vector(const Vector& other) { /* TODO */ }
    Vector(Vector&& other) noexcept { /* TODO */ }

    ~Vector() { /* TODO */ }

    Vector& operator=(const Vector& other) { /* TODO */ }
    Vector& operator=(Vector&& other) noexcept { /* TODO */ }

    iterator begin() noexcept { /* TODO */ }
    iterator end() noexcept { /* TODO */ }

    // nodiscard warns if you call this function and ignore its return value.
    // Does not affect implementation.
    [[nodiscard]] bool empty() const noexcept { /* TODO */ }
    size_t size() const noexcept { /* TODO */ }
    size_t capacity() const noexcept { /* TODO */ }

    T& at(size_t pos) { /* TODO */ }
    const T& at(size_t pos) const { /* TODO */ }
    T& operator[](size_t pos) { /* TODO */ }
    const T& operator[](size_t pos) const { /* TODO */ }
    T& front() { /* TODO */ }
    const T& front() const { /* TODO */ }
    T& back() { /* TODO */ }
    const T& back() const { /* TODO */ }

    void push_back(const T& value) { /* TODO */ }
    void push_back(T&& value) { /* TODO */ }
    void pop_back() { /* TODO */ }

    iterator insert(iterator pos, const T& value) { /* TODO */ }
    iterator insert(iterator pos, T&& value) { /* TODO */ }
    iterator insert(iterator pos, size_t count, const T& value) { /* TODO */ }
    iterator erase(iterator pos) { /* TODO */ }
    iterator erase(iterator first, iterator last) { /* TODO */ }

    class iterator {
    public:
    // NOTE: Type Aliases
    // These let you write code using English terms as type names to help you understand the code.
    // T& and refrence can be used interchangeably and you are not required to use the aliases.
    // (try to be consistent however)
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

        [[nodiscard]] reference operator*() const noexcept { /* TODO */ }
        [[nodiscard]] pointer operator->() const noexcept { /* TODO */ }

        // Prefix Increment: ++a
        iterator& operator++() noexcept { /* TODO */ }
        // Postfix Increment: a++
        iterator operator++(int) noexcept { /* TODO */ }
        // Prefix Decrement: --a
        iterator& operator--() noexcept { /* TODO */ }
        // Postfix Decrement: a--
        iterator operator--(int) noexcept { /* TODO */ }

        iterator& operator+=(difference_type offset) noexcept { /* TODO */ }
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept { /* TODO */ }

        iterator& operator-=(difference_type offset) noexcept { /* TODO */ }
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept { /* TODO */ }
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { /* TODO */ }

        [[nodiscard]] reference operator[](difference_type offset) const noexcept { /* TODO */ }

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { /* TODO */ }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept { /* TODO */ }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept { /* TODO */ }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept { /* TODO */ }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { /* TODO */ }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { /* TODO */ }
    };


    void clear() noexcept { /* TODO */ }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept { /* TODO */ }

#endif
