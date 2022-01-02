# nbt
A command-line NBT parser

## Description

This project is a command-line parser for Minecraft's [NBT
format](https://minecraft.fandom.com/wiki/NBT_format). It converts the binary
save data generated by Minecraft into human-readable text, called SNBT
(Stringified NBT).

The parser is written in C99-compliant C with no external dependencies.

## Building

Simply run `make` to build the program. The resulting binary will be called
`nbt`.

## Usage

Run `nbt -h` to print usage information for the program.

Minecraft saves its data in gzip-compressed NBT files. This program doesn't
decompress this data, so you must do that beforehand. You can use the following
pipeline on UNIX-like systems.

```sh
# Template
gzip -dc <compressed NBT file> | nbt [options]

# Example
gzip -dc level.dat | nbt
```