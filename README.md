# libmultilingual

This is a very simple (mostly proof-of-concept) C++11 library for localizing strings. Operation is fairly simple, and compiling is simple since it's (for the moment) header-only.

## Usage

Use the class `multilingual::Translator` to get strings from a file and localize them. There are convenience macros used for putting C++ tokens into strings for use in keys, and it has support for `std::wstring` and `wchar_t` (before including the header, `#define LIB_MULTI_WSTRING`).

The code is fairly self-explanatory, and since it's not meant to be a huge library, there is little documentation at the moment. Read through the header and the `Translator` class declaration in particular for handy comments, and read the [test code](test/main.cpp) for example usage.
