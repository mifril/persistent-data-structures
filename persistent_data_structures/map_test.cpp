#include <gtest/gtest.h>
#include "persistent_map.hpp"

typedef std::pair<PersistentMap<std::string, int>::iterator, bool> insert_pair;

class PersistentMapTest : public ::testing::Test {
};

TEST_F(PersistentMapTest, SimpleTest) {
    PersistentMap<std::string, int> map;
    ASSERT_TRUE(map.empty(0));

    insert_pair result = map.insert(0, std::make_pair("ten", 10));
    ASSERT_EQ(10, (*(result.first)).second);
    ASSERT_EQ(true, result.second);
    ASSERT_EQ(10, (*(map.find(1, "ten"))).second);
    ASSERT_EQ(10, map.at(1, "ten"));
    ASSERT_EQ(1, map.size(1));
    ASSERT_EQ(map.end(), map.find(1, "nine"));
//    ASSERT_EQ(0, map.at(1, "nine"));
    ASSERT_EQ(map.end(), map.find(1, "nine"));
    ASSERT_EQ(1, map.size(1));
    ASSERT_FALSE(map.empty(1));

//    result = map.insert(std::make_pair("nine", 9));
//    ASSERT_EQ(10, map["ten"]);
//    ASSERT_EQ(10, (*(map.find("ten"))).second);
//    ASSERT_EQ(0, map["nine"]);
//    ASSERT_EQ(0, (*(map.find("nine"))).second);
//    ASSERT_EQ(0, (*(result.first)).second);
//    ASSERT_EQ(false, result.second);
//    ASSERT_EQ(2, map.size());
//    ASSERT_FALSE(map.empty());

//    map["nine"] = 9;
//    ASSERT_EQ(10, map["ten"]);
//    ASSERT_EQ(10, (*(map.find("ten"))).second);
//    ASSERT_EQ(9, map["nine"]);
//    ASSERT_EQ(9, (*(map.find("nine"))).second);
//    ASSERT_EQ(2, map.size());
//    ASSERT_FALSE(map.empty());

//    result = map.insert(std::make_pair("one", 1));
//    ASSERT_EQ(10, map["ten"]);
//    ASSERT_EQ(10, (*(map.find("ten"))).second);
//    ASSERT_EQ(9, map["nine"]);
//    ASSERT_EQ(9, (*(map.find("nine"))).second);
//    ASSERT_EQ(1, map["one"]);
//    ASSERT_EQ(1, (*(map.find("one"))).second);
//    ASSERT_EQ(1, (*(result.first)).second);
//    ASSERT_EQ(true, result.second);
//    ASSERT_EQ(3, map.size());
//    ASSERT_FALSE(map.empty());

//    std::string ten = "ten";
//    std::string nine = "nine";
//    std::string one = "one";
//    ASSERT_EQ(10, map[ten]);
//    ASSERT_EQ(9, map[nine]);
//    ASSERT_EQ(1, map[one]);
//    ASSERT_EQ(10, (*(map.find(ten))).second);
//    ASSERT_EQ(9, (*(map.find(nine))).second);
//    ASSERT_EQ(1, (*(map.find(one))).second);

//    map["two"] = 2;
//    ASSERT_EQ(10, map["ten"]);
//    ASSERT_EQ(10, (*(map.find("ten"))).second);
//    ASSERT_EQ(9, map["nine"]);
//    ASSERT_EQ(9, (*(map.find("nine"))).second);
//    ASSERT_EQ(1, map["one"]);
//    ASSERT_EQ(1, (*(map.find("one"))).second);
//    ASSERT_EQ(2, map["two"]);
//    ASSERT_EQ(2, (*(map.find("two"))).second);
//    ASSERT_EQ(4, map.size());
//    ASSERT_FALSE(map.empty());

//    result = map.emplace("three", 3);
//    ASSERT_EQ(3, (*(result.first)).second);
//    ASSERT_EQ(3, map["three"]);
//    ASSERT_EQ(3, (*(map.find("three"))).second);
//    ASSERT_EQ(5, map.size());
//    ASSERT_FALSE(map.empty());

//    PersistentMap<std::string, int> map2 = map;
//    ASSERT_EQ(5, map.size());
//    ASSERT_FALSE(map.empty());
//    ASSERT_EQ(5, map2.size());
//    ASSERT_FALSE(map2.empty());

//    size_t erased = map.erase("ten");
//    ASSERT_EQ(1, erased);
//    ASSERT_EQ(map.end(), map.find("ten"));
//    ASSERT_EQ(4, map.size());
//    ASSERT_FALSE(map.empty());
//    ASSERT_EQ(5, map2.size());
//    ASSERT_FALSE(map2.empty());

//    PersistentMap<std::string, int> map3;
//    ASSERT_TRUE(map3.empty());
//    for (PersistentMap<std::string, int>::iterator it = map.begin(); it != map.end(); ++it) {
////        std::cout << it->first << " => " << it->second << std::endl;
//        result = map3.insert(*it);
//        ASSERT_EQ(map[it->first], map3[it->first]);
//        ASSERT_FALSE(map3.empty());
//    }

//    PersistentMap<std::string, int> map4;
//    map4.insert(map2.begin(), map2.end());
//    ASSERT_EQ(4, map.size());
//    ASSERT_FALSE(map.empty());
//    ASSERT_EQ(5, map2.size());
//    ASSERT_FALSE(map2.empty());
//    ASSERT_EQ(4, map3.size());
//    ASSERT_FALSE(map3.empty());
//    ASSERT_EQ(5, map4.size());
//    ASSERT_FALSE(map4.empty());

//    map.clear();
//    ASSERT_EQ(0, map.size());
//    ASSERT_TRUE(map.empty());
//    ASSERT_EQ(5, map2.size());
//    ASSERT_FALSE(map2.empty());
//    ASSERT_EQ(4, map3.size());
//    ASSERT_FALSE(map3.empty());
//    ASSERT_EQ(5, map4.size());
//    ASSERT_FALSE(map4.empty());

//    map2.clear();
//    ASSERT_EQ(0, map.size());
//    ASSERT_TRUE(map.empty());
//    ASSERT_EQ(0, map2.size());
//    ASSERT_TRUE(map2.empty());
//    ASSERT_EQ(4, map3.size());
//    ASSERT_FALSE(map3.empty());
//    ASSERT_EQ(5, map4.size());
//    ASSERT_FALSE(map4.empty());

//    map3.clear();
//    ASSERT_EQ(0, map.size());
//    ASSERT_TRUE(map.empty());
//    ASSERT_EQ(0, map2.size());
//    ASSERT_TRUE(map2.empty());
//    ASSERT_EQ(0, map3.size());
//    ASSERT_TRUE(map3.empty());
//    ASSERT_EQ(5, map4.size());
//    ASSERT_FALSE(map4.empty());

//    map4.clear();
//    ASSERT_EQ(0, map.size());
//    ASSERT_TRUE(map.empty());
//    ASSERT_EQ(0, map2.size());
//    ASSERT_TRUE(map2.empty());
//    ASSERT_EQ(0, map3.size());
//    ASSERT_TRUE(map3.empty());
//    ASSERT_EQ(0, map4.size());
//    ASSERT_TRUE(map4.empty());
}


//TEST_F(PersistentMapTest, ManyElementsTest) {
//    PersistentMap<int, int>  map;
//    ASSERT_TRUE(map.empty());

//    for (size_t i = 0; i < 10000; ++i)
//    {
//        map[rand()] = rand();
//    }
//    int count = 0;

//    for (auto it = map.begin(); it != map.end(); ++it) {
//        //std::cout << it->first << " => " << it->second << std::endl;
//        ++count;
//    }
//    ASSERT_EQ(count, map.size());
//}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
