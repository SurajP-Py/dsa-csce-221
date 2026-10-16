#pragma once

#include <functional> // std::less
#include <iostream>
#include <queue> // std::queue
#include <utility> // std::pair

template <typename K, typename V, typename Comparator = std::less<K>>
class BinarySearchTree
{
  public:
    using key_type        = K;
    using value_type      = V;
    using key_compare     = Comparator;
    using pair            = std::pair<key_type, value_type>;
    using pointer         = pair*;
    using const_pointer   = const pair*;
    using reference       = pair&;
    using const_reference = const pair&;
    using difference_type = ptrdiff_t;
    using size_type       = size_t;

  private:
    struct BinaryNode
    {
        pair element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode( const_reference theElement, BinaryNode *lt, BinaryNode *rt )
          : element{ theElement }, left{ lt }, right{ rt } { }
        
        BinaryNode( pair && theElement, BinaryNode *lt, BinaryNode *rt )
          : element{ std::move( theElement ) }, left{ lt }, right{ rt } { }
    };

    using node           = BinaryNode;
    using node_ptr       = node*;
    using const_node_ptr = const node*;

    node_ptr _root;
    size_type _size;
    key_compare comp;

  public:
    BinarySearchTree() : _root{ nullptr }, _size{ 0 }, comp{} {
        // TODO
    }
    BinarySearchTree( const BinarySearchTree & rhs ) : _root{ nullptr}, _size{ rhs._size }, comp{ rhs.comp } {
        // TODO
        _root = clone(rhs._root);
    }
    BinarySearchTree( BinarySearchTree && rhs ) : _root{ rhs._root }, _size{ rhs._size }, comp{ std::move(rhs.comp) }  {
        // TODO
        rhs._root = nullptr;
        rhs._size = 0;
    }
    ~BinarySearchTree() {
        // TODO
        clear( _root );
    }

    const_reference min() const { return min( _root )->element; }
    const_reference max() const { return max( _root )->element; }
    const_reference root() const {
        // TODO
        return _root->element;
    }
    bool contains( const key_type & x ) const { return contains( x, _root ); }
    value_type & find( const key_type & key ) { return find( key, _root )->element.second; }
    const value_type & find( const key_type & key ) const { return find( key, _root )->element.second; }
    bool empty() const {
        // TODO
        return _size == 0;
    }
    size_type size() const {
        // TODO
        return _size;
    }

    void clear() {
        clear( _root );
        _size = 0;
    }
    void insert( const_reference x ) { insert( x, _root ); }
    void insert( pair && x ) { insert( std::move( x ), _root ); }
    void erase( const key_type & x ) { erase(x, _root); }

    BinarySearchTree & operator=( const BinarySearchTree & rhs ) {
        // TODO
        if (this != &rhs) {
            clear(_root);
            _root = clone(rhs._root);
            _size = rhs._size;
            comp = rhs.comp;
        }

        return *this;
    }
    BinarySearchTree & operator=( BinarySearchTree && rhs ) {
        // TODO
        if (this != &rhs) {
            clear(_root);
            _root = clone(rhs._root);
            _size = rhs._size;
            comp = std::move(rhs.comp);
            rhs._root = nullptr;
            rhs._size = 0;
        }

        return *this;
    }

  private:
    void insert( const_reference x, node_ptr & t ) {
        // TODO
        if (t == nullptr) {
            t = new node{x, nullptr, nullptr};
            _size++;
        } else if (comp(x.first, t->element.first)) {
            insert(x, t->left);
        } else if (comp(t->element.first, x.first)) {
            insert(x, t->right);
        }
    }
    void insert( pair && x, node_ptr & t ) {
        // TODO
        if (t == nullptr) {
            t = new node{std::move(x), nullptr, nullptr};
            _size++;
        } else if (comp(x.first, t->element.first)) {
            insert(std::move(x), t->left);
        } else if (comp(t->element.first, x.first)) {
            insert(std::move(x), t->right);
        }
    }

    void erase( const key_type & x, node_ptr & t ) {
        // TODO
        if(t == nullptr) return;

        if(comp(x, t->element.first)) erase(x, t->left);
        else if(comp(t->element.first, x)) erase(x, t->right);
        else if(t->left != nullptr && t->right != nullptr) {
            node_ptr m = t->right;
            while(m->left != nullptr) m = m->left;
            t->element = m->element;
            erase(t->element.first,  t->right);
        } else {
            node_ptr old = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete old;
            _size--;
        }
    }

    const_node_ptr min( const_node_ptr t ) const {
        // TODO
        if(t == nullptr) return nullptr;
        while(t->left != nullptr) t = t->left;
        return t;
    }
    const_node_ptr max( const_node_ptr t ) const {
        // TODO
        if(t == nullptr) return nullptr;
        while(t->right != nullptr) t = t->right;
        return t;
    }

    bool contains( const key_type & x, const_node_ptr t ) const {
        // TODO
        while(t != nullptr){
            if(comp(x, t->element.first)) t = t->left;
            else if(comp(t->element.first, x)) t = t->right;
            else return true;
        }
        return false;
    }
    node_ptr find( const key_type & key, node_ptr t ) {
        // TODO
        while(t != nullptr){
            if(comp(key, t->element.first)) t = t->left;
            else if(comp(t->element.first, key)) t = t->right;
            else return t;
        }
        return nullptr;
    }
    const_node_ptr find( const key_type & key, const_node_ptr t ) const {
        // TODO
        while(t != nullptr){
            if(comp(key, t->element.first)) t = t->left;
            else if(comp(t->element.first, key)) t = t->right;
            else return t;
        }
        return nullptr;
    }

    void clear( node_ptr & t ) {
        // TODO
        if(t != nullptr) {
            clear(t->left);
            clear(t->right);
            delete t;
        }
        t = nullptr;
    }
    
    node_ptr clone ( const_node_ptr t ) const {
        // TODO
        if(t == nullptr) return nullptr;
        return new node{t->element, clone(t->left), clone(t->right)};
    }

  public:
    template <typename KK, typename VV, typename CC>
    friend void printLevelByLevel( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out );

    template <typename KK, typename VV, typename CC>
    friend std::ostream& printNode(std::ostream& o, const typename BinarySearchTree<KK, VV, CC>::node& bn);

    template <typename KK, typename VV, typename CC>
    friend void printTree( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out );

    template <typename KK, typename VV, typename CC>
    friend void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream & out, unsigned depth );

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr node, 
        std::ostream & out,
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev
    );

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        const BinarySearchTree<KK, VV, CC> & bst, 
        std::ostream & out
    );
};

template <typename KK, typename VV, typename CC>
std::ostream& printNode(std::ostream & o, const typename BinarySearchTree<KK, VV, CC>::node & bn) {
    return o << '(' << bn.element.first << ", " << bn.element.second << ')';
}

template <typename KK, typename VV, typename CC>
void printLevelByLevel( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out = std::cout ) {
    using node = typename BinarySearchTree<KK, VV, CC>::node;
    using node_ptr = typename BinarySearchTree<KK, VV, CC>::node_ptr;
    using const_node_ptr = typename BinarySearchTree<KK, VV, CC>::const_node_ptr;
    
    // TODO -- Guide in Instructions
}

template <typename KK, typename VV, typename CC>
void printTree( const BinarySearchTree<KK, VV, CC> & bst, std::ostream & out = std::cout ) { printTree<KK, VV, CC>(bst._root, out ); }

template <typename KK, typename VV, typename CC>
void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream & out, unsigned depth = 0 ) {
    if (t != nullptr) {
        printTree<KK, VV, CC>(t->right, out, depth + 1);
        for (unsigned i = 0; i < depth; ++i)
            out << '\t';
        printNode<KK, VV, CC>(out, *t) << '\n';
        printTree<KK, VV, CC>(t->left, out, depth + 1);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr node, 
    std::ostream & out,
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev = nullptr
) {
    if(node) {
        std::hash<KK> khash{};

        out << "\t" "node_" << (uint32_t) khash(node->element.first)
            << "[label=\"" << node->element.first 
            << " [" << node->element.second << "]\"];" << std::endl;
        
        if(prev)
            out << "\tnode_" << (uint32_t) khash(prev->element.first) <<  " -> ";
        else
            out << "\t";
        
        out << "node_" << (uint32_t) khash(node->element.first) << ";" << std::endl;
    
        vizTree<KK, VV, CC>(node->left, out, node);
        vizTree<KK, VV, CC>(node->right, out, node);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    const BinarySearchTree<KK, VV, CC> & bst, 
    std::ostream & out = std::cout
) {
    out << "digraph Tree {" << std::endl;
    vizTree<KK, VV, CC>(bst._root, out);
    out << "}" << std::endl;
}
