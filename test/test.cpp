#include <gtest/gtest.h>
#include "boost/with.hpp"
#include <mutex>

class Dummy_lock {
public:
    void lock() { l = true; }
    void unlock() { l = false; }
    bool locked() const { return l; }

private:
    bool l = false;
};

using Lock_guard = std::lock_guard<Dummy_lock>;

class Copyable {
public:
    Copyable(int, int) {}
    Copyable(Copyable const&) = default;
    Copyable(Copyable&&) = delete;
};

class Movable {
public:
    Movable() {}
    Movable(int, int, int) {}
    Movable(Movable const&) = delete;
    Movable(Movable&&) = default;
};

TEST(BoostWith, Basic) {
    BOOST_WITH (int, 0) {}
    BOOST_WITH (int) {}
    BOOST_WITH (Copyable, 1, 2) {}
    BOOST_WITH (Movable, 1, 2, 3) {}
    BOOST_WITH (Movable) {}
}

TEST(BoostWith, Guard) {
    Dummy_lock l;
    EXPECT_FALSE(l.locked());
    BOOST_WITH (Lock_guard, l)
        EXPECT_TRUE(l.locked());
    EXPECT_FALSE(l.locked());
}

TEST(BoostWith, Nesting) {
    Dummy_lock l1;
    Dummy_lock l2;
    BOOST_WITH (Lock_guard, l1)
        BOOST_WITH (Lock_guard, l2) {
            EXPECT_TRUE(l1.locked());
            EXPECT_TRUE(l2.locked());
        }
    EXPECT_FALSE(l1.locked());
    EXPECT_FALSE(l2.locked());

    BOOST_WITH (Lock_guard, l1) {
        BOOST_WITH (Lock_guard, l2)
            EXPECT_TRUE(l2.locked());
        EXPECT_TRUE(l1.locked());
        EXPECT_FALSE(l2.locked());
    }

    for (auto i = 0; i < 2; ++i)
        BOOST_WITH (Lock_guard, l1)
            EXPECT_TRUE(l1.locked());

    BOOST_WITH (Lock_guard, l1)
        for (auto i = 0; i < 2; ++i)
            EXPECT_TRUE(l1.locked());
}
