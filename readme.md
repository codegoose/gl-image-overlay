# Image Overlay Program

This is a small program that loads an image and uses the GPU to apply a text overlay to it and save that as a new image.

# Build

The Conan the build system is used to fetch dependencies and CMake does the rest.

```
mkdir build
cd build
conan install .. -s build_type=Debug
cmake ..
cmake --build . --config Debug
```

```
mkdir build
cd build
conan install .. -s build_type=Release
cmake ..
cmake --build . --config MinSizeRel
```