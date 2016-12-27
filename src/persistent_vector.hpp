#ifndef PERSISTENT_VECTOR_HPP
#define PERSISTENT_VECTOR_HPP

#include <utility>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

template <class T>
class PersistentVector {
public:
    typedef T value_type;

private:
    struct FatNode {
        std::vector<value_type> versions;
    };

public:
    PersistentVector() :
    {}
    PersistentVector(const PersistentVector& other) : _fatNodes(other._fatNodes)
    {}
    PersistentVector(PersistentVector&& other) : _fatNodes(other._fatNodes) {
        other.clear();
    }
    PersistentVector& operator=(const PersistentVector& other) {
        if (*this != other) {
            if (!_fatNodes.empty()) {
                clear();
            }
            _fatNodes = other._fatNodes;
        }
        return *this;
    }
    PersistentVector& operator=(PersistentVector&& other) {
        if (*this != other) {
            std::swap(_fatNodes, other._fatNodes);
        }
        return *this;
    }
    ~PersistentVector() {
        clear();
    }

    inline value_type& at(const size_t version, const size_t index) {
        return _getLatestVersion(version, index);
    }

    value_type* front(const size_t version) {
        return _getLatestVersion(version, 0);
    }
    const value_type* front(const size_t version) const {
        return _getLatestVersion(version, 0);
    }
    value_type* back(const size_t version) {
        return _getLatestVersion(version, _fatNodes.size() - 1);
    }
    const value_type* back(const size_t version) const {
        return _getLatestVersion(version, _fatNodes.size() - 1);
    }

    inline iterator begin(const size_t version) noexcept {

    }
    inline iterator end() noexcept {

    }
    inline const_iterator begin(const size_t version) const noexcept {

    }
    inline const_iterator end() const noexcept {

    }
    inline const_iterator cbegin(const size_t version) const noexcept {

    }
    inline const_iterator cend() const noexcept {

    }
    inline bool empty(const size_t version) const noexcept {

    }
    inline size_t size(const size_t version) const noexcept {

    }
    inline void clear() noexcept {
        _fatNodes.clear();
    }
    inline iterator insert(const size_t version, iterator pos, const value_type& value) {

    }
    inline iterator insert(const size_t version, const_iterator pos, const value_type& value) {

    }
    inline size_t erase(const size_t version, iterator pos) {

    }
    inline size_t erase(const size_t version, const_iterator pos) {

    }
    void push_back(const size_t version, const value_type& value) {
        _fatNodes
    }
    void pop_back(const size_t version) {

    }

//    PersistentList<T> toList() {

//    }

private:
    std::vector<Node> _fatNodes;

    value_type& _getLatestVersion(const size_t maxVersion, const size_t index) {
        auto elementVersions = _fatNodes[index].versions;
        size_t latestVersion = 0;
        for (auto v : elementVersions) {
            if (v <= maxVersion) {
                latestVersion = v;
            }
        }
        return elementVersions[latestVersion];
    }
};

#endif // PERSISTENT_LIST_HPP
