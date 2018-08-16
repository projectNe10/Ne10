# Building Ne10

## Prerequisites

Ne10 uses [CMake](https://cmake.org/) to manage the project's build process in a toolchain-independent, cross-platform manner. As such, you will need CMake installed to build Ne10. It can typically be obtained via a package manager (for example, with `sudo apt-get install cmake`), but can otherwise be built from source. More information about CMake is available at [cmake.org](https://cmake.org/).

All builds also require a local copy of the codebase. If you haven't already done so, you can download the latest Ne10 release as a [zip](https://github.com/projectNe10/Ne10/zipball/master) or [tarball](https://github.com/projectNe10/Ne10/tarball/master). Or, alternatively, you can clone the current cutting-edge version of Ne10 directly with Git by running:

```bash
git clone https://github.com/projectNe10/Ne10
```

## Native compilation on \*nix platforms

As CMake does most of the cross-platform heavy lifting, Ne10 can be natively compiled on \*nix systems by running just a few simple commands:

```bash
cd $NE10_PATH                       # Change directory to the location of the Ne10 source
mkdir build && cd build             # Create the `build` directory and navigate into it
export NE10_LINUX_TARGET_ARCH=armv7 # Set the target architecture (can also be "aarch64")
cmake -DGNULINUX_PLATFORM=ON ..     # Run CMake to generate the build files
make                                # Build the project
```

This process, if completed successfully, will yield the `libNE10.a` static library file in the `$NE10_PATH/build/modules/` directory. In addition, a sample program `NE10_test_static` compiled with this static library will be generated in `$NE10_PATH/build/samples/`.

You can also generate a dynamic, shared library file by passing CMake the `-DNE10_BUILD_SHARED=ON` option. This generates a handful of `*.so` files in the same directory as the static library file, in addition to the dynamically linked `NE10_test_dynamic` sample program.

By default, the release version of the library is built. The debug version can instead be produced by passing the `-DBUILD_DEBUG=ON` option to CMake. Note also that the target architecture can be varied by modifying the `NE10_LINUX_TARGET_ARCH` environment variable (or, overriding this, the `-DNE10_LINUX_TARGET_ARCH=$ARCH` CMake option).

## Cross compilation on \*nix platforms...

### ...for other general \*nix platforms

Cross-compiling Ne10 for \*nix platforms is similar to native compilation, but using a cross-compilation toolchain. A boilerplate toolchain file `GNUlinux_config.cmake` is present in the root directory of the project to allow for cross-compilation using the Linaro GCC ARM toolchain (obtained, for example, via `sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf`), but this can also be modified to work with other toolchains. Using this augments the compilation process to look something like the following:

```bash
cd $NE10_PATH
mkdir build && cd build
export NE10_LINUX_TARGET_ARCH=armv7                      # Can also be "aarch64"
cmake -DCMAKE_TOOLCHAIN_FILE=../GNUlinux_config.cmake ..
make
```

The products of the build are identical to that of native compilation, with the generated library files in `$NE10_PATH/build/modules/`. The dynamic and debug versions of the library can also be produced using the same CMake options as in native compilation. When running the `NE10_test_dynamic` program produced from compilation of the shared library, keep in mind that the library file must be in a known location such that it can be properly loaded and used (for example, by setting the `LD_LIBRARY_PATH` environment variable appropriately).

It's also worth keeping in mind that Ne10 uses some GAS (GNU Assembler) syntax, which can make building with other compilers — such as the LLVM-based `clang` — a bit awkward if these files are included in the build. This is dealt with in the build instructions for common platforms (below) where appropriate, translating the problematic syntax into supported equivalents, but may require more involved CMake configuration changes for more obscure compilation environments.

Additionally, for systems without hardware floating point support, the appropriate compilation options should be added to the `CMAKE_C_FLAGS` and `CMAKE_ASM_FLAGS` variables in the root `CMakeLists.txt` file. For example, `-mfloat-abi=softfp -mfpu=neon`.

### ...for Android

As Android is itself a \*nix platform, all the information about compilation to general \*nix platforms applies. Android is a particularly common use case for Ne10, however, and so the project provides a pre-configured CMake toolchain file especially for compilation using Android's Native Development Kit (NDK). Thus, to cross-compile for Android, the following steps can be used:

```bash
cd $NE10_PATH
mkdir build && cd build
export ANDROID_NDK=/absolute/path/of/android-ndk
export NE10_ANDROID_TARGET_ARCH=armv7                           # Can also be "aarch64"
cmake -DCMAKE_TOOLCHAIN_FILE=../android/android_config.cmake ..
make
```

This Android toolchain file also uses two additional environment variables which can be set as appropriate for the target:

- `ANDROID_API_LEVEL` specifies the Android API level.
    - This defaults to the value 14, but must be greater than or equal to 21 for AArch64 targets.
- `ARM_ANDROID_TOOLCHAIN_VERSION` specifies the GCC version to be used.
    - This defaults to 4.6, but must be greater than or equal to 4.8 for AArch64 targets.


### ...for iOS

As iOS is itself a \*nix platform, all the information about compilation to general \*nix platforms applies. Like with Android, iOS is an especially common use case for Ne10, and so a pre-configured CMake toolchain file is provided to make the compilation process easier. It is assumed within this that iOS builds will be performed using macOS with a copy of Xcode installed.

As well as selecting the correct compilation tools, the iOS toolchain file also activates infrastructure within the CMake configuration files to translate any problematic GAS assembly syntax into a form that `clang` can process. Thus, to cross-compile for iOS, the following steps can be used:

```bash
cd $NE10_PATH
mkdir build && cd build
export IOS_DEVELOPER_PATH=/absolute/path/of/ios/developer/path
export NE10_IOS_TARGET_ARCH=armv7                               # Can also be "aarch64"
cmake -DCMAKE_TOOLCHAIN_FILE=../ios/ios_config.cmake ..
make
```

With the default Xcode install path, the `IOS_DEVELOPER_PATH` variable should be set to `/Applications/Xcode.app/Contents/Developer`.

The infrastructure surrounding the iOS platform configuration also allows for an additional option `MIN_IOS_VER` to be specified to CMake (e.g. `-DMIN_IOS_VER=5.0`) to specify the minimum supported iOS version. When targetting iOS versions older than 7.0, setting this variable to 7.0 will result in a runtime error.

In the case that CMake gives a warning regarding the `CMAKE_OSX_SYSROOT` variable, please ensure that `IOS_DEVELOPER_PATH` is set correctly, as the former variable is derived from the latter in the iOS toolchain file. If necessary, this SDK path can be set explicitly by directly modifying the `ios_config.cmake` file.

## Building the unit tests

Ne10 has a number of unit tests, built around a modified version of the testing framework "seatest". Particularly, the project has three different modes of testing:

- Conformance testing (also called smoke testing), to check if the library works correctly.
- Regression testing, which is similar to conformance testing but is aimed more specifically at testing whether the library still operates correctly after a change.
- Performance testing, which gives an indication of how quickly the library performs certain tasks.

These can be built by specifying `-DNE10_SMOKE_TEST` with one of `smoke`, `regression`, and `performance`. The corresponding test programs for each Ne10 module will then be generated in the `$NE10_PATH/build/test/` directory.


## Building the Android and iOS demo applications

Ne10 also comes with two demo applications, one for Android, and another for iOS. Both demos need the Ne10 library to be installed on the system, which can be accomplished by running `make install` after building the project. Following this, the project files can be opened in Xcode or the Android SDK as usual. Devices are expected to have Internet access in order to run the demo applications correctly.
