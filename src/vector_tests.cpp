#include <gtest/gtest.h>
#include "persistent_vector.hpp"
#include "tests.hpp"

TEST_F(PersistentVectorTest, ConstructorTest) {
    PersistentVector<int> vector1;
    ASSERT_TRUE(vector1.empty(0));

    vector1.push_back(0, 10);
    vector1.push_back(1, 9);
    vector1.push_back(2, 8);

    PersistentVector<int> vector2 = vector1;
    auto it = vector2.begin(3);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(8, *it);
    ++it;
    ASSERT_EQ(vector2.end(), it);

    ASSERT_EQ(10, vector2.front(3));
    ASSERT_EQ(8, vector2.back(3));
    ASSERT_EQ(3, vector2.size(3));

//    for (auto it = vector2.begin(3); it != vector2.end(); ++it) {
//        std::cout << *it << std::endl;
//    }

    PersistentVector<int> vector3;
    vector3 = vector1;
    it = vector3.begin(3);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(8, *it);
    ++it;
    ASSERT_EQ(vector3.end(), it);

    ASSERT_EQ(10, vector3.front(3));
    ASSERT_EQ(8, vector3.back(3));
    ASSERT_EQ(3, vector3.size(3));
}

TEST_F(PersistentVectorTest, PushTest) {
    PersistentVector<int> vector;

    vector.push_back(0, 10);
    auto it = vector.begin(1);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(vector.end(), it);

    ASSERT_EQ(10, vector.front(1));
    ASSERT_EQ(10, vector.back(1));
    ASSERT_EQ(1, vector.size(1));
    ASSERT_TRUE(vector.empty(0));

    vector.push_back(1, 9);
    it = vector.begin(2);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(vector.end(), it);

    ASSERT_EQ(10, vector.front(1));
    ASSERT_EQ(10, vector.back(1));
    ASSERT_EQ(10, vector.front(2));
    ASSERT_EQ(9, vector.back(2));
    ASSERT_EQ(1, vector.size(1));
    ASSERT_EQ(2, vector.size(2));
    ASSERT_TRUE(vector.empty(0));

    vector.push_back(2, 8);
    it = vector.begin(3);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(8, *it);
    ++it;
    ASSERT_EQ(vector.end(), it);

    ASSERT_EQ(10, vector.front(3));
    ASSERT_EQ(8, vector.back(3));
    ASSERT_EQ(3, vector.size(3));
}

TEST_F(PersistentVectorTest, PopTest) {
    PersistentVector<int> vector;

    vector.push_back(0, 10);
    vector.push_back(1, 9);
    vector.push_back(2, 8);

    vector.pop_back(3);
    auto it = vector.begin(4);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(vector.end(), it);

    ASSERT_EQ(10, vector.front(4));
    ASSERT_EQ(9, vector.back(4));
    ASSERT_EQ(10, vector.front(3));
    ASSERT_EQ(8, vector.back(3));

    ASSERT_EQ(3, vector.size(3));
    ASSERT_EQ(2, vector.size(4));
    ASSERT_EQ(3, vector.size(3));

    vector.pop_back(4);
    it = vector.begin(5);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(vector.end(), it);

    ASSERT_EQ(10, vector.front(5));
    ASSERT_EQ(10, vector.back(5));
    ASSERT_EQ(10, vector.front(4));
    ASSERT_EQ(9, vector.back(4));

    ASSERT_EQ(1, vector.size(5));
    ASSERT_EQ(2, vector.size(4));
    ASSERT_EQ(3, vector.size(3));

    vector.push_back(5, 9);
    it = vector.begin(6);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(vector.end(), it);

    ASSERT_EQ(10, vector.front(6));
    ASSERT_EQ(9, vector.back(6));
    ASSERT_EQ(10, vector.front(5));
    ASSERT_EQ(10, vector.back(5));
    ASSERT_EQ(1, vector.size(5));
    ASSERT_EQ(2, vector.size(6));
}


