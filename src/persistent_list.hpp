#ifndef PERSISTENT_LIST_HPP
#define PERSISTENT_LIST_HPP

#include <utility>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>
#include "persistent_avl_tree.hpp"
#include "persistent_vector.hpp"

template <class T>
class PersistentList {
public:
    typedef T value_type;
    typedef std::less<size_t> comparator_type;
    typedef typename PersistentAVLTree<T, T, comparator_type>::iterator iterator;
    typedef typename PersistentAVLTree<T, T, comparator_type>::const_iterator const_iterator;

    PersistentList() : _tree (PersistentAVLTree<size_t, T, comparator_type>())
    {}
    PersistentList(const PersistentList& other) : _tree (PersistentAVLTree<size_t, T, comparator_type>()) {
        insert(other.begin(), other.end());
    }
    PersistentList(PersistentList&& other) : _tree(other._tree) {
        other._tree = PersistentAVLTree<size_t, T, comparator_type>();
    }
    PersistentList& operator=(const PersistentList& other) {
        if (*this != other) {
            if (_tree) {
                _tree.clear();
            }
            insert(other.begin(), other.end());
        }
        return *this;
    }
    PersistentList& operator=(PersistentList&& other) {
        if (*this != other) {
            std::swap(_tree, other._tree);
        }
        return *this;
    }
    ~PersistentList() {
        clear();
    }

    value_type& front(const size_t version) {
        return *(_tree.find(version, 0));
    }
    const value_type& front(const size_t version) const {
        return *(_tree.find(version, 0));
    }
    value_type& back(const size_t version) {
        return *(_tree.find(version, size(version) - 1));
    }
    const value_type& back(const size_t version) const {
        return *(_tree.find(version, size(version) - 1));
    }

    inline iterator begin(const size_t version) noexcept {
        return _tree.begin(version);
    }
    inline iterator end() noexcept {
        return _tree.end();
    }
    inline const_iterator begin(const size_t version) const noexcept {
        return _tree.cbegin(version);
    }
    inline const_iterator end() const noexcept {
        return _tree.cend();
    }
    inline const_iterator cbegin(const size_t version) const noexcept {
        return _tree.cbegin(version);
    }
    inline const_iterator cend() const noexcept {
        return _tree.cend();
    }
    inline bool empty(const size_t version) const noexcept {
        return _tree.empty(version);
    }
    inline size_t size(const size_t version) const noexcept {
        return _tree.size(version);
    }
    inline void clear() noexcept {
        _tree.clear();
    }
    inline iterator insert(const size_t version, iterator pos, const value_type& value) {
//        return _tree.insert(version, pair.first, pair.second);
    }
    inline iterator insert(const size_t version, const_iterator pos, const value_type& value) {
//        return _tree.insert(version, pair.first, pair.second);
    }
    inline size_t erase(const size_t version, iterator pos) {
//        return _tree.erase(version, key);
    }
    inline size_t erase(const size_t version, const_iterator pos) {
//        return _tree.erase(version, key);
    }
    void push_back(const size_t version, const value_type& value) {
        _tree.insert(version, size(version), value);
    }
    void pop_back(const size_t version) {

    }
    void push_front(const size_t version, const value_type& value) {
//        _tree.insert(version, size(version), value);
    }
    void pop_front(const size_t version) {

    }

    PesistentVector<T> toVector() {

    }

private:
    PersistentAVLTree<size_t, T, comparator_type> _tree;
};

#endif // PERSISTENT_LIST_HPP
