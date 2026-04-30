# utf8-codec

A low-level implementation of UTF-8 encoding and decoding in C.

## Features

* Decode UTF-8 byte sequences into Unicode code points
* Encode Unicode code points into UTF-8 bytes
* Supports 1–4 byte UTF-8 sequences
* Handles mixed-width UTF-8 strings

## Example

Input:

```
Aس€😉
```

Output:

```
U+0041
U+0633
U+20AC
U+1F609
```

## Build

```
gcc [version]/src/*.c -Iinclude -o utf8
```

## Run

```
./utf8
```

## Learning Goals

This project explores:

* UTF-8 encoding rules
* Bit manipulation in C
* Unicode code points
* Memory management

## Notes

This is a learning implementation and does not yet include full UTF-8 validation (e.g. overlong sequences, invalid ranges).

' codec is a computer hardware or software component that encodes or decodes a data stream or signal. '
