#ifndef PERSISTENT_LIST_HPP
#define PERSISTENT_LIST_HPP

#include <utility>
#include <functional>
#include <iterator>
#include <memory>
#include <vector>
#include <utility>

template <class T>
class PersistentList {
public:
    typedef T value_type;
    typedef std::less<size_t> comparator_type;

private:
    struct Node {
        std::shared_ptr<Node> next;
        value_type value;

        Node(const value_type & value_) : value(value_)
        {}
    };

    struct VersionData {
        std::shared_ptr<Node> root;
        size_t size;
        size_t version;

        VersionData(std::shared_ptr<Node> root_, size_t version_, const size_t size_) :
            root(root_), size(size_), version(version_)
        {}
        
        value_type& front() {
            return root->value;
        }

        bool operator==(const VersionData& other) {
            return root == other.root && size == other.size && version == other.version;
        }
        bool operator==(const VersionData& other) const {
            return root == other.root && size == other.size && version == other.version;
        }
    };

    template<class Y>
    class ListIterator : public std::iterator<std::forward_iterator_tag, Y> {
    public:
        ListIterator() : _cur(nullptr)
        {}
        ListIterator(std::shared_ptr<Node> node) : _cur(node)
        {}
        ListIterator(const ListIterator& other) : _cur(other._cur)
        {}
        ListIterator(ListIterator&& other) : _cur(other._cur) {
            other._cur = nullptr;
        }
        ListIterator& operator=(const ListIterator& other) {
            if (*this != other) {
                _cur = other._cur;
            }
            return* this;
        }
        ListIterator& operator=(ListIterator&& other) {
            if (*this != other) {
                std::swap(_cur, other._cur);
            }
            return* this;
        }
        ListIterator& operator++() {
            if (_cur) {
                _cur = _cur->next;
            }
            return* this;
        }
        ListIterator operator++(int) {
            ListIterator tmp(*this);
            operator++();
            return tmp;
        }
        bool operator==(const ListIterator& other) {
            return _cur == other._cur;
        }
        bool operator==(const ListIterator& other) const {
            return _cur == other._cur;
        }
        bool operator!=(const ListIterator& other) {
            return _cur != other._cur;
        }
        bool operator!=(const ListIterator& other) const {
            return _cur != other._cur;
        }
        T& operator*() {
            if (_cur) {
                return _cur->value;
            } else {
                throw new std::out_of_range("Iterator is out of range");
            }
        }
        T* operator->() {
            if (_cur) {
                return &(_cur->value);
            } else {
                throw new std::out_of_range("Iterator is out of range");
            }
        }
    private:
        std::shared_ptr<Node> _cur;
    };


public:
    typedef ListIterator<value_type> iterator;
    typedef ListIterator<const value_type> const_iterator;

    PersistentList() {
        _versions.push_back(VersionData(nullptr, 0, 0));
    }
    PersistentList(const PersistentList& other) : _versions (other._versions)
    {}
    PersistentList(PersistentList&& other) : _versions(other._versions) {
        other.clear();
    }
    PersistentList& operator=(const PersistentList& other) {
        if (*this != other) {
            if (!_versions.empty()) {
                clear();
            }
            _versions = other._versions;
        }
        return *this;
    }
    PersistentList& operator=(PersistentList&& other) {
        if (*this != other) {
            std::swap(_versions, other._versions);
        }
        return *this;
    }
    ~PersistentList() {
        clear();
    }

    bool operator==(const PersistentList& other) {
        return _versions == other._versions && _root == other._root;
    }
    bool operator==(const PersistentList& other) const {
        return _versions == other._versions && _root == other._root;
    }
    bool operator!=(const PersistentList& other) {
        return !operator==(other);
    }
    bool operator!=(const PersistentList& other) const {
        return !operator==(other);
    }

    value_type& front(const size_t srcVersion) {
        if (_versions.empty()) {
            throw new std::out_of_range("List is empty");
        }
        if (!_versions[srcVersion].root) {
            throw new std::out_of_range("This version is empty: " + srcVersion);
        } 
        return _versions[srcVersion].front();
    }
    const value_type& front(const size_t srcVersion) const {
        if (_versions.empty()) {
            throw new std::out_of_range("List is empty");
        }
        if (!_versions[srcVersion].root) {
            throw new std::out_of_range("This version is empty: " + srcVersion);
        }
        return _versions[srcVersion].front();
    }
    value_type& back(const size_t srcVersion) {
        if (_versions.empty()) {
            throw new std::out_of_range("List is empty");
        }
        
        auto cur =  _versions[srcVersion].root;
        if (!cur) {
            throw new std::out_of_range("This version is empty: " + srcVersion);
        }
        while (cur->next) {
            cur = cur->next;
        }
        return cur->value;
    }
    const value_type& back(const size_t srcVersion) const {
        if (_versions.empty()) {
            throw new std::out_of_range("List is empty");
        }
        
        auto cur =  _versions[srcVersion].root;
        if (!cur) {
            throw new std::out_of_range("This version is empty: " + srcVersion);
        }
        while (cur->next) {
            cur = cur->next;
        }
        return cur->value;
    }

    inline iterator begin(const size_t srcVersion) noexcept {
        return iterator(_versions[srcVersion].root);
    }
    inline iterator end() noexcept {
        return iterator(nullptr);
    }
    inline const_iterator begin(const size_t srcVersion) const noexcept {
       return const_iterator(_versions[srcVersion].root);
    }
    inline const_iterator end() const noexcept {
        return const_iterator(nullptr);
    }
    inline const_iterator cbegin(const size_t srcVersion) const noexcept {
        return const_iterator(_versions[srcVersion].root);
    }
    inline const_iterator cend() const noexcept {
        return const_iterator(nullptr);
    }
    
    inline bool empty(const size_t srcVersion) const noexcept {
        return _versions[srcVersion].size == 0;
    }
    inline size_t size(const size_t srcVersion) const noexcept {
        return _versions[srcVersion].size;
    }
    inline void clear() noexcept {
        _versions.clear();
    }
    inline iterator insert(const size_t srcVersion, iterator pos, const value_type& value) {
        if (_versions.size() - 1 < srcVersion) {
            throw new std::out_of_range("Invalid version: " + srcVersion);
        }
        auto newNode = std::make_shared<Node>(value);
        auto root = _versions[srcVersion].root;
        auto size = _versions[srcVersion].size;
        if (!root) {
            _versions.push_back(VersionData(newNode, srcVersion + 1, size + 1));
        } else if (pos == begin(srcVersion)) {
            newNode->next = root;
            _versions.push_back(VersionData(newNode, srcVersion + 1, size + 1));
        } else {
            auto curOld = root;
            auto curOldIt = iterator(root);
            std::shared_ptr<Node> prevNew = nullptr;
            std::shared_ptr<Node> copyRoot = nullptr;
            while (curOldIt != pos) {
                auto copyCur = std::make_shared<Node>(*curOldIt);
                if (curOldIt == begin(srcVersion)) {
                    copyRoot = copyCur;
                }
                if (prevNew) {
                    prevNew->next = copyCur;
                    prevNew = prevNew->next;
                } else {
                    prevNew = copyCur;
                }
                ++curOldIt;
                curOld = curOld->next;
            }
            prevNew->next = newNode;
            newNode->next = curOld;
            _versions.push_back(VersionData(copyRoot, srcVersion + 1, size + 1));
        }
        return iterator(newNode);
    }

    inline iterator insert(const size_t srcVersion, const_iterator pos, const value_type& value) {
        if (_versions.size() - 1 < srcVersion) {
            throw new std::out_of_range("Invalid version: " + srcVersion);
        }
        auto newNode = std::make_shared<Node>(value);
        auto root = _versions[srcVersion].root;
        auto size = _versions[srcVersion].size;
        if (!root) {
            _versions.push_back(VersionData(newNode, srcVersion + 1, size + 1));
        } else if (pos == begin(srcVersion)) {
            newNode->next = root;
            _versions.push_back(VersionData(newNode, srcVersion + 1, size + 1));
        } else {
            auto curOld = root;
            auto curOldIt = iterator(root);
            std::shared_ptr<Node> prevNew = nullptr;
            std::shared_ptr<Node> copyRoot = nullptr;
            while (curOldIt != pos) {
                auto copyCur = std::make_shared<Node>(*curOldIt);
                if (curOldIt == begin(srcVersion)) {
                    copyRoot = copyCur;
                }
                if (prevNew) {
                    prevNew->next = copyCur;
                }
                prevNew = prevNew->next;
                ++curOldIt;
                curOld = curOld->next;
            }
            prevNew->next = newNode;
            newNode->next = curOld;
            _versions.push_back(VersionData(copyRoot, srcVersion + 1, size + 1));
        }
        return iterator(newNode);
    }

    inline iterator erase(const size_t srcVersion, iterator pos) {
        if (_versions.size() - 1 < srcVersion) {
            throw new std::out_of_range("Invalid version: " + srcVersion);
        }
        auto root = _versions[srcVersion].root;
        auto size = _versions[srcVersion].size;
        if (!root || pos == end()) {
            return end();
        } else if (pos == begin(srcVersion)) {
            _versions.push_back(VersionData(root->next, srcVersion + 1, size - 1));
            return iterator(root->next);
        } else {
            auto curOldIt = iterator(root);
            auto curOld = root;
            std::shared_ptr<Node> curNew = nullptr;
            std::shared_ptr<Node> copyRoot = nullptr;
            while (curOldIt != pos) {
                auto copyCur = std::make_shared<Node>(*curOldIt);
                if (curOldIt == begin(srcVersion)) {
                    copyRoot = copyCur;
                }
                if (curNew) {
                    curNew->next = copyCur;
                    curNew = curNew->next;
                } else {
                    curNew = copyCur;
                }
                ++curOldIt;
                curOld = curOld->next;
            }
            curNew->next = curOld->next;
            _versions.push_back(VersionData(copyRoot, srcVersion + 1, size - 1));
            return iterator(curNew->next);
        }
    }
    inline iterator erase(const size_t srcVersion, const_iterator pos) {
        if (_versions.size() - 1 < srcVersion) {
            throw new std::out_of_range("Invalid version: " + srcVersion);
        }
        auto root = _versions[srcVersion].root;
        auto size = _versions[srcVersion].size;
        if (!root || pos == end()) {
            return end();
        } else if (pos == begin(srcVersion)) {
            _versions.push_back(VersionData(root->next, srcVersion + 1, size - 1));
            return iterator(root->next);
        } else {
            auto curOldIt = iterator(root);
            auto curOld = root;
            std::shared_ptr<Node> curNew = nullptr;
            std::shared_ptr<Node> copyRoot = nullptr;
            while (curOldIt != pos) {
                auto copyCur = std::make_shared<Node>(*curOldIt);
                if (curOldIt == begin(srcVersion)) {
                    copyRoot = copyCur;
                }
                if (curNew) {
                    curNew->next = copyCur;
                    curNew = curNew->next;
                } else {
                    curNew = copyCur;
                }
                ++curOldIt;
                curOld = curOld->next;
            }
            curNew->next = curOld->next;
            _versions.push_back(VersionData(copyRoot, srcVersion + 1, size - 1));
            return iterator(curNew->next);
        }
    }
    void push_back(const size_t srcVersion, const value_type& value) {
        insert(srcVersion, end(), value);
    }
    void pop_back(const size_t srcVersion) {
        auto root = _versions[srcVersion].root;
        auto size = _versions[srcVersion].size;
        auto curOld = root;
        std::shared_ptr<Node> curNew = nullptr;
        std::shared_ptr<Node> copyRoot = nullptr;
        while (curOld->next) {
            auto copyCur = std::make_shared<Node>(curOld->value);
            if (curNew) {
                curNew->next = copyCur;
                curNew = curNew->next;
            } else {
                curNew = copyCur;
            }
            if (curOld == root) {
                copyRoot = copyCur;
            }
            curOld = curOld->next;
        }
        _versions.push_back(VersionData(copyRoot, srcVersion + 1, size - 1));
    }
    void push_front(const size_t srcVersion, const value_type& value) {
        insert(srcVersion, begin(srcVersion), value);
    }
    void pop_front(const size_t srcVersion) {
        erase(srcVersion, begin(srcVersion));
    }

//    PesistentVector<T> toVector() {

//    }

private:
    std::vector<VersionData> _versions;
    std::shared_ptr<Node> _root;
};

#endif // PERSISTENT_LIST_HPP
