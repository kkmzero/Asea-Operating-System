# Asea Operating System Handbook

![](./img/logo_small.png)

## Contents
1. [Basics](#basics)
2. [License](#license)
3. [Getting Asea](#getting-asea)
4. [Contributing](#contributing)
5. [Documentation](#documentation)

## Basics
Purpose of this project is mainly educational. Asea is not trying to be Unix nor Unix-like opearting system clone. Source code of this operating system should be first and foremost simple and readable.
## License
Asea Operating System and its libraries are lincensed under [GNU General Public License v3.0](../LICENSE).
## Getting Asea
#### Downloading ISO image
Currently not available.

#### Compiling from source
For compiling Asea OS, the following is recommended (required*):

1. Linux-based operating system
2. GNU Make 4.1*
3. GCC/G++ 6.3.0
4. GNU Binutils 2.28 (GNU Linker + GNU assembler)
5. libc6-dev-i386 2.24-11
6. GRUB Legacy 0.97-72
7. grub-pc-bin 2.02~beta3-5
8. xorriso 1.4.6

**Note:** You can use Makefile's commands `make checkdeps` or `make installdeps` to do this process faster.
Programming environment for Asea OS is currently set up on Debian GNU/Linux distribution, which may cause issues on other Linux distributions due to dependency on `apt` and exact names of packages in its repository.

## Contributing
Everyone is free to contribute to Asea OS and any help is welcomed. Please consider proposing feature changes/implementations in *Issues* page first.

#### Future plans and WIP:
1. Multitasking and CLI/Shell
2. Moving from 32bit processor architecture support to 64bit (x86_64)
3. Improvements of libraries
4. Fixes of bugs and known issues
5. Documentation

## Documentation
Currently not available.
