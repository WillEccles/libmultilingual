# libmultilingual

This is a very simple C++11 library for localizing strings.

There is one main class used to handle all localizations. This class handles everything from loading `.lang` files to accessing and reloading them. You may switch language at any time, and strings may be set up as listeners which will be changed whenever the language is changed. This is likely a niche use-case, but it's there if you need it.

## Compilation

In order to compile and install this library, do the following:

```
$ mkdir build && cd build
$ cmake ..
$ sudo make install
```

This will install the library and header at your default locations as determined by CMake.

## Linking

In order to link this library, a pkgconfig has been included. Use the shell command `pkg-config --libs --cflags multilingual` to get the compiler arguments you should specify to link this library. Then, simply `#include "multilingual.h"` and you are good to go.

For an example on how to do this using a Makefile, see the [test Makefile](test/Makefile).

## Usage

*There is [test code](test/test.cpp) available for your reading pleasure.*

The main translator template class is `multilingual::basic_translator`. While you may use this template yourself, I recommend using one of the typedefs provided: `Translator` and `WTranslator`. `Translator` is a version of `basic_translator` that is instantiated to use `std::string`, and `WTranslator` is a version of `basic_translator` that is instantiated to use `std::wstring`. All functions will work for either type. Read through the test code for an example on how to use these classes.
