#include "tests.hpp"
#include "persistent_vector.hpp"
#include "persistent_list.hpp"
#include "persistent_map.hpp"

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

TEST_F(PersistentVectorTest, RandomAccessTest) {
    PersistentVector<int> vector;

    vector.push_back(0, 10);
    vector.push_back(1, 9);
    vector.push_back(2, 8);

    ASSERT_EQ(10, vector.at(1, 0));

    ASSERT_EQ(10, vector.at(2, 0));
    ASSERT_EQ(9, vector.at(2, 1));

    ASSERT_EQ(10, vector.at(3, 0));
    ASSERT_EQ(9, vector.at(3, 1));
    ASSERT_EQ(8, vector.at(3, 2));

    vector.update(3, 1, 90);
    ASSERT_EQ(10, vector.at(3, 0));
    ASSERT_EQ(9, vector.at(3, 1));
    ASSERT_EQ(8, vector.at(3, 2));
    ASSERT_EQ(10, vector.at(4, 0));
    ASSERT_EQ(90, vector.at(4, 1));
    ASSERT_EQ(8, vector.at(4, 2));
    ASSERT_EQ(3, vector.size(3));
    ASSERT_EQ(3, vector.size(4));
}

TEST_F(PersistentVectorTest, FullyPersistenceTest) {
    PersistentVector<int> vector;

    vector.push_back(0, 10);
    vector.push_back(1, 9);
    vector.push_back(2, 8);

    vector.push_back(2, 7);
    auto it = vector.begin(4);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(7, *it);
    ++it;
    ASSERT_EQ(vector.end(), it);

    it = vector.begin(3);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(9, *it);
    ++it;
    ASSERT_EQ(8, *it);
    ++it;
    ASSERT_EQ(vector.end(), it);

    ASSERT_EQ(10, vector.front(4));
    ASSERT_EQ(7, vector.back(4));
    ASSERT_EQ(10, vector.front(3));
    ASSERT_EQ(8, vector.back(3));
    ASSERT_EQ(10, vector.front(2));
    ASSERT_EQ(9, vector.back(2));

    ASSERT_EQ(3, vector.size(3));
    ASSERT_EQ(3, vector.size(4));
    ASSERT_EQ(2, vector.size(2));

    vector.pop_back(2);
    it = vector.begin(5);
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(vector.end(), it);

    ASSERT_EQ(10, vector.front(4));
    ASSERT_EQ(7, vector.back(4));
    ASSERT_EQ(10, vector.front(3));
    ASSERT_EQ(8, vector.back(3));
    ASSERT_EQ(10, vector.front(2));
    ASSERT_EQ(9, vector.back(2));
    ASSERT_EQ(10, vector.front(5));
    ASSERT_EQ(10, vector.back(5));

    ASSERT_EQ(3, vector.size(3));
    ASSERT_EQ(3, vector.size(4));
    ASSERT_EQ(2, vector.size(2));
    ASSERT_EQ(1, vector.size(5));
}

TEST_F(PersistentVectorTest, NestedListTest) {
    PersistentList<int> l1;
    l1.push_back(0, 1);
    l1.push_back(1, 2);
    l1.push_back(2, 3);
    PersistentList<int> l2;
    l2.push_back(0, 4);
    l2.push_back(1, 5);

    PersistentVector<PersistentList<int> > vector;
    vector.push_back(0, l1);
    vector.push_back(1, l2);
    vector.push_back(0, l2);

    ASSERT_EQ(l1, vector.front(1));
    ASSERT_EQ(l1, vector.back(1));
    ASSERT_EQ(l1, vector.front(2));
    ASSERT_EQ(l2, vector.back(2));
    ASSERT_EQ(l2, vector.front(3));
    ASSERT_EQ(l2, vector.back(3));

    ASSERT_EQ(1, vector.size(1));
    ASSERT_EQ(2, vector.size(2));
    ASSERT_EQ(1, vector.size(3));
}

TEST_F(PersistentVectorTest, NestedMapTest) {
    PersistentMap<std::string, int> m1;
    m1.insert(0, std::make_pair("ten", 10));
    m1.insert(1, std::make_pair("nine", 9));
    PersistentMap<std::string, int> m2;
    m2.insert(0, std::make_pair("one", 1));
    m2.insert(1, std::make_pair("two", 2));

    PersistentVector<PersistentMap<std::string, int>  > vector;
    vector.push_back(0, m1);
    vector.push_back(1, m2);
    vector.push_back(0, m2);

    ASSERT_EQ(m1, vector.front(1));
    ASSERT_EQ(m1, vector.back(1));
    ASSERT_EQ(m1, vector.front(2));
    ASSERT_EQ(m2, vector.back(2));
    ASSERT_EQ(m2, vector.front(3));
    ASSERT_EQ(m2, vector.back(3));

    ASSERT_EQ(1, vector.size(1));
    ASSERT_EQ(2, vector.size(2));
    ASSERT_EQ(1, vector.size(3));
}
