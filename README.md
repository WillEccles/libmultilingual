# libmultilingual

This is a very simple (mostly proof-of-concept) C++11 library for localizing strings. Operation is fairly simple, and compiling is simple since it's (for the moment) header-only.

## Compilation

Because this library supports either `std::string` or `std::wstring` based on precompiler flags, it is (for the moment) header-only. Therefore, there is no compilation for the library itself. Simply include the header and compile your program.

## Usage

Use the class `multilingual::Translator` to get strings from a file and localize them. There are convenience macros used for putting C++ tokens into strings for use in keys, and it has support for `std::wstring` and `wchar_t` (before including the header, `#define LIB_MULTI_WSTRING`).

The code is fairly self-explanatory, and since it's not meant to be a huge library, there is little documentation at the moment. Read through the header and the `Translator` class declaration in particular for handy comments, and read the [test code](test/test.cpp) for example usage.
