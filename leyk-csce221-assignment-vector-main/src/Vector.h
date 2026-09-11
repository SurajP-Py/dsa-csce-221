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
    void grow() { 
        size_t new_capacity = (_capacity == 0) ? 1 : _capacity * 2;
        T* new_array = new T[new_capacity];
        for (size_t i = 0; i < _size; i++) new_array[i] = std::move(array[i]);
        delete[] array;
        array = new_array;
        _capacity = new_capacity;
    }

public:
    Vector() noexcept : array(nullptr), _capacity(0), _size(0) { /* TODO */ }
    Vector(size_t count, const T& value) : array(new T[count]), _capacity(count), _size(count)  {
        for(size_t i = 0; i < count; ++i) array[i] = value;
    }

    // explicit prevents accidental conversions like Vector<int> v = 5;
    explicit Vector(size_t count) : array(new T[count]), _capacity(count), _size(count) { }

    Vector(const Vector& other) : array(new T[other._capacity]), _capacity(other._capacity), _size(other._size) { 
        for(size_t i = 0; i < _size; ++i) array[i] = other.array[i];
    }
    Vector(Vector&& other) noexcept : array(other.array), _capacity(other._capacity), _size(other._size) { 
        other.array = nullptr;
        other._capacity = 0;
        other._size = 0;
    }

    ~Vector() { delete[] array; }

    Vector& operator=(const Vector& other) { 
        _capacity = other._capacity;
        _size = other._size;
        array = new T[_capacity];
        for(size_t i = 0; i < _size; ++i) array[i] = other.array[i];
    }
    Vector& operator=(Vector&& other) noexcept { 
        array = other.array;
        _capacity = other._capacity;
        _size = other._size;
        
        other.array = nullptr;
        other._capacity = 0;
        other._size = 0;
    }

    iterator begin() noexcept { return iterator(array); }
    iterator end() noexcept { return iterator(array + _size); }

    // nodiscard warns if you call this function and ignore its return value.
    // Does not affect implementation.
    [[nodiscard]] bool empty() const noexcept { return _size == 0; }
    size_t size() const noexcept { return _size; }
    size_t capacity() const noexcept { return _capacity; }

    T& at(size_t pos) { 
        if (pos >= _size) throw std::out_of_range("Index out of range");
        return array[pos];
    }
    const T& at(size_t pos) const { 
        if (pos >= _size) throw std::out_of_range("Index out of range");
        return array[pos];
    }
    T& operator[](size_t pos) { return array[pos]; }
    const T& operator[](size_t pos) const { return array[pos]; }
    T& front() { return array[0]; }
    const T& front() const { return array[0]; }
    T& back() { return array[_size - 1]; }
    const T& back() const { return array[_size - 1]; }

    void push_back(const T& value) { 
        if(_capacity == _size) {
            grow();
        }
        array[_size] = value;
        ++_size;
    }
    void push_back(T&& value) { 
        if(_capacity == _size) {
            grow();
        }
        array[_size] = value;
        ++_size;
    }
    void pop_back() { 
        --_size;
    }

    iterator insert(iterator pos, const T& value) { 
        size_t index = static_cast<size_t>(pos - begin());
        if(_capacity == _size) {
            grow();
            for (size_t i = _size; i > index; --i) {
                array[i] = std::move(array[i - 1]);
            }
            array[index] = std::move(value);
        } else {
            for (size_t i = _size; i > index; --i) {
                array[i] = std::move(array[i - 1]);
            }
            array[index] = value;
        }
        
        ++_size;
        return iterator(array + index);
    }
    iterator insert(iterator pos, T&& value) { 
        size_t index = static_cast<size_t>(pos - begin());
        if(_capacity == _size) {
            grow();
        }

        for (size_t i = _size; i > index; --i) {
            array[i] = std::move(array[i - 1]);
        }

        array[index] = std::move(value);
        
        ++_size;
        return iterator(array + index);
    }
    iterator insert(iterator pos, size_t count, const T& value) { 
        size_t index = static_cast<size_t>(pos - begin());
        
        while (_size + count >  _capacity) grow();
        
        for (size_t i = _size; i > index; --i){
            array[i + count - 1] = std::move(array[i - 1]);
        }
        
        for (size_t i = 0; i < count; ++i) {
            array[index + i] = value;
        }

        _size += count;
        return iterator(array + index);
    }
    iterator erase(iterator pos) { 
        size_t index = static_cast<size_t>(pos - begin());
        for (size_t i = index; i < _size - 1; ++i) {
            array[i] = std::move(array[i + 1]);
        }
        --_size;
        return iterator(array + index);
    }
    iterator erase(iterator first, iterator last) { 
        size_t index = static_cast<size_t>(first - begin());
        size_t count = static_cast<size_t>(last - first);

        for (size_t i = 0; i < _size - (index + count); ++i){
            array[index + i] = std::move(array[index + i + count]);
        }

        _size -= count;
        return iterator(array + index);
    }

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
        iterator() : _ptr(nullptr) { /* TODO */ }
        explicit iterator(T* ptr) : _ptr(ptr) { /* TODO */ }

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        // This is used to access the element the iterator points to, like dereferencing a pointer
        [[nodiscard]] reference operator*() const noexcept { return *_ptr; }

        // This is used to access data members of the underlying elements
        // It should return the address of the element the iterator points to
        [[nodiscard]] pointer operator->() const noexcept { return _ptr; }

        // Prefix Increment: ++a
        // The value returned should be the iterator moved forward in the container
        iterator& operator++() noexcept { 
            ++_ptr;
            return *this;
        }

        // Postfix Increment: a++
        // The value returned should be the iterator before it was moved forward in the container
        iterator operator++(int) noexcept { 
            iterator tmp = *this;
            ++_ptr;
            return tmp;
        }

        // Prefix Decrement: --a
        // The value returned should be the iterator moved backwards in the container
        iterator& operator--() noexcept { 
            --_ptr;
            return *this;
        }

        // Postfix Decrement: a--
        // The value returned should be the iterator before it was moved backwards in the container
        iterator operator--(int) noexcept { 
            iterator tmp = *this;
            --_ptr;
            return tmp;
        }

        // Should move the iterator forward by an offset and return the new iterator
        iterator& operator+=(difference_type offset) noexcept { 
            _ptr += offset;
            return *this;
        }
        
        // Should return a copy of the iterator moved forward by an offset
        // NOTE: The iterator itself should not be moved, only the copy
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept { 
            return iterator(_ptr + offset);
        }
        
        // Should move the iterator backwards by an offset and return the new iterator
        iterator& operator-=(difference_type offset) noexcept { 
            _ptr -= offset;
            return *this;
        }

        // Should return a copy of the iterator moved backwards by an offset
        // NOTE: The iterator itself should not be moved, only the copy
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept { 
            return iterator(_ptr - offset);
        }

        // Should return the difference between two iterators. This should be the offset betweent the
        // elements in memory.
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { 
            return _ptr - rhs._ptr;
        }

        // Should return the element at the location offset from the iterator
        [[nodiscard]] reference operator[](difference_type offset) const noexcept { 
            return *(_ptr + offset);
        }

        // Used for comparing iterators
        // NOTE: This should be used to compare the iterators, not the elements they point to
        // HINT: Because iterators mimic pointers, many if not all of these operations can be done
        //       on the underlying pointers
        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { return _ptr == rhs._ptr; }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept { return _ptr != rhs._ptr; }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept { return _ptr < rhs._ptr; }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept { return _ptr > rhs._ptr; }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { return _ptr <= rhs._ptr; }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { return _ptr >= rhs._ptr; }
    };


    void clear() noexcept { _size = 0; }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept { return iterator + offset; }

#endif
