# Value model

The Value model is the way you can pass data from C++ to JSX and vice versa. Every type will be accessible in JSX like a
normal JavaScript type. It can hold primitives for which Value are implicit constructable and more complex data
structures like arrays, objects, functions or proxies for your own C++ classes.

In most cases you use this, to get the data into your view or use the comparison operator in your unittests, which
hopefully should be quite easy. But sometimes you have to pass data from JavaScript to C++, when you implement a
callback function for example. Then you have to pay more attention on the accessors, because JavaScript can pass you
anything.

**Accessing a value the safe way**

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

**Type conversion**

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

**Passing optionals**

```c++
Value value = optional<string>("bar");
// Is a string, because the optional had a value.
assert(value.is<string>());

Value empty = optional<string>();
// Is Undefined, because the optional was empty.
assert(empty.is<Undefined>());
```

## Undefined

A default constructed Value represent undefined. A value will become also undefined if you pass an empty optional of any
supported type (Bool, Number, Object, Proxy...) to it.

```c++
Value value;
assert(value.is<Undefined>());
value = optional<Object>();
// Stays undefined, because the optional was empty.
assert(value.is<Undefined>());
```

## Null

A null Value can be implicit constructed with nullptr.

```c++
Value value = nullptr;
assert(value.is<Null>());
```

## Bool

A boolean value can be implicit constructed with a bool.

```c++
Value value = false;
assert(value.is<bool>());
```

## Number

A number value can be implicit constructed with any supported type `int32_t`, `uint32_t`, `int64_t` and `double`.

```c++
Value value = (int32_t)-23;
// Can be access as int32_t and int64_t because the value fits into.
assert(value.is<int32_t>());
assert(value.is<int64_t>());
// Can't be access as uint32_t because it can't hold negative values.
assert(value.is<uint32_t>() == false);
```

## String

A string value can be implicit constructed with any supported type `string`, `string_view` and `const char *`. When a
string is a static constant, you should pass it explicit as a string_view, to avoid an unnecessary copy. For safety
reasons a String constructed by `const char *` will be copied.

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

## Array

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

## Object

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

## Function

Can be used to create a callback to C++, which can be used like any other JavaScript function. It is also possible to
return an optional from your function, when it can't supply a value.

**Without arguments**

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

**With arguments**

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

## Proxy

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

## ProxyWeak

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
