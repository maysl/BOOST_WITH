#include <gtest/gtest.h>
#include "boost/with.hpp"
#include <mutex>

class Lockable {
public:
    void lock() { l = true; }
    void unlock() { l = false; }
    bool locked() const { return l; }

private:
    bool l = false;
};

using Unique_lock = std::unique_lock<Lockable>;

TEST(BoostWithTest, TestIfStmt) {
    BOOST_WITH(0) {}
}

TEST(BoostWithTest, TestGuard) {
    Lockable l;
    EXPECT_FALSE(l.locked());
    BOOST_WITH(Unique_lock(l))
        EXPECT_TRUE(l.locked());
    EXPECT_FALSE(l.locked());
}

TEST(BoostWithTest, TestNesting) {
    Lockable l1;
    Lockable l2;
    BOOST_WITH(Unique_lock(l1))
        BOOST_WITH(Unique_lock(l2)) {
            EXPECT_TRUE(l1.locked());
            EXPECT_TRUE(l2.locked());
        }
    EXPECT_FALSE(l1.locked());
    EXPECT_FALSE(l2.locked());

    BOOST_WITH(Unique_lock(l1)) {
        BOOST_WITH(Unique_lock(l2))
            EXPECT_TRUE(l2.locked());
        EXPECT_TRUE(l1.locked());
        EXPECT_FALSE(l2.locked());
    }

    for (auto i = 0; i < 3; ++i)
        BOOST_WITH(Unique_lock(l1))
            EXPECT_TRUE(l1.locked());

    BOOST_WITH(Unique_lock(l1))
        for (auto i = 0; i < 10; ++i)
            EXPECT_TRUE(l1.locked());
}
