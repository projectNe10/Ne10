Copyright (c) 2011-12, ARM Limited
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
====

Copyright 2011-12 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


NE10
====

The objective of this library is to provide a set of common, useful functions
which have been heavily optimised for ARM, provide consistent well tested
behaviour and that can be easily incorporated into applications.

The primary API provided is C. The primary OS targeted is Android, although
the library is tested with Linaro Linux.

The design is intended to be available as a simple 'drop and go' pre-built
library and as a set of modular functions that can be incorporated in a more
modular pick and mix form where binary size might be an issue.

Each function is implemented in C, ARM Assembly and NEON code as a basis for
comparison. Assembly versions, while efficient, are not intended as 
best-practice examples.

Future releases are intended to expand on the functions provided and possibly
the supported languages (C++ being near the top of that list).

Licensed under the Apache License, Version 2.0

(See LICENSE for details)

Build
=====

See CMakeBuilding.txt file in the "doc" folder

Usage
=====

See USAGE.txt file in the "doc" folder

Code formatter
=====

See Formatter.txt file in the "doc" folder

