# Getting started

- [GLFW](https://en.wikipedia.org/wiki/GLFW)
- [Cairo](https://en.wikipedia.org/wiki/Cairo_(graphics))
- [OpenGL](https://en.wikipedia.org/wiki/OpenGL)

## Installing on Windows

## Installing on Linux

For Debian and Debian derivatives including Ubuntu:

```
   sudo apt-get install libcairo2-dev
```
For Fedora:

```
   sudo yum install cairo-devel
```
For openSUSE:
```
   zypper install cairo-devel
```

Also:

https://gitlab.freedesktop.org/cairo/cairo

Make a change in the meson_options.txt file:
```
   option('gl-backend', type : 'combo', value : 'disabled',
```
replace
```
   option('gl-backend', type : 'combo', value : 'gl',
```
```
   meson setup builddir
   ninja -C builddir
   ninja -C builddir install
```

```
   gcc gl_00.c -o gl_00 -lglfw3 -lGL -lcairo -lm -L /usr/local/lib/x86_64-linux-gnu
```

## Installing on Mac OSX

---
