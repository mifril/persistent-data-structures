//#ifndef PERSISTENT_AVL_TREE_HPP
//#define PERSISTENT_AVL_TREE_HPP

//#include <algorithm>
//#include <iostream>

//template <class Key, class Value, class Comparator = std::less<Key>>
//class PersistentAVLTree {
//public:
//    typedef std::pair<const Key, Value> value_type;

//private:
//    struct Node {
//        Node* left;
//        Node* right;
//        value_type kvPair;
//        unsigned int height;

//        Node(const Key & newKey = Key(), const Value & newValue = Value()) :
//            left(nullptr), right(nullptr), kvPair(newKey, newValue), height(1)
//        {}

//        Key key() const {
//            return kvPair.first;
//        }
//        Key value() const {
//            return kvPair.second;
//        }
//    };

//public:
//    template<class T>
//    class TreeIterator : public std::iterator<std::forward_iterator_tag, T> {
//    public:
//        TreeIterator() : _cur(nullptr)
//        {}
//        TreeIterator(Node* node) : _cur(node)
//        {}
//        TreeIterator(PersistentAVLTree* tree) : _cur(tree->_root)
//        {}
//        TreeIterator(const TreeIterator& other) : _cur(other._cur)
//        {}
//        TreeIterator(TreeIterator&& other) : _cur(other._cur) {
//            other._cur = nullptr;
//        }
//        TreeIterator& operator=(const TreeIterator& other) {
//            if (*this != other) {
//                _cur = other._cur;
//            }
//            return* this;
//        }
//        TreeIterator& operator=(TreeIterator&& other) {
//            if (*this != other) {
//                std::swap(_cur, other._cur);
//            }
//            return* this;
//        }
//        TreeIterator& operator++() {
//            if (!_cur) {
//                return* this;
//            }
//            if (_cur->right) {
//                _cur = _cur->right;
//                while (_cur && _cur->left) {
//                    _cur = _cur->left;
//                }
//            } else {
//                Node* parent = _cur->parent;
//                if (nullptr == parent) {
//                    _cur = nullptr;
//                } else {
//                    while (parent && _cur == parent->right) {
//                        _cur = parent;
//                        parent = parent->parent;
//                    }
//                    if (_cur && _cur->right != parent) {
//                        _cur = parent;
//                    }
//                }
//            }
//            return* this;
//        }
//        TreeIterator operator++(int) {
//            TreeIterator tmp(*this);
//            operator++();
//            return tmp;
//        }
//        bool operator==(const TreeIterator& other) {
//            return _cur == other._cur;
//        }
//        bool operator==(const TreeIterator& other) const {
//            return _cur == other._cur;
//        }
//        bool operator!=(const TreeIterator& other) {
//            return _cur != other._cur;
//        }
//        bool operator!=(const TreeIterator& other) const {
//            return _cur != other._cur;
//        }
//        T& operator*() {
//            if (_cur) {
//                return _cur->kvPair;
//            } else {
//                throw new std::out_of_range("Iterator is out of range");
//            }
//        }
//        T* operator->() {
//            if (_cur) {
//                return &(_cur->kvPair);
//            } else {
//                throw new std::out_of_range("Iterator is out of range");
//            }
//        }
//    private:
//        Node* _cur;
//    };

//    typedef TreeIterator<value_type> iterator;
//    typedef TreeIterator<const value_type> const_iterator;

//    PersistentAVLTree() : _root(nullptr), _treeSize(0)
//    {}

//    ~PersistentAVLTree() {
//        if (_root) {
//            clear();
//        }
//    }

//    std::pair<iterator, bool> _insert(const Key& key, const Value& value = Value()) {
//        if (!_root) {
//            _root = new Node(key, value);
//            ++_treeSize;
//            return std::make_pair(iterator(_root), true);
//        }
//        _root = _insert(_root, key, value);
//        return std::make_pair(iterator(_root), true);
//    }

//    void erase(const Key& key) {
//        _root = _erase(_root, key);
//    }

//    inline iterator find(const Key& key) {
//        if (!_root) {
//            return end();
//        }
//        _root = splay(_root, key);
//        if (_root->kvPair.first == key) {
//            return iterator(_root);
//        }
//        return end();
//    }

//    inline void clear() {
//        if (_root) {
//            while (_root) {
//                erase(_root->key());
//            }
//        }
//    }

//    inline bool empty() const {
//        return _root == nullptr;
//    }

//    inline size_t size() const {
//        return _treeSize;
//    }

//    inline iterator begin() noexcept {
//        Node* cur = _root;
//        while (cur->left) {
//            cur = cur->left;
//        }
//        return iterator(cur);
//    }

//    inline iterator end() noexcept {
//        return iterator();
//    }

//    inline const_iterator cbegin() const noexcept {
//        Node* cur = _root;
//        while (cur->left) {
//            cur = cur->left;
//        }
//        return const_iterator(cur);
//    }

//    inline const_iterator cend() const noexcept {
//        return const_iterator();
//    }

//    void preOrder() {
//        std::cout << "Size: " << size() << "; ";
//       _preOrder(_root);
//       std::cout << std::endl;
//    }

//private:
//    Node* _root;
//    size_t _treeSize;
//    Comparator _comparator;

//    void _preOrder(Node *cur) {
//        if (cur) {
//            std::cout << cur->key() << ' ';
//            _preOrder(cur->left);
//            _preOrder(cur->right);
//        }
//    }

//    unsigned int _height(Node* p) {
//        return p ? p->height : 0;
//    }

//    int _getBalance(Node* p) {
//        return _height(p->right) - _height(p->left);
//    }

//    void _fixHeight(Node* p) {
//        unsigned int hl = _height(p->left);
//        unsigned int hr = _height(p->right);
//        p->height = (hl > hr ? hl : hr) + 1;
//    }

//    // правый поворот вокруг p
//    Node* _rotateRight(Node* p) {
//        Node* q = p->left;
//        p->left = q->right;
//        q->right = p;
//        _fixHeight(p);
//        _fixHeight(q);
//        return q;
//    }

//    // левый поворот вокруг q
//    Node* _rotateleft(Node* q) {
//        Node* p = q->right;
//        q->right = p->left;
//        p->left = q;
//        _fixHeight(q);
//        _fixHeight(p);
//        return p;
//    }

//    // балансировка узла p
//    Node* _balance(Node* p) {
//        _fixHeight(p);
//        if (_getBalance(p) == 2) {
//            if (_getBalance(p->right) < 0) {
//                p->right = _rotateRight(p->right);
//            }
//            return _rotateleft(p);
//        }
//        if (_getBalance(p) == -2) {
//            if (_getBalance(p->left) > 0) {
//                p->left = _rotateleft(p->left);
//            }
//            return _rotateRight(p);
//        }
//        return p; // балансировка не нужна
//    }

//    // вставка ключа k в дерево с корнем p
//    Node* _insert(Node* p, const Key& key, const Value& value) {
//        if (!p) {
//            ++_treeSize;
//            return new Node(key, value);
//        }
//        if (_comparator(key, p->key())) {
//            p->left = _insert(p->left, key, value);
//        } else if (_comparator(p->key(), key)) {
//            p->right = _insert(p->right, key, value);
//        } else {
//            return p;
//        }
//        return _balance(p);
//    }

//    // поиск узла с минимальным ключом в дереве p
//    Node* _findMin(Node* p) {
//        return p->left ? _findMin(p->left) : p;
//    }

//    // удаление узла с минимальным ключом из дерева p
//    Node* _removeMin(Node* p) {
//        if (!p->left) {
//            return p->right;
//        }
//        p->left = _removeMin(p->left);
//        return _balance(p);
//    }

//    // удаление ключа k из дерева p
//    Node* _erase(Node* p, const Key& key) {
//        if (!p) {
//            return nullptr;
//        }
//        if (_comparator(key, p->key())) {
//            p->left = _erase(p->left,key);
//        } else if (_comparator(p->key(), key)) {
//            p->right = _erase(p->right,key);
//        } else {
//            //  k == p->key
//            Node* q = p->left;
//            Node* r = p->right;
//            --_treeSize;
//            delete p;
//            if (!r) {
//                return q;
//            }
//            Node* min = _findMin(r);
//            min->right = _removeMin(r);
//            min->left = q;
//            return _balance(min);
//        }
//        return _balance(p);
//    }
//};

//#endif // PERSISTENT_AVL_TREE_HPP
