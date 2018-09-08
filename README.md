# mandelbrot-fractal-drawer
![Screenshot 1 of Gameplay](https://raw.githubusercontent.com/Balajanovski/mandelbrot-fractal-drawer/master/screenshot.png)<br>

## Dependencies
* OpenGL 
* GLFW
* LibPNG
* CMake

## Installation guide
If you've somehow come to the decision that you want to run this abomination of a program, please follow the steps below: <br>
Firstly download the dependencies listen above if you have not already<br>
```
git clone https://github.com/Balajanovski/mandelbrot-fractal-drawer.git
cd mandelbrot-fractal-drawer
cmake .
make
./mandelbrot-fractal-drawer
```

## Usage guide
The program will ask you what colour you want the fractal to be drawn as.<br>
Supply your colour in RGB format with the components being between 0 - 255 like this {R,G,B}. For example {255,255,255} is white.<br>
The program will ask you what range of complex numbers to derive the pixels from.<br>
Supply the range.<br>
The program will ask you if you want to draw to either a window or an image.<br>
Type W if you want to draw to a window<br>
Type I if you want to draw to a PNG image, then type the name of the file you want to generate and where you want to draw relative to the program<br>
Enjoy!<br>
