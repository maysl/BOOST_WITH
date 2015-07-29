# BOOST_WITH

## C++ Macro enabling Python-style 'with' scopes

### Disclaimer: NOT PART OF BOOST

#### Usage
Example: scoped locking, the intuitive way
``` cpp
#include <boost/with.hpp>
#include <mutex>

using Lock = std::unique_lock<std::mutex>;

int main() {
    std::mutex the_mutex;
    BOOST_WITH(Lock(the_mutex)) {
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

struct Pushed_matrix {
    Pushed_matrix() { glPushMatrix(); }
    ~Pushed_matrix() { glPopMatrix(); }
};

int main() {
    BOOST_WITH(Pushed_matrix())
        draw_something();
}
```
