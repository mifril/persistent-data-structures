#include "tests.hpp"
#include "persistent_list.hpp"
#include "persistent_vector.hpp"
#include "persistent_map.hpp"

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

TEST_F(PersistentListTest, NestedVectorTest) {
    PersistentVector<int> v1;
    v1.push_back(0, 1);
    v1.push_back(1, 2);
    v1.push_back(2, 3);
    PersistentVector<int> v2;
    v2.push_back(0, 4);
    v2.push_back(1, 5);
    v2.push_back(2, 6);

    PersistentList<PersistentVector<int> > list;
    list.push_back(0, v1);
    list.push_back(1, v2);
    list.push_back(0, v2);

    ASSERT_EQ(v1, list.front(1));
    ASSERT_EQ(v1, list.back(1));
    ASSERT_EQ(v1, list.front(2));
    ASSERT_EQ(v2, list.back(2));
    ASSERT_EQ(v2, list.front(3));
    ASSERT_EQ(v2, list.back(3));

    ASSERT_EQ(1, list.size(1));
    ASSERT_EQ(2, list.size(2));
    ASSERT_EQ(1, list.size(3));
}

TEST_F(PersistentListTest, NestedMapTest) {
    PersistentMap<std::string, int> m1;
    m1.insert(0, std::make_pair("ten", 10));
    m1.insert(1, std::make_pair("nine", 9));
    PersistentMap<std::string, int> m2;
    m2.insert(0, std::make_pair("one", 1));
    m2.insert(1, std::make_pair("two", 2));

    PersistentList<PersistentMap<std::string, int>  > list;
    list.push_back(0, m1);
    list.push_back(1, m2);
    list.push_back(0, m2);

    ASSERT_EQ(m1, list.front(1));
    ASSERT_EQ(m1, list.back(1));
    ASSERT_EQ(m1, list.front(2));
    ASSERT_EQ(m2, list.back(2));
    ASSERT_EQ(m2, list.front(3));
    ASSERT_EQ(m2, list.back(3));

    ASSERT_EQ(1, list.size(1));
    ASSERT_EQ(2, list.size(2));
    ASSERT_EQ(1, list.size(3));
}
