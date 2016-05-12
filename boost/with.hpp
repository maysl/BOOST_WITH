#ifndef BOOST_WITH_HPP_INCLUDED
#define BOOST_WITH_HPP_INCLUDED

#include <utility>
#include <cassert>
#include <type_traits>

#define BOOST_WITH(T, ...)                                                     \
    if (boost::with_detail::always_false<T> boost_with_intentionally_unused    \
        = {__VA_ARGS__}) {                                                     \
        (void) boost_with_intentionally_unused;                                \
    } else


namespace boost {
namespace with_detail {

// makes anything convertible to bool (always false)
template <class T>
class always_false {
public:
    template <class... Args>
    always_false(Args&&... args)
        : x(std::forward<Args>(args)...) {}

    constexpr operator bool() const { return false; }

private:
    T x;
};
}
} // namepace boost::with_detail

#endif // BOOST_WITH_HPP_INCLUDED
