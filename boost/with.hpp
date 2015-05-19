#ifndef BOOST_WITH_HPP_INCLUDED
#define BOOST_WITH_HPP_INCLUDED

#include <utility>
#include <type_traits>

#define BOOST_WITH(what)                                                       \
    if (auto boost_with_intentionally_unused                                   \
        = boost::with_detail::make_false(what)) {                              \
        (void) boost_with_intentionally_unused;                                \
    } else


namespace boost {
namespace with_detail {

// wraps an object of movable type
// and makes it convertible to bool (always false)
template <class T>
class always_false {
public:
    explicit always_false(T what) : x{std::move(what)} {}
    constexpr operator bool() const { return false; }

private:
    T x;
};

// always_false<T> factory
template <class T>
auto make_false(T&& what, std::true_type) {
    static_assert(std::is_move_constructible<T>::value,
                  "this is a BOOST_WITH bug");
    return always_false<T>{std::forward<T>(what)};
}

// static error for non-movable types
template <class T>
auto make_false(T&&, std::false_type) {
    static_assert(std::is_move_constructible<T>::value,
                  "BOOST_WITH requires the scoped object's type to be move "
                  "constructible");
    return std::false_type{}; // never reached
}

// macro entry point, tag dispatch for static type checking
template <class T>
auto make_false(T&& what) {
    return make_false(std::forward<T>(what), std::is_move_constructible<T>{});
}
}
} // namepace boost::with_detail

#endif // BOOST_WITH_HPP_INCLUDED
