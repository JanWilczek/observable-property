# ObservableProperty 👀

A header-only C++ utility library for creating observable class properties. `ObservableProperty` allows classes to notify listeners of changes to their data members, making it easier to build reactive components in C++.

The main use case for this library is to facilitate data binding in MVVM-style architectures in C++. You can use `ObservableProperty` similarly to an `event` in C# or `LiveData` in Android.

If you want the callback to be called on a particular thread, you can subclass `MutableObservableProperty`. See `LiveObservableProperty` for how to do it in the JUCE C++ framework.

## Installation

To use the library, you can put the _include/_ folder in your include path OR link against it in CMake:

```cmake
target_link_libraries(MyProject PRIVATE wolfsound::observable_property)
```

Just `#include <observable_property/ObservableProperty.hpp>` and you're good to go!

The library uses `boost-signals2` internally (bundled with this repo).

## Usage

A minimal example:

```cpp
#include <observable_property/ObservableProperty.hpp>

class Holder {
public:
  [[nodiscard]] ObservableProperty<int>& property() noexcept {
    return property_;
  }

  void incrementValue() { property_.setValue(property_.value() + 1); }

private:
  MutableObservableProperty<int> property_{0};
} holder;

const auto connection = holder.property().observe([&](const int& value) {
  // callback code
});

// Doesn't work although property() is public.
// holder.property().setValue(1);

// Tightly restricted API
holder.incrementValue();
```

You can use `OBSERVABLE_PROPERTY` macro to avoid boilerplate:

```cpp
#include <observable_property/ObservableProperty.hpp>

class Holder {
public:
  Holder() : property_{0} {}

  OBSERVABLE_PROPERTY(property, int)

  void incrementValue() { property_.setValue(property_.value() + 1); }
} holder;

const auto connection = holder.property().observe([&](const int& value) {
  // callback code
});

// Doesn't work although property() is public.
// holder.property().setValue(1);

// Tightly restricted API
holder.incrementValue();
```

See tests in _tests/_ folder for more details.

## License

For simplicity, this library is licensed under the Boost Software License, Version 1.0. See the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests for any improvements or new features.

