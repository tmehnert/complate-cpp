# complate-cpp

*- Rendering of JSX based views in C++*

[![Build](https://github.com/tmehnert/complate-cpp/actions/workflows/build.yml/badge.svg)](https://github.com/tmehnert/complate-cpp/actions/workflows/build.yml)
[![codecov](https://codecov.io/gh/tmehnert/complate-cpp/branch/main/graph/badge.svg)](https://codecov.io/gh/tmehnert/complate-cpp)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE)

[complate](https://complate.org) adapter that can be used in C++ 17 projects to render JSX based HTML views. This
project was inspired by
[complate-java](https://github.com/complate/complate-java).

## Getting started

### Usage

For a detailed description how to use this library checkout out the [User Guide](USER_GUIDE.md). I recommend you to
check out the dedicated [Sample Project](https://github.com/tmehnert/complate-sample-cpp) to try out complate-cpp.

#### Define your views using JSX

```jsx
import {createElement} from 'complate-stream';

export default function Greeting({person}) {
    return <html>
    <head>
        <meta charSet="UTF-8"/>
        <title>Greeting | Example</title>
    </head>
    <body>
    <h1>Hello {person.name}</h1>
    </body>
    </html>
}
```

#### Using them to render HTML inside your C++ application

```c++
unique_ptr<Renderer> renderer; // use QuickJs or V8 renderer implementation
string html = renderer.renderToString("Greeting", Object{
  {"name", "John Doe"}
});
```

### Installation

```shell
# install dependencies
sudo apt install -y build-essential git cmake libv8-dev
# build and install the library
git clone https://github.com/tmehnert/complate-cpp.git && cd complate-cpp
cmake -B build
cmake --build build -j4
sudo cmake --install build/
```

#### CMake link installed library

When you have installed the library, you can use `find_package` and
`target_link_libraries` as follows.

```cmake
find_package(complate REQUIRED CONFIG)
# and
target_link_libraries(your_app PRIVATE complate::quickjs)
# or
target_link_libraries(your_app PRIVATE complate::v8)
```

#### CMake use FetchContent

Another possibility, without having to install the library, is let CMake fetch it. When you use this method, you may not
use `find_package(complate)`. Just put the following commands in your CMakeLists.txt file and replace the GIT_TAG `HEAD`
with the commit-sha or git-tag you want.

```cmake
message("-- Fetching complate...")
FetchContent_Declare(
    complate
    GIT_REPOSITORY https://github.com/tmehnert/complate-cpp.git
    GIT_TAG HEAD
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(complate)
message("-- Fetching complate - done")
# and
target_link_libraries(your_app PRIVATE complate::quickjs)
# or
target_link_libraries(your_app PRIVATE complate::v8)
```

### Running tests and example

The unittests and the example are not built by default, you have to enable them using BUILD_TESTS=on and
BUILD_EXAMPLE=on as shown below.

```shell
# install dependencies
sudo apt install -y build-essential git cmake libv8-dev
# build the project with tests and example enabled
git clone https://github.com/tmehnert/complate-cpp.git && cd complate-cpp
cmake -B build -DBUILD_TESTS=on -DBUILD_EXAMPLE=on
cmake --build build -j4
cd build/

# execute tests
ctest --output-on-failure
# run example, which starts a webserver which serves views/greeting.jsx.
example/complate-example
```

### Compatibility

The library is tested with following compilers.

* GCC 7 - 10 (on Ubuntu 20.04)
* Clang 7 - 12 (on Ubuntu 20.04)

### Dependencies

* [QuickJS](https://bellard.org/quickjs/), MIT Licence (bundled)
* [V8](https://v8.dev/), MIT Licence

#### Dev dependencies

* [Catch2](https://github.com/catchorg/Catch2), BSL-1.0 (fetched via CMake)
* [trompeloeil](https://github.com/rollbear/trompeloeil), BSL-1.0 (fetched via CMake)
* [cpp-httplib](https://github.com/yhirose/cpp-httplib), MIT License (fetched via CMake)
* [Node.js](https://nodejs.org/)
* [CMake](https://cmake.org/) >= 3.14 required

## License

complate-cpp is Open Source software released under the [Apache 2.0 license](LICENSE).

See [LICENCE](LICENSE) for more information about bundled software.
