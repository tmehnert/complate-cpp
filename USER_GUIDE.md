# User Guide

This guide should help you to get started. When you need more detail look into the public headers of the library, which
are under **#include<complate/...>**. All public complate-cpp types reside in the namespace **complate**, which is
omitted in this guide for simplicity.

## Table of contents

- [Generating views](#generating-views)
    - [Setup Pipeline](#setup-pipeline)
    - [Create your first JSX view](#create-your-first-jsx-view)
- [Instantiate a Renderer](#instantiate-a-renderer)
    - [Choose a Renderer implementation](#choose-a-renderer-implementation)
    - [Global bindings for your views](#global-bindings-for-your-views)
    - [Prototypes for your own classes](#prototypes-for-your-own-classes)
    - [ThreadLocalRenderer](#threadlocalrenderer)
    - [ReEvaluatingRenderer](#reevaluatingrenderer)
- [Rendering HTML](#rendering-html)
    - [Render to string](#render-to-string)
    - [Render to stream](#render-to-stream)
    - [Exception handling](#exception-handling)
    - [More realistic JSX for the examples above](#more-realistic-jsx-for-the-examples-above)
- [Value model](#value-model)
    - [Primitives](#primitives)
    - [Array](#array)
    - [Object](#object)
    - [Function](#function)
    - [Proxy](#proxy)
    - [ProxyWeak](#proxyweak)
- [Appendix JSX](#appendix-jsx)
    - [Reusable components](#reusable-components)
    - [UI logic on the server](#ui-logic-on-the-server)
    - [UI logic on the client](#ui-logic-on-the-client)

## Generating views

Your views need be packaged into a single **views.js** file, which have to be delivered alongside your application. The
renderer will load this file when it becomes initialized. For the next steps you must have **npm** installed on your
development machine or ci system. Let's assume you have an empty project and get through the steps.

### Setup Pipeline

In this section you will create your pipeline in order be able to create views. We will
use [faucet-pipeline](https://www.faucet-pipeline.org/) as our asset pipeline in this example.

#### Initialize your package.json

```shell
npm init
# This package contains the rendering code.
npm install complate-stream
# These packages needed to transform your views during compilation time.
npm install --save-dev \
  @babel/plugin-transform-react-jsx \
  faucet-pipeline-esnext \
  faucet-pipeline-js
```

#### Adding a faucet.config.js file with following content to root directory

```js
"use strict";

// The directory in which you would like to place your JSX views.
let templateDir = "./views";
module.exports = {
    watchDirs: [templateDir],
    js: [{
        source: templateDir + "/index.js",
        // The path where your bundle should be generted.
        target: "./views.js",
        exports: "render",
        esnext: {
            exclude: ["complate-stream"]
        },
        jsx: {pragma: "createElement"}
    }]
};
```

#### Adding a views/index.js file with following content

```js
import Renderer from "complate-stream";

// Here your views will be registered. We will add your first view later.
const renderer = new Renderer({
    doctype: '<!DOCTYPE html>'
});

// This will export the render function which will be called from C++.
export default function render(view, params, stream) {
    renderer.renderView(view, params, stream)
}
```

#### Add some useful scripts to your package.json to run the pipeline

```json
{
  "scripts": {
    "start": "npm run compile -- --watch",
    "compile": "faucet --compact"
  }
}
```

#### Testing your pipeline

```shell
# Run following, which compile your views once and terminate.
npm run compile
# Or run following, which will compile continuously as you edit your views.
npm start
# Stop "npm start" by pressing STRG+C.
# Congratulations you have configured your view pipeline properly.
```

### Create your first JSX view

In this section you will create a JSX view and register it to the engine. After adding the files, please
run `npm run compile` to check if it works.

#### Adding a views/greeting.jsx file with following content

```jsx
// This line have to be included in every view.
import {createElement} from 'complate-stream';

// Parameter "person" is a view parameter, which will be passed from C++.
export default function Greeting({person}) {
    return <html>
    <head>
        <meta charSet="UTF-8"/>
    </head>
    <body>
    <h1>Hello {person.name}</h1>
    </body>
    </html>
}
```

#### Register your view by adding this to views/index.js

```js
import Greeting from "./greeting"

// Place after instantiate the Renderer.
// Use the name "Greeting", when refering this view from C++.
renderer.registerView(Greeting)
```

## Instantiate a Renderer

Now we go inside the C++ code of your application and set up the renderer. There are some possibilities you have, which
are described in this section.

### Choose a Renderer implementation

You can either choose [QuickJS](https://bellard.org/quickjs/) or [V8](https://v8.dev/) for the underlying JavaScript
engine. QuickJs is smaller and bundled, V8 is faster but need the V8 libraries installed on your system. Both of them
implement the same interface to render HTML, so you can easily change your decision later.

```c++
#include <complate/quickjs/quickjsrendererbuilder.h>
// or
#include <complate/quickjs/v8platform.h>
#include <complate/quickjs/v8rendererbuilder.h>

// Here we are using the builder, because it's more fluid.
auto qjsRenderer = QuickJsRendererBuilder()
    .source("<content-of-your-views.js")
    .build();

// Instantiate exactly one V8Platform, before using the V8Renderer.
V8Platform v8Platform;
// Afterwards you can use the V8Renderer normally.
auto v8Renderer  = V8RendererBuilder()
    .source("<content-of-your-views.js")
    .build();
```

### Global bindings for your views

When instantiate a renderer you can pass an Object which holds global variables that can be accessed from every view.
The bindings can contain every type of the [Value model](#value-model) and will be accessible as JavaScript globals.
Just pass everything you want to use in a global context into the binding.

```c++
auto bindings = Object{
  // "application" will become a global Object.
  { "application", Object{
    // With a property "name".
    { "name", "Example" }
  }}
};

auto renderer = QuickJsRendererBuilder()
    .source("<content-of-your-views.js")
    // Pass your bindings to the builder.
    .bindings(bindings)
    .build();
```

In JSX just declare the global you want to use as a comment and then use it.

```jsx
/* global application */
import {createElement} from 'complate-stream';

export default function About({}) {
    return <div>
        <span>{application.name}</span>
    </div>
}
```

### Prototypes for your own classes

When you want to make your C++ class available in the JavaScript engine, you have to provide a prototype for your class.
After defined and passed to the renderer, you can wrap your class into a [Proxy](#proxy) or [ProxyWeak](#proxyweak).
This way you can use them in the global bindings Object or the view parameters Object.

```c++
// Let's assume you have a class Person
class Person {
public:
  const string& getForename() const { return m_forename; }
  const string& getLastname() const { return m_lastname; }
  string generateGreeting(const string &greet) const {
    return = greet + " " + m_forename + " " + m_lastname;
  }
private:
  string m_forename, m_lastname;
};

#include <complate/core/prototypebuilder.h>

// You can omit the string, in this case std::type_info::name will be used.
auto prototype = PrototypeBuilder<Person>("Person")
    // Will become a read-only properties in JavaScript
    .property("forename", &Person::getForename)
    .property("lastname", &Person::getLastname)
    // Will become a method in JavaScript
    .method("greeting", [] (Person &p, const Array &args) {
      // See the section "Value model" at this guide to learn more about Array.
      return p.generateGreeting(args.at(0).get<string>().value_or(""));
    });

// Usually you will have more than one prototype.
vector<Prototype> prototypes = {prototype};
auto renderer = QuickJsRendererBuilder()
    .source("<content-of-your-views.js")
    // Pass your prototypes to the builder.
    .prototypes(prototypes)
    .build();
```

If you want your properties to be writable, your can pass a third argument to `property` as setter which
accept `const Value &` or use a lambda `[] (Person &p, const Value &value) {...}` if you don't want to modify your
class. This also applies to methods, you can use a member function pointer or supply a lambda both will be accepted.

### ThreadLocalRenderer

This renderer instantiates and holds a renderer instance per thread. A renderer can render only one view at a time, when
you are in a multi-threaded environment it could be much faster to use one renderer per thread.

```c++
#include <complate/core/threadlocalrenderer.h>

// Make a function that load your view.js bundle from filesystem.
string loadViewsJsFromFile() { return "<content-of-your-views.js"; }

// Wrap your renderer, but use creator() instead of build().
auto renderer = ThreadLocalRenderer(QuickJsRendererBuilder()
    .source(loadViewsJsFromFile)
    .creator()
);
```

### ReEvaluatingRenderer

This renderer is a development tool to make your work more comfortable. It can wrap any other renderer and instantiate
it every time you render a view. Because of this behaviour you can edit your JSX and get the changes without having to
restart your application. You shouldn't use this in your release, because it involves a lot of overhead and prevent the
optimizing compiler of V8 to speed things up.

```c++
#include <complate/core/reevaluatingrenderer.h>

// Make a function that load your view.js bundle from filesystem.
string loadViewsJsFromFile() { return "<content-of-your-views.js"; }

// Wrap your renderer, but use creator() instead of build().
auto renderer = ReEvaluatingRenderer(QuickJsRendererBuilder()
    .source(loadViewsJsFromFile)
    .creator()
);
```

## Rendering HTML

Now all is prepared, we can use the renderer, passing the view name and parameters to it and doing somewhat with the
HTML output. In this section you also see what happens when an error in JSX occurs and how to handle them.

### Render to string

This is the easiest way is to render your HTML. Just call the renderer and get your html back.

```c++
// Let's assume you have set up your renderer with source, bindings and prototypes.
unique_ptr<Renderer> renderer;

// Define some parameters you want to pass to your view
auto person = make_shared<Person>("John", "Doe");
auto parameters = Object{
  { "person", Proxy{ "Person", person } },
  { "links", Object{
    { "support", "https://example.org/support" },
    { "homepage", "https:// example.org"}
  }}
};

// You can use renderToString to get the rendered content.
string html = renderer->renderToString("Greeting", parameters);
```

### Render to stream

You can achieve **progressive rendering** by using a Stream. The difference is that instead the renderer return the
HTML, it's continuously written to the stream while complate generates it. You might have to implement the Stream
interface in order to forward HTML written to your webservers output channel. Two very simple implementations of Stream
are included (BasicStream and StringStream).

```c++
#include <complate/core/basicstream.h>
// Let's assume you have set up your renderer with source, bindings and prototypes.
unique_ptr<Renderer> renderer;

// Define some parameters you want to pass to your view
auto person = make_shared<Person>("John", "Doe");
// Let's assume you set up your view parameters like in the example above.
auto parameters = Object{};

// This way the HTML will be written piece by piecewise to the stream.
BasicStream stream(std::cout);
renderer->render("Greeting", parameters, stream);
```

### Exception handling

A renderer will throw **complate::Exception**, which derived from **std::runtime_error**, when an error occurs. This
usually happens if there is some error in JSX like accessing an undefined object.

```c++
// Let's assume you have set up your renderer with source, bindings and prototypes.
unique_ptr<Renderer> renderer;
// Let's assume you set up your view parameters like in the example above.
auto parameters = Object{};

try {
  string html = renderer.renderToString("Greeting", parameters);
} catch (runtime_error &e) {
  cerr << "Render[Greeting]: " << e.what() << endl;
}
```

### More realistic JSX for the examples above

This is a slightly more realistic example of the "Greeting" view. It should act as a preview of what's possible with
JSX. Some idioms will be documented in the [Appendix JSX](#appendix-jsx) section of this guide, but there are many
better JSX guides on the web.

```jsx
import {createElement} from 'complate-stream';
// Reusable JSX components imported from other files
import Layout from './components/layout';
import Link from './components/link';

// JSX components feel like native HTML when used. I love it!
export default function Greeting({person, links}) {
    return <Layout title="Greeting | Example">
        <h1>Hello {person.forename} {person.lastname}</h1>
        <p>
            Click <Link href={links.support} target="_blank"><b>here</b></Link> to get some help.
        </p>
    </Layout>
}
```

## Value model

The Value model is the way you can pass data from C++ to JSX and vice versa. Every type will be accessible in JSX like a
normal JavaScript type. It can hold primitives like undefined, null, bool, number and string and more complex data
structures like arrays, objects, functions or proxies for your own C++ classes.

In most cases you use this, to get the data into your view or use the comparison operator in your unittests, which
hopefully should be quite easy. But sometimes you have to pass data from JavaScript to C++, when you implement a
callback function for example. Then you have to pay more attention on the accessors, because JavaScript can pass you
anything.

#### Accessing a value the safe way

```c++
Value value = "foo";
// Always check the type via is<T>() before accessing it with get<T>().
assert(value.is<string>());
assert(value.get<string>() == "foo");
// Or use optional<T>() with returns an std::optional and is always safe.
assert(value.optional<string>().has_value());
assert(value.optional<string>().value() == "foo");
assert(value.optional<string>().value_or("") == "foo");
```

#### Type conversion

```c++
Value i32value = (int32_t)-23;
// Can be accessed as an int64_t because the value fits into.
assert(i32value.is<int64_t>());
// Can't be accessed as an uint32_t because it can't hold a negative values.
assert(i32value.is<uint32_t>() == false);

Value i64value = (int64_t)-23;
// Can be compared, regardless of the exact type.
assert(i32value == i64value);
```

#### Passing optionals

```c++
Value value = optional<string>("bar");
// Is a string, because the optional had a value.
assert(value.is<string>());

Value empty = optional<string>();
// Is Undefined, because the optional was empty.
assert(empty.is<Undefined>());
```

### Primitives

A Value of a primitive is implicit constructable to avoid boilerplate code.

#### Undefined

A default constructed Value represent undefined. A value will become also undefined if you pass an empty optional of any
supported type (Bool, Number, Object, Proxy...) to it.

```c++
Value value;
assert(value.is<Undefined>());
value = optional<Object>();
// Stays undefined, because the optional was empty.
assert(value.is<Undefined>());
```

#### Null

A null Value can be constructed with nullptr.

```c++
Value value = nullptr;
assert(value.is<Null>());
```

#### Bool

A boolean value can be constructed with a bool.

```c++
Value value = false;
assert(value.is<bool>());
```

#### Number

A number value can be constructed with any supported type `int32_t`, `uint32_t`, `int64_t` and `double`. The explicit
typecast to **int32_t** in the first is only for documentation purpose and is not required.

```c++
Value value = (int32_t)-23;
// Can be access as int32_t and int64_t because the value fits into.
assert(value.is<int32_t>());
assert(value.is<int64_t>());
// Can't be access as uint32_t because it can't hold negative values.
assert(value.is<uint32_t>() == false);
```

#### String

A string value can be constructed with  `string`, `string_view` and `const char *`. When a string is a static constant,
you should pass it explicit as a string_view, to avoid an unnecessary copy. For safety reasons a String constructed
by `const char *` will be copied.

```c++
Value value = "foo";
// Can be accessed as string_view, no copy will occur.
assert(value.is<string_view>());
// Can be accessed as string, will make a copy.
assert(value.is<string>());

value = (const char*)nullptr;
// Okay, but will become Null
assert(value.is<Null>());
```

### Array

An Array is a using directive for **vector\<Value\>**, so you can handle it like a standard C++ vector. It can hold
every type of Value, that complate-cpp supports.

```c++
auto arr = Array{
  Object{
      { "what", "Change the tires of your car" },
      { "done", true }
  },
  Object{
      { "what", "Book a hotel for next summer" },
      { "done", false }
  }
};
Value value = arr;
assert(value.is<Array>());

// Can contain different types of value.
Array mixed = { nullptr, true, 1, 3.1415, "foo", Array{ 2, 3 } };
// There are 6 elements in this array.
assert(mixed.size() == 6);
```

### Object

An Object is a using directive for **map<string, Value>**, so you can handle it like a standard C++ map. It can hold
every type of Value, that complate-cpp supports. This is the most important type, because when rendering a view or pass
global bindings during the creating of a renderer, this type is expected.

```c++
auto obj = Object{ 
  { "email", "support@example.org" },
  { "phone", "+00 123 456789"},
  { "languages", Array{ "en", "de" } }
};
Value value = obj;
assert(value.is<Object>());
```

### Function

Can be used to create a callback to C++, which can be used like any other JavaScript function. It is also possible to
return an optional from your function, when it can't supply a value.

#### Without arguments

```c++
string getSoftwareVersion() { return "0.0.1"; }
// Can be constructed by a function pointer.
auto version = Function{getSoftwareVersion};
// Can be constructed by a lambda.
auto build   = Function{[] {
  return 23;
}};
Value value = version;
assert(value.is<Function>());
```

#### With arguments

```c++
string toUppercaseA(const Array &args) { return "first arg -> to upper"; }
// Can be constructed by a function pointer.
auto uppercase = Function{toUppercaseA};

string toUppercaseB(const string &str) { return "str -> to upper"; }
// You can use a lambda as a wrapper, if not want to modify your function.
uppercase = Function{[] (const Array &args) {
  return "toUppercaseB(first arg with type checked)";
}};
Value value = uppercase;
assert(value.is<Function>());
```

### Proxy

Can be used map your C++ class directly in JSX. Your class will usable with its properties and methods like a normal
JavaScript object. The Proxy will hold a **shared_ptr** so the lifetime of your class instance is managed.

```c++
auto person = std::make_shared<Person>("John", "Doe");
// Pass the same class name, as used at your Prototype.
Value value = Proxy{"Person", person};
// Or none if type_info::name should be used.
value = Proxy{person};
assert(value.is<Proxy>());
```

### ProxyWeak

Can be used map your C++ class directly in JSX. Your class will usable with its properties and methods like a normal
JavaScript object. The Proxy will hold a **raw pointer** which have to stay valid as long as the ProxyWeak is used.

```c++
Person person("John", "Doe");
// Pass the same class name, as used at your Prototype
Value value = ProxyWeak{"Person", &person};
// Or none if type_info::name should be used.
value = ProxyWeak{&person};
assert(value.is<ProxyWeak>());
```

## Appendix JSX

This appendix can only be a preview of what is possible with JSX. I recommend you to read some better documentation
about JSX and [Web components](https://www.webcomponents.org/). You should start
at [complate.org](https://complate.org/).

### Reusable components

In order to make your components reusable, start by putting them in a separate file to use it across your application or
in a component library like [complate-fractal](https://github.com/complate/complate-fractal) to use it across multiple
applications and languages like C++, Java, JavaScript or Ruby. Let's start with a separate file. Create a folder
**views/components** and place a file **link.jsx** in it. It's used in the example shown
at [More realistic JSX for the examples above](#more-realistic-jsx-for-the-examples-above). Because of the **children**,
which is the content between the opening and closing tag of Link, this is might be as composable as html itself.

`<Link href="https://example.org">Everything, <b>including tags</b>, can be placed here</Link>`

```jsx
import {createElement} from 'complate-stream';

export default function Link({href, target, classNames}, ...children) {
    return <a href={href} target={target} class={classNames}>
        {{children}}
    </a>
}
```

### UI logic on the server

Here is an example how to put some UI logic inside your components. Usually set some additional CSS classes or
conditionally render elements. Let's make an example TodoList, where a list of Todos have a property **what**, which
describes what to do, and can be **veryLate**, which require additional attention.

```jsx
import {createElement} from 'complate-stream';
import Layout from './components/layout';

export default function TodoList({todos}) {
    return <Layout title="My todos">
        <h1>My Todos</h1>
        <section>
            {(todos.length >= 0) ? todos.map(Todo) : <p>All work done!!!</p>}
        </section>
    </Layout>
}

function Todo(todo) {
    return <div class={`todo ${todo.veryLate && 'todo--very-late'}`}>
        {todo.what}
    </div>
}
```

### UI logic on the client

A typical idiom to put some logic on client side is to make use of [Web components](https://www.webcomponents.org/) to
create a custom html element which triggers your JavaScript code on client side. In this example a custom element
**\<to-do\>** is used. For getting this to execute some Code, you would have to define this element and its behaviour in
our clientside JavaScript assets.

```jsx
function Todo(todo) {
    return <to-do class={`todo ${todo.veryLate && 'todo--very-late'}`}>
        {todo.what}
    </to-do>
}
```