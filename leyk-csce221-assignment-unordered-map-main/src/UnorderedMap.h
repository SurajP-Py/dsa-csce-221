#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"



template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
class UnorderedMap {
    public:

    using key_type = Key;
    using mapped_type = T;
    using const_mapped_type = const T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    private:

    struct HashNode {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}
        HashNode(const value_type & val, HashNode * next = nullptr) : next { next }, val { val } { }
        HashNode(value_type && val, HashNode * next = nullptr) : next { next }, val { std::move(val) } { }
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode * _head;
    size_type _size;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

    public:

    template <typename pointer_type, typename reference_type, typename _value_type>
    class basic_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = _value_type;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        const UnorderedMap * _map;
        HashNode * _ptr;

        explicit basic_iterator(UnorderedMap const * map, HashNode *ptr) noexcept : _map{map}, _ptr{ptr} { /* TODO */ }

    public:
        basic_iterator() : _map{nullptr}, _ptr{nullptr} { /* TODO */ };

        basic_iterator(const basic_iterator &) = default;
        basic_iterator(basic_iterator &&) = default;
        ~basic_iterator() = default;
        basic_iterator &operator=(const basic_iterator &) = default;
        basic_iterator &operator=(basic_iterator &&) = default;
        reference operator*() const { return _ptr->val; }
        pointer operator->() const { return &(_ptr->val); }
        basic_iterator &operator++() {
            if(!_ptr) return *this;
            if(_ptr->next) {
                _ptr = _ptr->next;
                return *this;
            }

            size_type b = _map->_bucket(_ptr->val) + 1;
            while(b < _map->_bucket_count && _map->_buckets[b] == nullptr) ++b;
            _ptr = (b < _map->_bucket_count) ? _map->_buckets[b] : nullptr;
            return *this;
        }
        basic_iterator operator++(int) {
            basic_iterator copy = *this;
            ++(*this);
            return copy;
        }
        bool operator==(const basic_iterator &other) const noexcept { return _ptr == other._ptr; }
        bool operator!=(const basic_iterator &other) const noexcept { return _ptr != other._ptr; }
    };

    using iterator = basic_iterator<pointer, reference, value_type>;
    using const_iterator = basic_iterator<const_pointer, const_reference, const value_type>;

    class local_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const key_type, mapped_type>;
            using difference_type = ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

        private:
            friend class UnorderedMap<Key, T, Hash, key_equal>;
            using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

            HashNode * _node;

            explicit local_iterator( HashNode * node ) noexcept : _node{node} { /* TODO */ }

        public:
            local_iterator() : _node{nullptr} { /* TODO */ }

            local_iterator(const local_iterator &) = default;
            local_iterator(local_iterator &&) = default;
            ~local_iterator() = default;
            local_iterator &operator=(const local_iterator &) = default;
            local_iterator &operator=(local_iterator &&) = default;
            reference operator*() const { return _node->val; }
            pointer operator->() const { return &(_node->val); }
            local_iterator & operator++() { 
                if(_node) _node = _node->next;
                return *this;
            }
            local_iterator operator++(int) {
                local_iterator copy = *this;
                ++(*this);
                return copy;
            }

            bool operator==(const local_iterator &other) const noexcept { return _node == other._node; }
            bool operator!=(const local_iterator &other) const noexcept { return _node != other._node; }
    };

private:

    size_type _bucket(size_t code) const { return _range_hash(code, _bucket_count); }
    size_type _bucket(const Key & key) const { return _range_hash(_hash(key), _bucket_count); }
    size_type _bucket(const value_type & val) const { return _range_hash(_hash(val.first), _bucket_count); }

    HashNode*& _find(size_type code, size_type bucket, const Key & key) { 
        (void) code;
        HashNode ** slot = &_buckets[bucket];

        while(*slot && !_equal((*slot)->val.first, key)) slot = &(*slot)->next;

        return *slot;
    }

    HashNode*& _find(const Key & key) { 
        size_type code = _hash(key);
        return _find(code, _range_hash(code, _bucket_count), key);
    }

    HashNode * _insert_into_bucket(size_type bucket, value_type && value) { 
        _buckets[bucket] = new HashNode(std::move(value), _buckets[bucket]);
        ++_size;
        return _buckets[bucket];
    }

    void _move_content(UnorderedMap & src, UnorderedMap & dst) { 
        dst.clear();
        delete[] dst._buckets;

        dst._bucket_count = src._bucket_count;
        dst._buckets = src._buckets;
        dst._head = src._head;
        dst._size = src._size;
        dst._hash = src._hash;
        dst._equal = src._equal;

        src._buckets = nullptr;
        src._head = nullptr;
        src._size = 0;
    }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash & hash = Hash { },
                const key_equal & equal = key_equal { }) : _bucket_count{next_greater_prime(bucket_count)}, _buckets{nullptr}, _head{nullptr}, _size{0}, _hash{hash}, _equal{equal} { 
                    _buckets = new HashNode*[_bucket_count]();
                 }

    ~UnorderedMap() { 
        clear();
        delete[] _buckets;
    }

    UnorderedMap(const UnorderedMap & other) : _bucket_count{other._bucket_count}, _buckets{new HashNode*[other._bucket_count]()}, _head{nullptr}, _size{0}, _hash{other._hash}, _equal{other._equal} { 
        for(size_type b = 0; b < _bucket_count; ++b){
            HashNode ** tail = &_buckets[b];
            for(HashNode * n = other._buckets[b]; n; n = n->next){
                *tail = new HashNode(n->val);
                tail = &(*tail)->next;
                ++_size;
            }
        }
    }

    UnorderedMap(UnorderedMap && other) : _bucket_count{0}, _buckets{nullptr}, _head{nullptr}, _size{0}, _hash{other._hash}, _equal{other._equal} { 
        _move_content(other, *this); 
        other._buckets = new HashNode*[other._bucket_count]();
    }

    UnorderedMap & operator=(const UnorderedMap & other) { 
        if(this != &other) {
            UnorderedMap tmp(other);
            _move_content(tmp, *this);
        }
        return *this;
    }

    UnorderedMap & operator=(UnorderedMap && other) { 
        if(this != &other) {
            _move_content(other, *this);
            other._buckets = new HashNode*[other._bucket_count]();
        }
        return *this;
    }

    void clear() noexcept { 
        if(!_buckets) return;
        for(size_type b = 0; b < _bucket_count; ++b){
            HashNode * node = _buckets[b];
            while(node) {
                HashNode * next = node->next;
                delete node;
                node = next;
            }
            _buckets[b] = nullptr;
        }
        _size = 0;
    }

    size_type size() const noexcept { return _size; }

    bool empty() const noexcept { return _size == 0; }

    size_type bucket_count() const noexcept { return _bucket_count; }

    iterator begin() { 
        for(size_type b = 0; b < _bucket_count; ++b) {
            if(_buckets[b]) return iterator(this, _buckets[b]);
        }
        return end();
    }
    iterator end() { return iterator(this, nullptr); }

    const_iterator cbegin() const { 
        for(size_type b = 0; b < _bucket_count; ++b) {
            if(_buckets[b]) return const_iterator(this, _buckets[b]);
        }
        return cend();};
    const_iterator cend() const { return const_iterator(this, nullptr); };

    local_iterator begin(size_type n) { return local_iterator(_buckets[n]); }
    local_iterator end(size_type n) { 
        (void) n;
        return local_iterator(nullptr);
    }

    size_type bucket_size(size_type n) { 
        size_type count = 0;
        for(HashNode * node = _buckets[n]; node; node = node->next) ++count;
        return count;
    }

    float load_factor() const { return static_cast<float>(_size) / static_cast<float>(_bucket_count); }

    size_type bucket(const Key & key) const { return _range_hash(_hash(key), _bucket_count); }

    std::pair<iterator, bool> insert(value_type && value) { 
        size_type code = _hash(value.first);
        size_type b = _range_hash(code, _bucket_count);

        HashNode *& slot = _find(code, b, value.first);
        if(slot) return { iterator(this, slot), false};

        HashNode * node = _insert_into_bucket(b, std::move(value));
        return { iterator(this, node), true};
    }

    std::pair<iterator, bool> insert(const value_type & value) { return insert(value_type(value)); }

    iterator find(const Key & key) { return iterator(this, _find(key)); }

    T& operator[](const Key & key) { 
        size_type code = _hash(key);
        size_type b = _range_hash(code, _bucket_count);

        HashNode *& slot = _find(code, b, key);

        if(slot) return slot->val.second;

        HashNode * node = _insert_into_bucket(b, value_type(key, T()));
        return node->val.second;
    }

    iterator erase(iterator pos) { 
        if(pos._ptr == nullptr) return end();

        iterator next = pos;
        ++next;

        HashNode *& slot = _find(pos._ptr->val.first);
        HashNode * victim = slot;
        slot = victim->next;
        delete victim;
        --_size;

        return next;
    }

    size_type erase(const Key & key) { 
        HashNode *& slot = _find(key);
        if(!slot) return 0;

        HashNode * victim = slot;
        slot = victim->next;
        delete victim;
        --_size;
        return 1;
    }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> & map, std::ostream & os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> & map, std::ostream & os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for(size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const * node = map._buckets[bucket];

        while(node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}
