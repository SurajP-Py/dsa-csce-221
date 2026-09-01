#include "executable.h"

TEST(empty_after_push_pop) {
    {
        Vector<int> v(1);

        ASSERT_FALSE(v.empty());

        v.pop_back();

        ASSERT_EQ(v.size(), 0UL);
        ASSERT_TRUE(v.capacity() > 0UL);
        ASSERT_TRUE(v.empty());
    }
};