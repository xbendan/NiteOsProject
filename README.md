# **Nite OS Project**

Graphical (not yet) operating system for x86_64 and risc-v computers.


## About 
**What is Nite OS?**  
Nite is a full system project written in C/C++ (primarily C++, with OOP features), aim to run in modern desktop and laptop devices. This repository includes Kernel **「Siberix」**, Runtime libraries and basic applications and an XML-like UI framework to build UI-based program.

## Build and Test
To build this operating system, you need `nasm, x86_64-elf-g++, xorriso, python3`. When these packages were installed, go to any sub directory and run `python3 ../Scripts/build-<...>.py`. After that, compiled files will generate in `build` or `dist` directory.
### Requirements
- `nasm`
- `build-essential`
- `x86_64-elf-g++`  
- `xorriso`  
- `python3`  

move to sub folders (Kernel, Apps/x, etc.)  
use command "`python3 ../Scripts/build-<...>.py`"

For example, if you want to build the kernel, go to `Kernel` directory and run `python3 ../Scripts/build-os.py`. And you will see the image file generated in distribution folder.  

## Features


## Architectures
- [x] x86 [64-bit, x64, amd64] **(In progress)**  
- [ ] arm [64-bit]  
- [ ] risc-v [64-bit]  
*There is no future plan to support 32-bit archs because they are already outdated.*

## Contribute