# Getting started

- [GLFW](https://en.wikipedia.org/wiki/GLFW)
- [Cairo](https://en.wikipedia.org/wiki/Cairo_(graphics))

## Installing on Windows

## Installing on Linux

```
sudo apt-get update
sudo apt-get install libglfw3-dev
sudo apt-get install libcairo2-dev
```

A typical compile and link command-line when using the static version of the GLFW and Cairo library may look like this:
```
gcc $(pkg-config --cflags glfw3 cairo) gl_00.c -o gl_00 $(pkg-config --static --libs glfw3 cairo)
```

If you are using the shared version of the GLFW library, omit the --static flag.
```
gcc $(pkg-config --cflags glfw3 cairo) gl_00.c -o gl_00 $(pkg-config --libs glfw3 cairo )
```

e.t.c

```
gcc gl_00.c -o gl_00 -lglfw3 -lcairo -lm
```

## Installing on Mac OSX

---
