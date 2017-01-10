#ifndef PERSISTENT_VECTOR_HPP
#define PERSISTENT_VECTOR_HPP

#include <utility>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>
#include "version_tree.h"

template <class T>
class PersistentVector {
public:
    typedef T value_type;

private:
    struct VersionData {
        size_t version;

        VersionData() : version(0)
        {}
        VersionData(const VersionData & other) : version(other.version)
        {}
        VersionData(const size_t version_) :
            version(version_)
        {}

        bool operator==(const VersionData& other) {
            return version == other.version;
        }
        bool operator==(const VersionData& other) const {
            return version == other.version;
        }
    };

    struct VersionValue {
        size_t version;
        T value;

        VersionValue(const size_t version_, const T value_) : version(version_), value(value_)
        {}

        bool operator==(const VersionValue& other) {
            return version == other.version && value == other.value;
        }
        bool operator==(const VersionValue& other) const {
            return version == other.version && value == other.value;
        }
    };

    struct FatNode {
        std::list<VersionValue> nodeVersions;

        bool operator==(const FatNode& other) {
            return nodeVersions == other.nodeVersions;
        }
        bool operator==(const FatNode& other) const {
            return nodeVersions == other.nodeVersions;
        }
    };

    template<class Y>
    class VectorIterator : public std::iterator<std::bidirectional_iterator_tag, Y> {
    public:
        VectorIterator(PersistentVector& vector) : _vector(vector), _isEnd(true)
        {}
//        VectorIterator(PersistentVector* vector) : _vector(vector), _cur(0), _version(0), _isEnd(false)
//        {}
//        VectorIterator(PersistentVector* vector, const size_t version)
//            : _vector(vector), _version(version), _isEnd(false)
//        {}
        VectorIterator(PersistentVector& vector, const size_t version, const long long cur)
            : _vector(vector), _cur(cur), _version(version), _isEnd(false)
        {}
        VectorIterator(const VectorIterator& other)
            : _vector(other._vector), _cur(other._cur), _version(other._version), _isEnd(false)
        {}
        VectorIterator(VectorIterator&& other)
                : _vector(other._vector), _cur(other._cur), _version(other._version), _isEnd(false) {
            other._cur = 0;
            other._version = 0;
        }
        VectorIterator& operator=(const VectorIterator& other) {
            if (*this != other) {
                _vector = other._vector;
                _cur = other._cur;
                _version - other._version;
                _isEnd = other._isEnd;
            }
            return* this;
        }
        VectorIterator& operator=(VectorIterator&& other) {
            if (*this != other) {
                std::swap(_vector, other._vector);
                std::swap(_cur, other._cur);
                std::swap(_version, other._version);
                std::swap(_isEnd, other._isEnd);
            }
            return* this;
        }
        VectorIterator& operator++() {
            if (!_isEnd) {
                ++_cur;
            }
            if (_cur >= _vector.size(_version)) {
                _isEnd = true;
            }
            return* this;
        }
        VectorIterator operator++(int) {
            VectorIterator tmp(*this);
            operator++();
            return tmp;
        }
        VectorIterator& operator--() {
            if (!_isEnd && _cur > 0) {
                --_cur;
            }
            return* this;
        }
        VectorIterator operator--(int) {
            VectorIterator tmp(*this);
            operator++();
            return tmp;
        }
        bool operator==(const VectorIterator& other) {
            return (_isEnd == other._isEnd && _isEnd == true)
                    || (_vector == other._vector && _cur == other._cur
                        && _version == other._version && _isEnd == other._isEnd);
        }
        bool operator==(const VectorIterator& other) const {
            return (_isEnd == other._isEnd && _isEnd == true)
                    || (_vector == other._vector && _cur == other._cur
                        && _version == other._version && _isEnd == other._isEnd);
        }
        bool operator!=(const VectorIterator& other) {
            return !operator ==(other);
        }
        bool operator!=(const VectorIterator& other) const {
            return !operator ==(other);
        }
        T& operator*() {
            if (_cur >= 0) {
                return _vector.at(_version, _cur);
            } else {
                throw new std::out_of_range("Iterator is out of range");
            }
        }
        T* operator->() {
            if (_cur) {
                return&(_vector.at(_version, _cur));
            } else {
                throw new std::out_of_range("Iterator is out of range");
            }
        }
    private:
        PersistentVector& _vector;
        size_t _cur;
        size_t _version;
        bool _isEnd;
    };

public:
    typedef VectorIterator<value_type> iterator;
    typedef VectorIterator<const value_type> const_iterator;

    PersistentVector() {
        _versionSizes.push_back(0);
    }
    PersistentVector(const PersistentVector& other)
            : _fatNodes(other._fatNodes), _versionSizes(other._versionSizes), _versions(other._versions) {
        _versionSizes.push_back(0);
    }
    PersistentVector(PersistentVector&& other)
            : _fatNodes(other._fatNodes), _versionSizes(other._versionSizes), _versions(other._versions) {
        other.clear();
    }
    PersistentVector& operator=(const PersistentVector& other) {
        if (*this != other) {
            clear();
            _fatNodes = other._fatNodes;
            _versionSizes = other._versionSizes;
            _versions = other._versions;
        }
        return *this;
    }
    PersistentVector& operator=(PersistentVector&& other) {
        if (*this != other) {
            std::swap(_fatNodes, other._fatNodes);
            std::swap(_versionSizes, other._versionSizes);
            std::swap(_versions, other._versions);
        }
        return *this;
    }
    ~PersistentVector() {
        clear();
    }

    bool operator==(const PersistentVector& other) {
        return _fatNodes == other._fatNodes && _versionSizes == other._versionSizes && _versions == other._versions;
    }
    bool operator==(const PersistentVector& other) const {
        return _fatNodes == other._fatNodes && _versionSizes == other._versionSizes && _versions == other._versions;
    }
    bool operator!=(const PersistentVector& other) {
        return !operator ==(other);
    }
    bool operator!=(const PersistentVector& other) const {
        return !operator ==(other);
    }

    inline value_type& at(const size_t version, const size_t index) {
        if (index >= _versionSizes[version]) {
            throw new std::out_of_range("Index out of range: " + index);
        }
        return _getLatestVersion(version, index);
    }

    value_type& front(const size_t version) {
        return _getLatestVersion(version, 0);
    }
    const value_type& front(const size_t version) const {
        return _getLatestVersion(version, 0);
    }
    value_type& back(const size_t version) {
        return _getLatestVersion(version, _versionSizes[version] - 1);
    }
    const value_type& back(const size_t version) const {
        return _getLatestVersion(version, _versionSizes[version] - 1);
    }

    inline iterator begin(const size_t version) noexcept {
        return iterator(*this, version, 0);
    }
    inline iterator end() noexcept {
        return iterator(*this);
    }
    inline const_iterator begin(const size_t version) const noexcept {
        return const_iterator(*this, version, 0);
    }
    inline const_iterator end() const noexcept {
        return const_iterator(*this);
    }
    inline const_iterator cbegin(const size_t version) const noexcept {
        return const_iterator(*this, version, 0);
    }
    inline const_iterator cend() const noexcept {
        return const_iterator(*this);
    }
    inline bool empty(const size_t version) const noexcept {
        return _versionSizes[version] == 0;
    }
    inline size_t size(const size_t version) const noexcept {
        return _versionSizes[version];
    }
    inline void clear() noexcept {
        _fatNodes.clear();
        _versions.clear();
        _versionSizes.clear();
        _versionSizes.push_back(0);
    }
//    inline iterator insert(const size_t version, iterator pos, const value_type& value) {

//    }
//    inline iterator insert(const size_t version, const_iterator pos, const value_type& value) {

//    }
//    inline size_t erase(const size_t version, iterator pos) {

//    }
//    inline size_t erase(const size_t version, const_iterator pos) {

//    }
    void push_back(const size_t srcVersion, const value_type& value) {
        size_t version = _versions.size();
        _versions.insert(VersionData(version), srcVersion);

        _versionSizes.push_back(_versionSizes[srcVersion] + 1);
        _fatNodes.push_back(FatNode());
        _fatNodes[_versionSizes[version] - 1].nodeVersions.push_back(VersionValue(version, value));
    }
    void pop_back(const size_t srcVersion) {
        _versions.insert(VersionData(_versions.size()), srcVersion);
        _versionSizes.push_back(_versionSizes[srcVersion] - 1);
    }

//    PersistentVector<T> toVector() {

//    }

private:
    std::vector<FatNode> _fatNodes;
    std::vector<size_t> _versionSizes;
    VersionTree<VersionData> _versions;

    value_type& _getLatestVersion(const size_t maxVersion, const size_t index) {
        auto elementVersions = _fatNodes[index].nodeVersions;
        value_type& versionValue = elementVersions.front().value;
        for (auto v : elementVersions) {
            if (_versions.order(v.version, maxVersion)) {
                versionValue = v.value;
            }
        }
        return versionValue;
    }
};

#endif // PERSISTENT_LIST_HPP
