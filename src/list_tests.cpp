#include <gtest/gtest.h>
#include "persistent_list.hpp"
#include "tests.hpp"

TEST_F(PersistentListTest, SimpleTest) {
    PersistentList<int> list;
    ASSERT_TRUE(list.empty(0));

    list.push_back(0, 10);
    auto it = list.begin(1);
    ASSERT_EQ(10, *it);
    ASSERT_EQ(10, list.front(1));
    ASSERT_EQ(10, list.back(1));
    ASSERT_EQ(1, list.size(1));
    ASSERT_TRUE(list.empty(0));
}



