#ifndef PERSISTENT_MAP_H
#define PERSISTENT_MAP_H

#include <utility>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>
#include "persistent_avl_tree.hpp"

template <class Key, class Value, class Comparator = std::less<Key> >
class PersistentMap {
public:
    typedef Key key_type;
    typedef Value mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef Comparator comparator_type;
    typedef typename PersistentAVLTree<key_type, mapped_type, comparator_type>::iterator iterator;

    PersistentMap() : _tree (PersistentAVLTree<Key, Value, Comparator>())
    {}
    PersistentMap(const PersistentMap& other) : _tree (other._tree)
    {}
    PersistentMap(PersistentMap&& other) : _tree(other._tree) {
        other._tree = PersistentAVLTree<Key, Value, Comparator>();
    }
    PersistentMap& operator=(const PersistentMap& other) {
        if (*this != other) {
            _tree.clear();
            _tree = other._tree;
        }
        return *this;
    }
    PersistentMap& operator=(PersistentMap&& other) {
        if (*this != other) {
            std::swap(_tree, other._tree);
        }
        return *this;
    }
    ~PersistentMap() {
        clear();
    }

    bool operator==(const PersistentMap& other) {
        return _tree == other._tree;
    }
    bool operator==(const PersistentMap& other) const {
        return _tree == other._tree;
    }
    bool operator!=(const PersistentMap& other) {
        return _tree != other._tree;
    }
    bool operator!=(const PersistentMap& other) const {
        return _tree != other._tree;
    }

    // Will not create new element (key, Value()) if 'key' does not exist in the tree
    inline const mapped_type& at(const size_t version, const Key& key) {
        auto findResult = _tree.find(version, key);
        return (*(findResult)).second;
    }
    inline const mapped_type& at(const size_t version, Key&& key) {
        Key keyCopy;
        std::swap(key, keyCopy);
        return at(version, keyCopy);
    }

    inline iterator begin(const size_t version) const noexcept {
        return _tree.begin(version);
    }
    inline iterator end() const noexcept {
        return _tree.end();
    }

    inline bool empty(const size_t version) const noexcept {
        return _tree.empty(version);
    }
    inline size_t size(const size_t version) const noexcept {
        return _tree.size(version);
    }
    inline size_t versionsNumber() const {
        return _tree.versionsNumber();
    }
    inline void clear() noexcept {
        _tree.clear();
    }
    inline std::pair<iterator, bool> insert(const size_t version, const value_type& pair) {
        return _tree.insert(version, pair.first, pair.second);
    }
    inline void erase(const size_t version, const Key& key) {
        return _tree.erase(version, key);
    }
    inline iterator find(const size_t version, const key_type& key) const {
        return _tree.find(version, key);
    }

private:
    PersistentAVLTree<Key, Value, Comparator> _tree;
};

#endif // PERSISTENT_MAP_H
