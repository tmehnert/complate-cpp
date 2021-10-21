# complate-cpp
*- Rendering of JSX based views in C++*

[![Build](https://github.com/tmehnert/complate-cpp/actions/workflows/build.yml/badge.svg)](https://github.com/tmehnert/complate-cpp/actions/workflows/build.yml)
[![codecov](https://codecov.io/gh/tmehnert/complate-cpp/branch/main/graph/badge.svg)](https://codecov.io/gh/tmehnert/complate-cpp)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE)

[complate](https://complate.org) adapter that can be used in C++ 17 projects to
render JSX based HTML views. This project was inspired by
[complate-java](https://github.com/complate/complate-java).

## Getting started

### Usage
You can choose between two rendering engines, depending on your needs.
[QuickJS](https://bellard.org/quickjs/) which is bundled and smaller or
[V8](https://v8.dev/) which is faster, but needs the V8 library to be installed.
Both of them provide an identical interface to render HTML.
Look at `example` and `views` for a minimal example.

Define your view using JSX and compile it to plain Javascript `views.js`,
see `npm run compile`.
```jsx
import {createElement} from 'complate-stream';

export default function Greeting ({ person }) {
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

Create a renderer with your `views.js` bundle and render HTML.
```c++
  using namespace complate;
  QuickJsRenderer renderer("<content-of-your-views.js>");
  BasicStream stream(std::cout);

  Object parameters;
  parameters.emplace("person", Object{{"name", "John Doe"}});
  renderer.render("Greeting", parameters, stream);
```

### Running tests and example
The unittests and the example are not built by default, you have to enable
them using BUILD_TESTS=on and BUILD_EXAMPLE=on as shown below.
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
While the example is running you can run `npm start` in another terminal,
edit [views/greeting.jsx](views/greeting.jsx) and refresh your browser.
You will see your changes without restarting the example.
This happens because the example make use of the complate::ReEvaluatingRenderer
which can speed up your local development.

### Installation
```shell
sudo apt-get install libv8-dev
mkdir build && cd build
cmake ..
make
sudo make install
sudo ldconfig
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
Another possibility, without having to install the library, is let CMake fetch
it. When you use this method, you may not use `find_package(complate)`.
Just put the following commands in your CMakeLists.txt file
and replace the GIT_TAG `HEAD` with the commit-sha or git-tag you want.
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

### Compatibility
The library is tested with following compilers.
* GCC 7 - 10 (on Ubuntu 20.04)
* Clang 7 - 12 (on Ubuntu 20.04)

### Dependencies
* QuickJS, MIT Licence (bundled)
* V8, MIT Licence

#### Dev dependencies
* Catch2, BSL-1.0 (fetched via CMake)
* Trompeloeil, BSL-1.0 (fetched via CMake)
* cpp-httplib, MIT License (fetched via CMake)
* Node.js
* CMake >= 3.14 required

## License
complate-cpp is Open Source software released under the [Apache 2.0 license](LICENSE).

See [LICENCE](LICENSE) for more information about bundled software.
