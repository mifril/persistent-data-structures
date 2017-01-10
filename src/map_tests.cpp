#include "persistent_map.hpp"
#include "tests.hpp"

typedef std::pair<PersistentMap<std::string, int>::iterator, bool> insert_pair;

TEST_F(PersistentMapTest, ConstructorTest) {
    PersistentMap<std::string, int> map1;
    ASSERT_TRUE(map1.empty(0));

    map1.insert(0, std::make_pair("ten", 10));
    map1.insert(1, std::make_pair("nine", 9));

    PersistentMap<std::string, int> map2 = map1;
    ASSERT_EQ(map2.end(), map2.find(1, "nine"));
    ASSERT_EQ(map2.end(), map2.find(0, "nine"));
    ASSERT_EQ(10, (*(map2.find(1, "ten"))).second);

    ASSERT_EQ(10, map2.at(1, "ten"));
    ASSERT_EQ(2, map2.size(2));
    ASSERT_EQ(1, map2.size(1));


    PersistentMap<std::string, int> map3;
    map3 = map1;
    ASSERT_EQ(map3.end(), map3.find(1, "nine"));
    ASSERT_EQ(map3.end(), map3.find(0, "nine"));
    ASSERT_EQ(10, (*(map3.find(1, "ten"))).second);

    ASSERT_EQ(10, map3.at(1, "ten"));
    ASSERT_EQ(2, map3.size(2));
    ASSERT_EQ(1, map3.size(1));
}

TEST_F(PersistentMapTest, InsertTest) {
    PersistentMap<std::string, int> map;

    insert_pair result = map.insert(0, std::make_pair("ten", 10));
    ASSERT_EQ(true, result.second);
    ASSERT_EQ(10, (*(result.first)).second);

    ASSERT_EQ(10, (*(map.find(1, "ten"))).second);
    ASSERT_EQ(10, map.at(1, "ten"));
    ASSERT_EQ(map.end(), map.find(1, "nine"));

    ASSERT_EQ(1, map.size(1));
    ASSERT_FALSE(map.empty(1));


    result = map.insert(1, std::make_pair("nine", 9));
    ASSERT_EQ(true, result.second);

    ASSERT_EQ(map.end(), map.find(1, "nine"));
    ASSERT_EQ(map.end(), map.find(0, "nine"));
    ASSERT_EQ(10, (*(map.find(1, "ten"))).second);

    ASSERT_EQ(10, map.at(1, "ten"));
    ASSERT_EQ(2, map.size(2));
    ASSERT_EQ(1, map.size(1));


    result = map.insert(2, std::make_pair("eight", 8));
    ASSERT_EQ(true, result.second);

    ASSERT_EQ(map.end(), map.find(2, "eight"));
    ASSERT_EQ(map.end(), map.find(1, "eight"));
    ASSERT_EQ(map.end(), map.find(0, "eight"));


    ASSERT_EQ(8, (*(map.find(3, "eight"))).second);
    ASSERT_EQ(9, (*(map.find(3, "nine"))).second);
    ASSERT_EQ(10, (*(map.find(3, "ten"))).second);
    ASSERT_EQ(8, map.at(3, "eight"));

    ASSERT_EQ(3, map.size(3));
    ASSERT_EQ(2, map.size(2));
    ASSERT_EQ(1, map.size(1));
}

TEST_F(PersistentMapTest, EraseTest) {
    PersistentMap<std::string, int> map;
    map.insert(0, std::make_pair("ten", 10));
    map.insert(1, std::make_pair("nine", 9));
    map.erase(2, "ten");
    ASSERT_EQ(10, (*(map.find(1, "ten"))).second);
    ASSERT_EQ(10, (*(map.find(2, "ten"))).second);
    ASSERT_EQ(map.end(), map.find(3, "ten"));
    ASSERT_EQ(10, map.at(1, "ten"));
    ASSERT_EQ(10, map.at(2, "ten"));

    ASSERT_EQ(1, map.size(1));
    ASSERT_EQ(2, map.size(2));
    ASSERT_EQ(1, map.size(3));
    ASSERT_FALSE(map.empty(1));

    map.erase(3, "nine");
    ASSERT_TRUE(map.empty(4));
    ASSERT_FALSE(map.empty(3));
    ASSERT_FALSE(map.empty(2));
    ASSERT_FALSE(map.empty(1));

    map.insert(4, std::make_pair("ten", 10));
    ASSERT_EQ(10, (*(map.find(5, "ten"))).second);
    ASSERT_EQ(10, map.at(5, "ten"));
    ASSERT_EQ(map.end(), map.find(4, "ten"));

    ASSERT_EQ(1, map.size(5));
    ASSERT_FALSE(map.empty(5));
    ASSERT_TRUE(map.empty(4));
    ASSERT_FALSE(map.empty(3));
    ASSERT_FALSE(map.empty(2));
    ASSERT_FALSE(map.empty(1));
}
