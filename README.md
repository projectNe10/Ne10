# What’s Ne10?
Ne10 is a library of common, useful functions that have been heavily optimised for ARM-based CPUs equipped with [NEON](https://www.arm.com/products/processors/technologies/neon.php) SIMD capabilities. It provides consistent, well-tested behaviour, allowing for painless integration into a wide variety of applications. The library currently focuses primarily around math, signal processing, image processing, and physics functions.

# Building
Out of the box, Ne10 supports the Linux, Android, and iOS platforms. For instructions on building Ne10 for these platforms, please consult the build instructions in [`doc/BuildingNe10.txt`](https://github.com/joesavage/Ne10/blob/master/doc/BuildingNe10.txt). It is possible to use the library on other platforms (or, indeed, “no platform”), however you may have to fiddle with some of the build configuration files.

# Documentation
Ne10’s official documentation is generated from [doxygen](https://www.stack.nl/~dimitri/doxygen/) annotations in the source code. As such, these documents can be generated locally from the codebase by running `doxygen doxygen.cfg` within `doc/doxygen` (producing results in `doc/doxygen/documentation`), or viewed remotely as [hosted copies available over the Internet](http://projectne10.github.io/Ne10/doc/).

We also have a handful of carefully prepared sample code snippets in the [`samples/`](https://github.com/projectNe10/Ne10/tree/master/samples) directory that outline how Ne10 can be used to accomplish a number of common tasks. These include example programs to perform the [FFT](https://github.com/projectNe10/Ne10/blob/master/samples/NE10_sample_complex_fft.c), [FIR](https://github.com/projectNe10/Ne10/blob/master/samples/NE10_sample_fir.c), and [matrix multiply](https://github.com/projectNe10/Ne10/blob/master/samples/NE10_sample_matrix_multiply.c) operations.

# Contributing
Ne10 welcomes and encourages external contributions of any and all forms. If you’ve found a bug or have a suggestion, please don’t hesitate to detail these in the [official issue tracker](https://github.com/projectNe10/Ne10/issues). For those looking to get their hands dirty and contribute code (the best kind of contribution!), please see [contributing.md](https://github.com/projectNe10/Ne10/blob/master/contributing.md) for more details.

# Miscellaneous

- Ne10’s official website: http://projectne10.org/
- Latest release notes: [`doc/ReleaseNote.txt`](https://github.com/projectNe10/Ne10/blob/master/doc/ReleaseNote.txt)
- Automated code formatting instructions: [`doc/Formatter.txt`](https://github.com/projectNe10/Ne10/blob/master/doc/Formatter.txt)

Find Project Ne10 useful? You can help us justify spending more engineering resources on the project! Please email us, outlining how you are using the project in your applications.

Want us to help cross-promote your product using Ne10 at developer events? We’re also looking for Ne10 use cases to show at conferences and meetups.

# Contacts

- roberto.mijat@arm.com
- matthew.dupuy@arm.com
- fang.bao@arm.com
