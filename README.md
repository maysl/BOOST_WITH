# BOOST_WITH

## C++ Macro enabling intuitive RAII

### Disclaimer: NOT PART OF BOOST

#### Usage
Example: scoped locking, the intuitive way
``` cpp
#include <boost/with.hpp>
#include <mutex>

using Lock = std::lock_guard<std::mutex>;

int main() {
    std::mutex the_mutex;
    BOOST_WITH(Lock, the_mutex) {
        // the_mutex locked
        do_something();
        do_other_thing();
    }
    // the_mutex unlocked
}
```
Example: OpenGL push/pop
``` cpp
#include <GL/gl.h>
#include <boost/with.hpp>

struct Pushed_matrix {
    Pushed_matrix() { glPushMatrix(); }
    ~Pushed_matrix() { glPopMatrix(); }
};

int main() {
    BOOST_WITH(Pushed_matrix)
        draw_something();
}
```
