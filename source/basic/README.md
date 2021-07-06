# Basic

Path to: [`source/basic`](./source/basic)

This module contains the basic funtions, such as string and math library. In the sub-directoris:

## [`zen-base`](./source/basic/zen-base)

The most basic functions are here:

- macro `"zen_macro.h"`

  operation system macros, make an uniform definition for cross-os developing.

- exception `"zen_exception.h"`

  the exception expanding `std::exception`, `Zen::Exception` contains more informations (file,line,error number) that help to debug.

- log `"zen_log.h"`

  uniform output on different platforms.

- endian `"zen_endian.h"`

  big/little endian and net/host endian transform.

- types `"zen_types.h"`

  some simple types with high frequency use.

## [`zen-string`](./source/basic/zen-string)

- url encoding `"zen_urlcoding.h"` (cpp)

  tranform between normal string and URL coded string. (encode/decode)

- md5 `"zen_md5.h"` (cpp)

  MD5 functions.

- utf8 `"zen_utf8.h"`

  transform between utf8 and unicode.

- base64 `"zen_base64.h"` (cpp)

  base64 convert.

- csv `"zen_csv.h"` (cpp)

  a csv file loader.

- buffer `"zen_buffer.h"`

  a serialize buffer

## [`zen-math`](./source/basic/zen-math)

- random `"zen_random.h"`

  random number generator.

- matrix `"zen_matrix.h"`

  matrix algorithm

