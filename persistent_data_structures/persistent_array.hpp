#ifndef PERSISTENT_ARRAY_HPP
#define PERSISTENT_ARRAY_HPP

#include <iostream>
#include <initializer_list>

template<typename T>
class PersistentArray {
    PersistentArray() {

    }
    explicit PersistentArray(const size_t n) {

    }
    PersistentArray(const size_t n, const T& val) {

    }
    PersistentArray(const PersistentArray & other) {

    }
    PersistentArray(PersistentArray && other) {

    }
    PersistentArray(std::initializer_list<T> initList) {

    }
    ~PersistentArray() {

    }

    PersistentArray& operator=(const PersistentArray& other) {

    }
    PersistentArray& operator=(PersistentArray&& other) {

    }
    PersistentArray& operator=(std::initializer_list<T> initList) {

    }



};


#endif // PERSISTENT_ARRAY_HPP
