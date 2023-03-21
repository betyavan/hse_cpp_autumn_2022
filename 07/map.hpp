#pragma once
#include <functional>
#include <tuple>
#include <stdexcept>

namespace avl {

    template <typename value_type>
    struct Node {

        Node(const value_type& pair) : m_pair(pair) {}
        value_type m_pair;
        Node* m_parent = nullptr;
        Node* m_left = nullptr;
        Node* m_right = nullptr;
        size_t m_height = 1;
    };

    template<
        class Key,
        class Value,
        class Comparator = std::less<Key>,
        class Allocator = std::allocator<Node<std::pair<const Key, Value>>>
    >
        class MyMap {
        public:
            using key_type = Key;
            using mapped_type = Value;
            using key_compare = Comparator;
            using value_type = std::pair<const Key, Value>;
        public:
            using Node_t = Node<value_type>;
            class const_iterator;
            class iterator;
            class const_reverse_iterator;
            class reverse_iterator;


            MyMap() = default;
            ~MyMap() { delete_tree(m_root); }

            iterator begin() { return iterator{ find_min(m_root) }; }
            iterator end() { return iterator(); }
            const_iterator begin() const { const_iterator{ find_min(m_root) }; }
            const_iterator end() const { return const_iterator{}; }
            const_iterator cbegin() const { return begin(); }
            const_iterator cend() const { return end(); }

            reverse_iterator rbegin() { return reverse_iterator{ find_max(m_root) }; }
            reverse_iterator rend() { return reverse_iterator{}; }
            const_reverse_iterator rbegin() const { return const_reverse_iterator{ find_max(m_root) }; }
            const_reverse_iterator rend() const { return const_reverse_iterator{}; }
            const_reverse_iterator crbegin() const { return const_reverse_iterator{ find_max(m_root) }; }
            const_reverse_iterator crend() const { return const_reverse_iterator{}; }

            std::tuple<iterator, bool> insert(const value_type& value);
            size_t erase(const Key& key);
            iterator find(const Key& key) { return iterator{ find_(m_root, key) }; }
            bool contains(const Key& key) const { return find_(m_root, key) != nullptr; }
            bool empty() const noexcept { return !m_root; }
            size_t size() const noexcept { return m_size; }
            void clear();
            mapped_type& operator[](const Key& key);
            mapped_type& at(const Key& key);
            const mapped_type& at(const Key& key) const { return at(key); }

        private:
            size_t height(Node_t* p) const noexcept { return p ? p->m_height : 0; }
            int balance_factor(Node_t* p) const noexcept { return height(p->m_right) - height(p->m_left); }
            void fixheight(Node_t* p);
            Node_t* rotate_right(Node_t* p);
            Node_t* rotate_left(Node_t* q);
            Node_t* balance(Node_t* p);
            Node_t* find_min(Node_t* p) { return p->m_left ? find_min(p->m_left) : p; }
            Node_t* remove_min(Node_t* p);
            Node_t* insert_(Node_t* p, const value_type& key);
            Node_t* remove_(Node_t* p, const Key& key);
            Node_t* find_(Node_t* p, const Key& key) const;
            void delete_tree(Node_t* p);
            Node_t* find_max(Node_t* p) { return p->m_right ? find_max(p->m_right) : p; }
        private:
            Node_t* m_root = nullptr;
            bool m_success_insert = false;
            Node_t* m_insert_it = nullptr;
            bool m_success_remove = false;
            size_t m_size = 0;
            Allocator allocator;
    };



    template<class Key, class Value, class Comparator, class Allocator>
    class MyMap<Key, Value, Comparator, Allocator>::const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        // using value_type = std::pair<const Key, Value>; 
        //using difference_type = typename size_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        const_iterator() = default;
        const_iterator(Node_t* it) : m_it(it) {}

        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);
        reference operator*() const { return m_it->m_pair; }
        pointer operator->() const { return &(m_it->m_pair); }
        bool operator==(const_iterator it) const { return m_it == it.m_it; }
        bool operator!=(const_iterator it) const { return !(*this == it); }

    private:
        Node_t* m_it = nullptr;
    };

    template<class Key, class Value, class Comparator, class Allocator>
    class MyMap<Key, Value, Comparator, Allocator>::iterator : public const_iterator {
    public:
        using pointer = value_type*;
        using reference = value_type&;

        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
        reference operator*();
        pointer operator->();

    };

    template<class Key, class Value, class Comparator, class Allocator>
    class MyMap<Key, Value, Comparator, Allocator>::const_reverse_iterator : public const_iterator {
    public:
        const_reverse_iterator& operator++();
        const_reverse_iterator operator++(int);
        const_reverse_iterator& operator--();
        const_reverse_iterator operator--(int);
    };

    template<class Key, class Value, class Comparator, class Allocator>
    class MyMap<Key, Value, Comparator, Allocator>::reverse_iterator : public iterator {
    public:
        reverse_iterator& operator++();
        reverse_iterator operator++(int);
        reverse_iterator& operator--();
        reverse_iterator operator--(int);
    };

    template<class Key, class Value, class Comparator, class Allocator>
    std::tuple<typename MyMap<Key, Value, Comparator, Allocator>::iterator, bool> MyMap<Key, Value, Comparator, Allocator>::insert(const value_type& value) {
        m_root = insert_(m_root, value);
        if (m_success_insert) ++m_size;
        return std::make_tuple(iterator(m_insert_it), m_success_insert);
    }

    template<class Key, class Value, class Comparator, class Allocator>
    size_t MyMap<Key, Value, Comparator, Allocator>::erase(const Key& key) {
        m_root = remove_(m_root, key);
        if (m_success_remove) {
            --m_size;
            return 1;
        }
        return 0;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    void MyMap<Key, Value, Comparator, Allocator>::clear() {
        delete_tree(m_root);
        m_root = nullptr;
        m_size = 0;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::mapped_type& MyMap<Key, Value, Comparator, Allocator>::operator[](const Key& key) {
        Node_t* p = find_(m_root, key);
        if (p) return p->m_pair.second;
        m_root = insert_(m_root, { key, Value() });
        ++m_size;
        return m_insert_it->m_pair.second;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::mapped_type& MyMap<Key, Value, Comparator, Allocator>::at(const Key& key) {
        Node_t* p = find_(m_root, key);
        if (p) return p->m_pair.second;
        throw std::out_of_range("Accessing a non-existent Key");
    }

    template<class Key, class Value, class Comparator, class Allocator>
    void MyMap<Key, Value, Comparator, Allocator>::fixheight(Node_t* p) {
        size_t left_h = height(p->m_left);
        size_t right_h = height(p->m_right);
        p->m_height = 1 + (left_h > right_h ? left_h : right_h);
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::Node_t* MyMap<Key, Value, Comparator, Allocator>::rotate_right(Node_t* p) {
        Node_t* q = p->m_left;
        q->m_parent = p->m_parent;
        p->m_parent = q;
        if (q->m_right) q->m_right->m_parent = p;
        p->m_left = q->m_right;
        q->m_right = p;
        fixheight(p);
        fixheight(q);
        return q;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::Node_t* MyMap<Key, Value, Comparator, Allocator>::rotate_left(Node_t* q) {
        Node_t* p = q->m_right;
        p->m_parent = q->m_parent;
        q->m_parent = p;
        if (p->m_left) p->m_left->m_parent = q;
        q->m_right = p->m_left;
        p->m_left = q;
        fixheight(p);
        fixheight(q);
        return p;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::Node_t* MyMap<Key, Value, Comparator, Allocator>::balance(Node_t* p) {
        fixheight(p);
        if (balance_factor(p) == 2) {
            if (balance_factor(p->m_right) < 0) {
                p->m_right = rotate_right(p->m_right);
                if (p->m_right) p->m_right->m_parent = p;
            }
            return rotate_left(p);
        }
        if (balance_factor(p) == -2) {
            if (balance_factor(p->m_left) > 0) {
                p->m_left = rotate_left(p->m_left);
                if (p->m_left) p->m_left->m_parent = p;
            }
            return rotate_right(p);
        }
        return p;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::Node_t* MyMap<Key, Value, Comparator, Allocator>::remove_min(Node_t* p) {
        if (!p->m_left) {
            // delete p->m_left;
            allocator.deallocate(p->m_left, 1);
            return p->m_right;
        }
        p->m_left = remove_min(p->m_left);
        if (p->m_left) p->m_left->m_parent = p;
        return balance(p);
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::Node_t* MyMap<Key, Value, Comparator, Allocator>::insert_(Node_t* p, const value_type& key) {
        if (!p) {
            m_success_insert = true;
            // Node* node = new Node(key);
            Node_t* node = allocator.allocate(1);
            new(node) Node_t(key);
            m_insert_it = node;
            return node;
        }
        if (key.first == p->m_pair.first) {
            m_success_insert = false;
            m_insert_it = p;
        }
        else if (key_compare{}(key.first, p->m_pair.first)) {
            p->m_left = insert_(p->m_left, key);
            p->m_left->m_parent = p;
        }
        else {
            p->m_right = insert_(p->m_right, key);
            p->m_right->m_parent = p;
        }

        return balance(p);
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::Node_t* MyMap<Key, Value, Comparator, Allocator>::remove_(Node_t* p, const Key& key) {
        if (!p) {
            m_success_remove = false;
            return nullptr;
        }
        if (key == p->m_pair.first) {
            m_success_remove = true;
            Node_t* q = p->m_left;
            Node_t* r = p->m_right;
            auto parent = p->m_parent;
            // delete p;
            allocator.deallocate(p, 1);
            if (!r) return q;
            Node_t* min = find_min(r);
            min->m_parent = parent;
            min->m_right = remove_min(r);
            if (min->m_right) min->m_right->m_parent = min;
            min->m_left = q;
            if (min->m_left) min->m_left->m_parent = min;
            return balance(min);
        }
        else if (key_compare{}(key, p->m_pair.first)) {
            p->m_left = remove_(p->m_left, key);
            if (p->m_left) p->m_left->m_parent = p;
        }
        else {
            p->m_right = remove_(p->m_right, key);
            if (p->m_right) p->m_right->m_parent = p;
        }

        return balance(p);
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::Node_t* MyMap<Key, Value, Comparator, Allocator>::find_(Node_t* p, const Key& key) const {
        if (!p) return nullptr;
        if (p->m_pair.first == key) return p;
        else if (key_compare{}(key, p->m_pair.first)) {
            if (p->m_left) return find_(p->m_left, key);
            return nullptr;
        }
        if (p->m_right) return find_(p->m_right, key);
        return nullptr;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    void MyMap<Key, Value, Comparator, Allocator>::delete_tree(Node_t* p) {
        if (!p) return;
        delete_tree(p->m_left);
        delete_tree(p->m_right);
        // delete p;
        allocator.deallocate(p, 1);
    }


    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::const_iterator& MyMap<Key, Value, Comparator, Allocator>::const_iterator::operator++() {
        if (m_it->m_right) {
            m_it = m_it->m_right;
            while (m_it->m_left) m_it = m_it->m_left;
            return *this;
        }
        auto parent = m_it->m_parent;
        while (parent && parent->m_left != m_it) {
            m_it = parent;
            parent = m_it->m_parent;
        }
        m_it = parent;
        return *this;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::const_iterator MyMap<Key, Value, Comparator, Allocator>::const_iterator::operator++(int) {
        const_iterator tmp = *this;
        this->operator++();
        return tmp;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::const_iterator& MyMap<Key, Value, Comparator, Allocator>::const_iterator::operator--() {
        if (m_it->m_left) {
            m_it = m_it->m_left;
            while (m_it->m_right) m_it = m_it->m_right;
            return *this;
        }
        auto parent = m_it->m_parent;
        while (parent && parent->m_right != m_it) {
            m_it = parent;
            parent = m_it->m_parent;
        }
        m_it = parent;
        return *this;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::const_iterator MyMap<Key, Value, Comparator, Allocator>::const_iterator::operator--(int) {
        const_iterator tmp = *this;
        this->operator--();
        return tmp;
    }


    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::iterator& MyMap<Key, Value, Comparator, Allocator>::iterator::operator++() {
        const_iterator::operator++();
        return *this;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::iterator MyMap<Key, Value, Comparator, Allocator>::iterator::operator++(int) {
        iterator tmp = *this;
        const_iterator::operator++();
        return tmp;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::iterator& MyMap<Key, Value, Comparator, Allocator>::iterator::operator--() {
        const_iterator::operator--();
        return *this;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::iterator MyMap<Key, Value, Comparator, Allocator>::iterator::operator--(int) {
        iterator tmp = *this;
        const_iterator::operator--();
        return tmp;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::iterator::reference MyMap<Key, Value, Comparator, Allocator>::iterator::operator*() {
        return const_cast<reference>(const_iterator::operator*());
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::iterator::pointer MyMap<Key, Value, Comparator, Allocator>::iterator::operator->() {
        return const_cast<pointer>(const_iterator::operator->());
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::const_reverse_iterator& MyMap<Key, Value, Comparator, Allocator>::const_reverse_iterator::operator++() {
        const_iterator::operator--();
        return *this;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::const_reverse_iterator MyMap<Key, Value, Comparator, Allocator>::const_reverse_iterator::operator++(int) {
        const_iterator::operator--(1);
        return *this;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::const_reverse_iterator& MyMap<Key, Value, Comparator, Allocator>::const_reverse_iterator::operator--() {
        const_iterator::operator++();
        return *this;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::const_reverse_iterator MyMap<Key, Value, Comparator, Allocator>::const_reverse_iterator::operator--(int) {
        const_iterator::operator++(1);
        return *this;
    }

    // nvfqehbfq
    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::reverse_iterator& MyMap<Key, Value, Comparator, Allocator>::reverse_iterator::operator++() {
        iterator::operator--();
        return *this;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::reverse_iterator MyMap<Key, Value, Comparator, Allocator>::reverse_iterator::operator++(int) {
        iterator::operator--(1);
        return *this;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::reverse_iterator& MyMap<Key, Value, Comparator, Allocator>::reverse_iterator::operator--() {
        iterator::operator++();
        return *this;
    }

    template<class Key, class Value, class Comparator, class Allocator>
    MyMap<Key, Value, Comparator, Allocator>::reverse_iterator MyMap<Key, Value, Comparator, Allocator>::reverse_iterator::operator--(int) {
        iterator::operator++(1);
        return *this;
    }

}