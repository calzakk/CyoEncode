# CyoEncode

Provides C functions for encoding binary data into a printable representation using Base64, Base32, Base16 (hex), or Base85/Ascii85 character sets, plus equivalent functions for the decoding of such encoded data back into its binary form.

## Instructions

Simply copy the required source files (CyoEncode.h/cpp and CyoDecode.h/cpp)
into your C/C++ project.

Examples of usage can be found in the test.c file.

For Unix/Linux developers, there's a shell script that will build the test
using GCC.

For Windows developers, a Visual Studio project is included.

For QT developers, a project include file (`CyoEncode.pri`) is included.

## Platforms

CyoEncode requires a C++98 compiler, and is known to build on the following platforms:

* Linux using GCC 4.9;
* Windows using Visual Studio 2015.

## License

### Simplified BSD License

All the files in this library are covered under the terms of the Berkeley Software Distribution (BSD) License:

Copyright (c) 2009-2016, Graham Bull.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

