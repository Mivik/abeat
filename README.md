
# abeat

## A simple music visualizer.

### Build

Install dependencies:

```sh
$ sudo apt install cmake libpulse-dev libfftw3-dev libgl-dev libglfw3-dev libglm-dev
```

Compiling:

```sh
$ mkdir build && cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```

Then you can get `abeat` executable under current directory.
