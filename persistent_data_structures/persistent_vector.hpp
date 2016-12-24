#ifndef PERSISTENT_VECTOR_HPP
#define PERSISTENT_VECTOR_HPP

#include <utility>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>
#include "persistent_list.hpp"

template <class T>
class PersistentVector {
public:
    typedef T value_type;

    PersistentVector() :
    {}
    PersistentVector(const PersistentVector& other) : {
        insert(other.begin(), other.end());
    }
    PersistentVector(PersistentVector&& other) : {

    }
    PersistentVector& operator=(const PersistentVector& other) {
        if (*this != other) {

        }
        return *this;
    }
    PersistentVector& operator=(PersistentVector&& other) {
        if (*this != other) {

        }
        return *this;
    }
    ~PersistentVector() {
        clear();
    }

    inline value_type& at(const size_t version, const size_t index) {

    }
    value_type* front(const size_t version) {

    }
    const value_type* front(const size_t version) const {

    }
    value_type* back(const size_t version) {

    }
    const value_type* back(const size_t version) const {

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

    }
    void pop_back(const size_t version) {

    }

    PersistentList<T> toList() {

    }

private:

};

#endif // PERSISTENT_LIST_HPP
