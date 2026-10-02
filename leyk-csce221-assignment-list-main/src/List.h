#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}
        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

    public:
        basic_iterator() : node{nullptr} { /* TODO */ };
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        reference operator*() const {
            // TODO
            return node->data;
        }
        pointer operator->() const {
            // TODO
            return &(node->data);
        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            // TODO
            node = node->next;
            return *this;
        }
        // Postfix Increment: a++
        basic_iterator operator++(int) {
            // TODO
            basic_iterator temp{*this};
            node = node->next;
            return temp;
        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            // TODO
            node = node->prev;
            return *this;
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            // TODO
            basic_iterator temp{*this};
            node = node->prev;
            return temp;
        }

        bool operator==(const basic_iterator& other) const noexcept {
            // TODO
            return node == other.node;
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            // TODO
            return !(*this == other);
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List() : head(nullptr), tail(nullptr), _size(0) {
        // TODO - Don't forget to initialize the list beforehand 

        head.next = &tail;
        head.prev = nullptr;

        tail.next = nullptr;
        tail.prev = &head;
    }
    List( size_type count, const T& value ) : head(nullptr), tail(nullptr), _size(0) {
        // TODO - Don't forget initialize the list beforehand
        
        head.next = &tail;
        tail.prev = &head;

        for(size_t i = 0; i < count; i++){
            push_back(value);
        }
    }
    explicit List( size_type count ) : head(nullptr), tail(nullptr), _size(0) {
        // TODO - Don't forget initialize the list beforehand
        
        head.next = &tail;
        tail.prev = &head;

        for(size_t i = 0; i < count; i++){
            push_back(T());
        }
    }
    List( const List& other ) : head(nullptr), tail(nullptr), _size(0) {
        // TODO - Don't forget initialize the list beforehand

        head.next = &tail;
        tail.prev = &head;

        Node* curr = other.head.next;
        while(curr != &other.tail) {
           push_back(curr->data);
           curr = curr->next;
        }
    }
    List( List&& other ) : head(nullptr), tail(nullptr), _size(0) {
        head.next = &tail;
        tail.prev = &head;

        if(other._size > 0) {
            head.next = other.head.next;
            tail.prev = other.tail.prev;
            head.next->prev = &head;
            tail.prev->next = &tail;
            _size = other._size;

            other.head.next = &other.tail;
            other.tail.prev = &other.head;
            other._size =  0;
        }
    }
    ~List() {
        // TODO
        Node* curr = head.next;
        while(curr != &tail) {
            Node* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
    }
    List& operator=( const List& other ) {
        // TODO
        if(this == &other) return *this;

        clear();

        Node* curr = other.head.next;
        while(curr != &other.tail) {
            push_back(curr->data);
            curr = curr->next;
        }
        return *this;
    }
    List& operator=( List&& other ) noexcept {
        // TODO
        if(this != &other){ 
            clear();
            if(other._size > 0) {
                head.next = other.head.next;
                tail.prev = other.tail.prev;
                head.next->prev = &head;
                tail.prev->next = &tail;
                _size = other._size;

                other.head.next = &other.tail;
                other.tail.prev = &other.head;
                other._size =  0;
            }
        }
        return *this;
    }

    reference front() {
        // TODO
        return head.next->data;
    }
    const_reference front() const {
        // TODO
        return head.next->data;
    }
	
    reference back() {
        // TODO
        return tail.prev->data;
    }
    const_reference back() const {
        // TODO
        return tail.prev->data;
    }
	
    iterator begin() noexcept {
        // TODO
        return iterator(head.next);
    }
    const_iterator begin() const noexcept {
        // TODO
        return const_iterator(head.next);
    }
    const_iterator cbegin() const noexcept {
        // TODO
        return const_iterator(head.next);
    }

    iterator end() noexcept {
        // TODO
        return iterator(&tail);
    }
    const_iterator end() const noexcept {
        // TODO
        return const_iterator(&tail);
    }
    const_iterator cend() const noexcept {
        // TODO
        return const_iterator(&tail);
    }

    bool empty() const noexcept {
        // TODO
        return _size == 0;
    }

    size_type size() const noexcept {
        // TODO
        return _size;
    }

    void clear() noexcept {
        // TODO
        Node* curr = head.next;
        while(curr != &tail) {
            Node* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
        head.next = &tail;
        tail.prev = &head;
        _size = 0;
    }

    iterator insert( const_iterator pos, const T& value ) {
        // TODO
        Node* curr = pos.node;
        Node* prev_node = curr->prev;
        Node* new_node = new Node(value, prev_node, curr);

        prev_node->next = new_node;
        curr->prev = new_node;
        _size++;
        return iterator(new_node);
    }
    iterator insert( const_iterator pos, T&& value ) {
        // TODO
        Node* curr = pos.node;
        Node* prev_node = curr->prev;
        Node* new_node = new Node(std::move(value), prev_node, curr);

        prev_node->next = new_node;
        curr->prev = new_node;
        _size++;
        return iterator(new_node);
    }

    iterator erase( const_iterator pos ) {
        // TODO
        Node* curr = pos.node;
        Node* prev_node = curr->prev;
        Node* next_node = curr->next;

        prev_node->next = next_node;
        next_node->prev = prev_node;

        delete curr;
        _size--;

        return iterator(next_node);
    }

    void push_back( const T& value ) {
        // TODO
        Node* newNode = new Node(value, tail.prev, &tail);

        tail.prev->next = newNode;
        tail.prev = newNode;

        _size++;

    }
    void push_back( T&& value ) {
        // TODO
        Node* newNode = new Node(std::move(value), tail.prev, &tail);

        tail.prev->next = newNode;
        tail.prev = newNode;

        _size++;
    }

    void pop_back() {
        // TODO
        if(_size == 0) return;
        Node* temp = tail.prev;

        temp->prev->next = &tail;
        tail.prev = temp->prev;

        _size--;
        delete temp;
    }
	
    void push_front( const T& value ) {
        // TODO
        Node* newNode = new Node(value, &head, head.next);

        head.next->prev = newNode;
        head.next = newNode;

        _size++;
    }
	void push_front( T&& value ) {
        // TODO
        Node* newNode = new Node(std::move(value), &head, head.next);

        head.next->prev = newNode;
        head.next = newNode;

        _size++;
    }

    void pop_front() {
        // TODO
        if (_size == 0) return;
        Node* temp = head.next;

        head.next = temp->next;
        temp->next->prev = &head;

        delete temp;
        _size--;
    }

    /*
      You do not need to modify these methods!
      
      These method provide the non-const complement 
      for the const_iterator methods provided above.
    */
    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/
 
namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}