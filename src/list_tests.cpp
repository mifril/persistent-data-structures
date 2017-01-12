#include <gtest/gtest.h>
#include "persistent_list.hpp"
#include "tests.hpp"

TEST_F(PersistentListTest, ConstructorTest) {
    PersistentList<int> list1;
    ASSERT_TRUE(list1.empty(0));

    list1.push_back(0, 10);
    list1.push_back(1, 9);
    list1.push_front(2, 8);

    PersistentList<int> list2 = list1;
    auto it = list2.begin(3);
    ASSERT_EQ(8, *it);
    ++it;
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(list2.end(), it);

    ASSERT_EQ(8, list2.front(3));
    ASSERT_EQ(9, list2.back(3));
    ASSERT_EQ(3, list2.size(3));

//    for (auto it = list2.begin(3); it != list2.end(); ++it) {
//        std::cout << *it << std::endl;
//    }

    PersistentList<int> list3;
    list3 = list1;
    it = list3.begin(3);
    ASSERT_EQ(8, *it);
    ++it;
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(list3.end(), it);

    ASSERT_EQ(8, list3.front(3));
    ASSERT_EQ(9, list3.back(3));
    ASSERT_EQ(3, list3.size(3));
}

TEST_F(PersistentListTest, PushTest) {
    PersistentList<int> list;

    list.push_back(0, 10);
    auto it = list.begin(1);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(list.end(), it);

    ASSERT_EQ(10, list.front(1));
    ASSERT_EQ(10, list.back(1));
    ASSERT_EQ(1, list.size(1));
    ASSERT_TRUE(list.empty(0));

    list.push_back(1, 9);
    it = list.begin(2);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(list.end(), it);

    ASSERT_EQ(10, list.front(1));
    ASSERT_EQ(10, list.back(1));
    ASSERT_EQ(10, list.front(2));
    ASSERT_EQ(9, list.back(2));
    ASSERT_EQ(1, list.size(1));
    ASSERT_EQ(2, list.size(2));
    ASSERT_TRUE(list.empty(0));

    list.push_front(2, 8);
    it = list.begin(3);
    ASSERT_EQ(8, *it);
    ++it;
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(list.end(), it);

    ASSERT_EQ(8, list.front(3));
    ASSERT_EQ(9, list.back(3));
    ASSERT_EQ(3, list.size(3));
}

TEST_F(PersistentListTest, PopTest) {
    PersistentList<int> list;

    list.push_back(0, 10);
    list.push_back(1, 9);
    list.push_front(2, 8);

    list.pop_back(3);
    auto it = list.begin(4);
    ASSERT_EQ(8, *it);
    ++it;
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(list.end(), it);

    ASSERT_EQ(8, list.front(4));
    ASSERT_EQ(10, list.back(4));
    ASSERT_EQ(8, list.front(3));
    ASSERT_EQ(9, list.back(3));

    ASSERT_EQ(2, list.size(4));
    ASSERT_EQ(3, list.size(3));

    list.pop_front(4);
    it = list.begin(5);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(list.end(), it);

    ASSERT_EQ(10, list.front(5));
    ASSERT_EQ(10, list.back(5));
    ASSERT_EQ(8, list.front(4));
    ASSERT_EQ(10, list.back(4));

    ASSERT_EQ(1, list.size(5));
    ASSERT_EQ(2, list.size(4));
    ASSERT_EQ(3, list.size(3));

    list.push_back(5, 9);
    it = list.begin(6);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(list.end(), it);

    ASSERT_EQ(10, list.front(6));
    ASSERT_EQ(9, list.back(6));
    ASSERT_EQ(10, list.front(5));
    ASSERT_EQ(10, list.back(5));
    ASSERT_EQ(1, list.size(5));
    ASSERT_EQ(2, list.size(6));
}

TEST_F(PersistentListTest, FullyPersistenceTest) {
    PersistentList<int> list;

    list.push_back(0, 10);
    list.push_back(1, 9);
    list.push_front(2, 8);

    list.pop_back(2);
    auto it = list.begin(4);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(list.end(), it);

    ASSERT_EQ(10, list.front(4));
    ASSERT_EQ(10, list.back(4));
    ASSERT_EQ(8, list.front(3));
    ASSERT_EQ(9, list.back(3));
    ASSERT_EQ(10, list.front(2));
    ASSERT_EQ(9, list.back(2));

    ASSERT_EQ(2, list.size(2));
    ASSERT_EQ(3, list.size(3));
    ASSERT_EQ(1, list.size(4));

    list.push_front(1, 11);
    it = list.begin(5);
    ASSERT_EQ(11, *it);
    ++it;
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(list.end(), it);

    ASSERT_EQ(11, list.front(5));
    ASSERT_EQ(10, list.back(5));
    ASSERT_EQ(10, list.front(1));
    ASSERT_EQ(10, list.back(1));
    ASSERT_EQ(10, list.front(4));
    ASSERT_EQ(10, list.back(4));
    ASSERT_EQ(8, list.front(3));
    ASSERT_EQ(9, list.back(3));
    ASSERT_EQ(10, list.front(2));
    ASSERT_EQ(9, list.back(2));

    ASSERT_EQ(2, list.size(2));
    ASSERT_EQ(3, list.size(3));
    ASSERT_EQ(1, list.size(4));
    ASSERT_EQ(2, list.size(5));
}
