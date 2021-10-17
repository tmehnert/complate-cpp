# complate-cpp
*- Rendering of JSX based views in C++*

[![Build](https://github.com/tmehnert/complate-cpp/actions/workflows/build.yml/badge.svg)](https://github.com/tmehnert/complate-cpp/actions/workflows/build.yml)
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

### Installation
```shell
sudo apt-get install libv8-dev
mkdir build && cd build
cmake ..
make
sudo make install
sudo ldconfig
```

### Compatibility
The library is tested with following compilers.
* GCC 7 - 10 (on Ubuntu 20.04)
* Clang 7 - 12 (on Ubuntu 20.04)

#### Running Tests
Type following commands to run the unittests on your system.
```shell
mkdir build && cd build
cmake -DBUILD_TESTS=on ..
make && make test
```

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
