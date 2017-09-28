# mandelbrot-fractal-drawer
My first implementation of a program to render the mandelbrot fractal.
It is pretty primitive when it comes to functionality, but it can render the image to an OpenGL context or an image.

[![fractal.png](https://s10.postimg.org/r6mpm6ks9/fractal.png)](https://postimg.org/image/jdw1u7et1/)

## Dependencies
* OpenGL 
* GLFW
* LibPNG
* CMake

## Disclaimer
I have used Benhiller's opencl source code to accelerate my mandelbrot drawer from this project:
https://github.com/benhiller/opencl-mandelbrot

## Installation guide
If you've somehow come to the decision that you want to run this abomination of a program, please follow the steps below: <br>
1. Download the dependencies listen above if you have not already
2. Clone this repository by typing `git clone https://github.com/Balajanovski/mandelbrot-fractal-drawer.git ` into the command line
3. Enter the repository via command line
4. Type `cmake .` {This generates a make file to build the program}
5. Type `make` {This will build the executable}
6. Now just run the produced executable and enjoy

## Usage guide
The program will ask you what colour you want the fractal to be drawn as.<br>
Supply your colour in RGB format with the components being between 0 - 255 like this {R,G,B}. For example {255,255,255} is white.
The program will ask you what range of complex numbers to derive the pixels from.
Supply the range like so ()
The program will ask you if you want to draw to either a window or an image.<br>
Type W if you want to draw to a window<br>
Type I if you want to draw to a PNG image, then type the name of the file you want to generate and where you want to draw relative to the program<br>
Enjoy!

## To Do:
1. Different image files (JPEG, GIF, etc...)
