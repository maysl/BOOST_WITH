#ifndef BOOST_WITH_HPP_INCLUDED
#define BOOST_WITH_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/config.hpp>

#define BOOST_WITH_UNUSED_ARG                                                  \
    boost_with_INTENTIONALLY_UNUSED BOOST_ATTRIBUTE_UNUSED

#define BOOST_WITH(what)                                                       \
    if (auto BOOST_WITH_UNUSED_ARG = boost::with_detail::make_true(what))


namespace boost {
namespace with_detail {

// wraps an object of movable type
// and makes it convertible to bool (always true)
template <class T>
class always_true {
public:
    explicit always_true(T what) : x{std::move(what)} {}
    constexpr operator bool() const { return true; }

private:
    T x;
};

// always_true<T> construction helper
template <class T>
auto make_true(T&& what, std::true_type) {
    static_assert(std::is_move_constructible<T>::value,
                  "this is a BOOST_WITH bug");
    return always_true<T>{std::forward<T>(what)};
}

// static error for non-movable types
template <class T>
auto make_true(T&&, std::false_type) {
    static_assert(std::is_move_constructible<T>::value,
                  "BOOST_WITH requires the scoped object's type to be move "
                  "constructible");
    return std::true_type{}; // never reached
}

// macro entry point, tag dispatch for static type checking
// NOTE: the static_assert has to be hidden away from the #define, because
// otherwise the construct
//
// for(;;)
//     BOOST_WITH(0) {}
//
// would not work (note missing curly brace after 'for')
//
template <class T>
auto make_true(T&& what) {
    return make_true(std::forward<T>(what), std::is_move_constructible<T>{});
}
}
} // namepace boost::with_detail

#endif // BOOST_WITH_HPP_INCLUDED
