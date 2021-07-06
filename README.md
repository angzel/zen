# zen

- [Brief](#brief)

- [Structure](#structure)

- [Example](#example)

- [Lisence](#lisence)

- [Author](#author)

## Brief

 The cpp library [Zen](#zen) is a cross-os library that contains many REUSABLE codes to expand the STANDARD library.

 Some basic and simple functions were provided in a single header files, while some were defined in a pair of header and source (cpp) files. And so far, some part contains several files and provides a complex system (such as [Vap](#vap) game library). For more details, please view the [structure](#structure) chapter.

 This library is free ([LGPL v3](#https://www.gnu.org/licenses/lgpl-3.0.en.html)) for noncommercial projects.

## Structure

In the root repository, you can see two directories named "sample" (contains some example projects, see reference to [Example](#example)) and "source".

The "source" is the core part of the codes. Here are the introductions for the modules of the library.

Source Tags

Goto the pages to get more details.

- [Basic](./source/basic/README.md)

- [Platform](./source/platform/README.md)

- [Network](./source/network/README.md)

- [Third](./source/third/README.md)

- [Visual](./source/visual/README.md)

## Example

In the `sample` directory, there are some example project and test cases.

- [game](./sample/game)

  a tutorial of Vap.

- [image convert](./sample/image_convert)

  a command line program.

  convert jpg/png to png/jpg. (a test case for `zen-jpg`, `zen-png`, `zen-image`)

- [truetype](./sample/truetype)

  a command line program.

  render a string to image. (a test case for `zen-freetype`)
  
- [audio player](./sample/audio_player)

  a command line program that test audio player (`zen-player`)

## Lisence

LGPL v2.

## Author

Author: Meher Tj. Guo

E-mail: karmaleaving@qq.com
