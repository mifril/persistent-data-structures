#ifndef PERSISTENT_AVL_TREE_HPP
#define PERSISTENT_AVL_TREE_HPP

#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>

template <class Key, class Value, class Comparator = std::less<Key>>
class PersistentAVLTree {
public:
    typedef std::pair<const Key, Value> value_type;

private:
    struct Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        value_type kvPair;
        unsigned int height;

        Node(const Key & newKey = Key(), const Value & newValue = Value()) :
            left(nullptr), right(nullptr), kvPair(newKey, newValue), height(1)
        {}

        Key key() const {
            return kvPair.first;
        }
        Value value() const {
            return kvPair.second;
        }
    };

    struct Version {
        std::shared_ptr<Node> root;
        size_t version;
        size_t size;

        Version(std::shared_ptr<Node> root_, size_t version_, const size_t size_) :
            root(root_), version(version_), size(size_)
        {}

        bool operator==(const Version& other) {
            return root == other.root && size == other.size && version == other.version;
        }
        bool operator==(const Version& other) const {
            return root == other.root && size == other.size && version == other.version;
        }
    };

    template<class T>
    class TreeIterator : public std::iterator<std::forward_iterator_tag, T> {
    public:
        TreeIterator() : _cur(nullptr)
        {}
        TreeIterator(std::shared_ptr<Node> node) : _cur(node)
        {}
        TreeIterator(PersistentAVLTree* tree) : _cur(tree->_root)
        {}
        TreeIterator(const TreeIterator& other) : _cur(other._cur)
        {}
        TreeIterator(TreeIterator&& other) : _cur(other._cur) {
            other._cur = nullptr;
        }
        TreeIterator& operator=(const TreeIterator& other) {
            if (*this != other) {
                _cur = other._cur;
            }
            return* this;
        }
        TreeIterator& operator=(TreeIterator&& other) {
            if (*this != other) {
                std::swap(_cur, other._cur);
            }
            return* this;
        }
        TreeIterator& operator++() {
            if (!_cur) {
                return* this;
            }
            if (_cur->right) {
                _cur = _cur->right;
                while (_cur && _cur->left) {
                    _cur = _cur->left;
                }
            } else {
                std::shared_ptr<Node> parent = _cur->parent;
                if (nullptr == parent) {
                    _cur = nullptr;
                } else {
                    while (parent && _cur == parent->right) {
                        _cur = parent;
                        parent = parent->parent;
                    }
                    if (_cur && _cur->right != parent) {
                        _cur = parent;
                    }
                }
            }
            return* this;
        }
        TreeIterator operator++(int) {
            TreeIterator tmp(*this);
            operator++();
            return tmp;
        }
        bool operator==(const TreeIterator& other) {
            return _cur == other._cur;
        }
        bool operator==(const TreeIterator& other) const {
            return _cur == other._cur;
        }
        bool operator!=(const TreeIterator& other) {
            return _cur != other._cur;
        }
        bool operator!=(const TreeIterator& other) const {
            return _cur != other._cur;
        }
        T& operator*() {
            if (_cur) {
                return _cur->kvPair;
            } else {
                throw new std::out_of_range("Iterator is out of range");
            }
        }
        T* operator->() {
            if (_cur) {
                return &(_cur->kvPair);
            } else {
                throw new std::out_of_range("Iterator is out of range");
            }
        }
    private:
        std::shared_ptr<Node> _cur;
    };

public:
    typedef TreeIterator<value_type> iterator;
    typedef TreeIterator<const value_type> const_iterator;

    PersistentAVLTree() {
        _versions.push_back(Version(nullptr, 0, 0));
    }

    ~PersistentAVLTree() {
        if (!_versions.empty()) {
            clear();
        }
    }

    bool operator==(const PersistentAVLTree& other) {
        return _versions == other._versions;
    }
    bool operator==(const PersistentAVLTree& other) const {
        return _versions == other._versions;
    }
    bool operator!=(const PersistentAVLTree& other) {
        return _versions != other._versions;
    }
    bool operator!=(const PersistentAVLTree& other) const {
        return _versions != other._versions;
    }

    // invalid return value. now returns iterator to new Root, but should return iterator to inserted element
    std::pair<iterator, bool> insert(const size_t srcVersion, const Key& key, const Value& value) {
        if (_versions.size() - 1 < srcVersion) {
            throw new std::out_of_range("Invalid version: " + srcVersion);
        }

        auto root = _versions[srcVersion].root;
        auto size = _versions[srcVersion].size;

        if (!root) {
            std::shared_ptr<Node> newRoot = std::make_shared<Node>(key, value);
            _versions.push_back(Version(newRoot, srcVersion + 1, size + 1));
            return std::make_pair(iterator(newRoot), true);
        }
        std::shared_ptr<Node> newRoot = _insert(root, key, value);
        _versions.push_back(Version(newRoot, srcVersion + 1, size + 1));
        return std::make_pair(iterator(newRoot), true);
    }

    void erase(const size_t srcVersion, const Key& key) {
        if (_versions.size() < srcVersion) {
            throw new std::out_of_range("Invalid version: " + srcVersion);
        }

        auto root = _versions[srcVersion].root;
        auto size = _versions[srcVersion].size;
        std::shared_ptr<Node> newRoot = _erase(root, key);
        _versions.push_back(Version(newRoot, srcVersion + 1, size - 1));
    }

    inline iterator find(const size_t version, const Key& key) {
        auto cur = _versions[version].root;
        if (!cur) {
            return end();
        }
        while (cur) {
            if (_comparator(key, cur->key())) {
                cur = cur->left;
            } else if (_comparator(cur->key(), key)) {
                cur = cur->right;
            } else {
                break;
            }
        }
        if (cur) {
            return iterator(cur);
        }
        return end();
    }

    inline void clear() {
        _versions.clear();
    }

    inline bool empty(const size_t version) const {
        return _versions[version].root == nullptr;
    }

    inline size_t size(const size_t version) const {
        return _versions[version].size;
    }

    inline size_t versionsNumber() const {
        return _versions.size();
    }

    inline iterator begin(const size_t version) noexcept {
        std::shared_ptr<Node> cur = _versions[version].root;
        while (cur->left) {
            cur = cur->left;
        }
        return iterator(cur);
    }

    inline iterator end() noexcept {
        return iterator();
    }

    inline const_iterator cbegin(const size_t version) const noexcept {
        std::shared_ptr<Node> cur = _versions[version].root;
        while (cur->left) {
            cur = cur->left;
        }
        return const_iterator(cur);
    }

    inline const_iterator cend() const noexcept {
        return const_iterator();
    }

    void preOrder(const size_t version) {
        std::cout << "Size: " << size(version) << "; ";
       _preOrder(_versions[version].root);
       std::cout << std::endl;
    }

private:
    std::vector<Version> _versions;
    Comparator _comparator;

    std::shared_ptr<Node> _copyNode(std::shared_ptr<Node> node) {
        std::shared_ptr<Node> copy = std::make_shared<Node>(node->key(), node->value());
        copy->left = node->left;
        copy->right = node->right;
        return copy;
    }

    void _preOrder(Node *node) {
        if (node) {
            std::cout << node->key() << ' ';
            _preOrder(node->left);
            _preOrder(node->right);
        }
    }

    unsigned int _height(std::shared_ptr<Node> node) {
        return node ? node->height : 0;
    }

    int _getBalance(std::shared_ptr<Node> node) {
        return _height(node->right) - _height(node->left);
    }

    void _fixHeight(std::shared_ptr<Node> node) {
        unsigned int hl = _height(node->left);
        unsigned int hr = _height(node->right);
        node->height = (hl > hr ? hl : hr) + 1;
    }

    std::shared_ptr<Node> _rotateRight(std::shared_ptr<Node> node) {
        std::shared_ptr<Node> l = node->left;
        node->left = l->right;
        l->right = node;
        _fixHeight(node);
        _fixHeight(l);
        return l;
    }

    std::shared_ptr<Node> _rotateleft(std::shared_ptr<Node> node) {
        std::shared_ptr<Node> r = node->right;
        node->right = r->left;
        r->left = node;
        _fixHeight(node);
        _fixHeight(r);
        return r;
    }

    std::shared_ptr<Node> _balance(std::shared_ptr<Node> node) {
        _fixHeight(node);
        if (_getBalance(node) == 2) {
            if (_getBalance(node->right) < 0) {
                node->right = _rotateRight(node->right);
            }
            return _rotateleft(node);
        }
        if (_getBalance(node) == -2) {
            if (_getBalance(node->left) > 0) {
                node->left = _rotateleft(node->left);
            }
            return _rotateRight(node);
        }
        return node;
    }

    std::shared_ptr<Node> _insert(std::shared_ptr<Node> root, const Key& key, const Value& value) {
        if (!root) {
            return std::make_shared<Node>(key, value);
        }
        std::shared_ptr<Node> copyP = _copyNode(root);
        if (_comparator(key, copyP->key())) {
            copyP->left = _insert(copyP->left, key, value);
        } else if (_comparator(copyP->key(), key)) {
            copyP->right = _insert(copyP->right, key, value);
        } else {
            return copyP;
        }
        return _balance(copyP);
    }

    std::shared_ptr<Node> _findMin(std::shared_ptr<Node> root) {
        return root->left ? _findMin(root->left) : root;
    }

    std::shared_ptr<Node> _removeMin(std::shared_ptr<Node> root) {
        if (!root->left) {
            return root->right;
        }
        root->left = _removeMin(root->left);
        return _balance(root);
    }

    std::shared_ptr<Node> _erase(std::shared_ptr<Node> root, const Key& key) {
        if (!root) {
            return nullptr;
        }

        std::shared_ptr<Node> copyP = _copyNode(root);
        if (_comparator(key, copyP->key())) {
            copyP->left = _erase(copyP->left,key);
        } else if (_comparator(copyP->key(), key)) {
            copyP->right = _erase(copyP->right,key);
        } else {
            std::shared_ptr<Node> l = copyP->left;
            std::shared_ptr<Node> r = copyP->right;
            if (!r) {
                return l;
            }
            std::shared_ptr<Node> min = _findMin(r);
            min->right = _removeMin(r);
            min->left = l;
            return _balance(min);
        }
        return _balance(copyP);
    }
};

#endif // PERSISTENT_AVL_TREE_HPP
