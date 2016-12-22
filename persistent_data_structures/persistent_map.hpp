#ifndef PERSISTENT_MAP_H
#define PERSISTENT_MAP_H

#include <utility>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>
#include "persistent_avl_tree.hpp"

template <class Key, class Value, class Comparator = std::less<Key>>
class PersistentMap {
public:
    typedef Key key_type;
    typedef Value mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef Comparator comparator_type;
    typedef typename PersistentAVLTree<key_type, mapped_type, comparator_type>::iterator iterator;
    typedef typename PersistentAVLTree<key_type, mapped_type, comparator_type>::const_iterator const_iterator;

    PersistentMap() : _tree (std::make_shared<PersistentAVLTree<Key, Value, Comparator>>())
    {}
    PersistentMap(const PersistentMap& other) : _tree (std::make_shared<PersistentAVLTree<Key, Value, Comparator>>()) {
        insert(other.begin(), other.end());
    }
    PersistentMap(PersistentMap&& other) : _tree(other._tree) {
        other._tree = nullptr;
    }
    PersistentMap& operator=(const PersistentMap& other) {
        if (*this != other) {
            if (_tree) {
                _tree->clear();
            }
            insert(other.begin(), other.end());
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
        if (_tree) {
            clear();
        }
    }

    inline Value& at(const size_t version, const Key& key) {
        std::pair<iterator, bool> insertResult = _tree->insert(version, key);
        return (*(insertResult.first)).second;
    }
    inline Value& at(const size_t version, Key&& key) {
        Key keyCopy;
        std::swap(key, keyCopy);
        std::pair<iterator, bool> insertResult = _tree->insert(version, keyCopy);
        return (*(insertResult.first)).second;
    }
    inline iterator begin(const size_t version) noexcept {
        return _tree->begin(version);
    }
    inline iterator end() noexcept {
        return _tree->end();
    }
    inline const_iterator begin(const size_t version) const noexcept {
        return _tree->cbegin(version);
    }
    inline const_iterator end() const noexcept {
        return _tree->cend();
    }
    inline const_iterator cbegin(const size_t version) const noexcept {
        return _tree->cbegin(version);
    }
    inline const_iterator cend() const noexcept {
        return _tree->cend();
    }
    inline bool empty(const size_t version) const noexcept {
        return _tree->empty(version);
    }
    inline size_t size(const size_t version) const noexcept {
        return _tree->size(version);
    }
    inline void clear() noexcept {
        _tree->clear();
    }
    inline std::pair<iterator, bool> insert(const size_t version, const value_type& pair) {
        return _tree->insert(version, pair.first, pair.second);
    }
//    template<class InputIt>
//    void insert(InputIt first, InputIt last) {
//        for (auto it = first; it != last; ++it) {
//            insert(*it);
//        }
//    }
//    template <class... Args>
//    inline std::pair<iterator, bool> emplace(Args&&... args) {
//        return _tree->insert(std::forward<Args>(args)...);
//    }
    inline size_t erase(const size_t version, const Key& key) {
        return _tree->erase(version, key);
    }
    inline iterator find(const size_t version, const key_type& key) {
        return _tree->find(version, key);
    }
    inline const_iterator find(const size_t version, const key_type& key) const {
        return _tree->find(version, key);
    }

private:
    std::shared_ptr<PersistentAVLTree<Key, Value, Comparator>> _tree;
};

#endif // PERSISTENT_MAP_H
