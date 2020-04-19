# Soft Body Physics Demo

 [Try out the demo in your Browser.](https://cthoens.github.io/SoftBodyPhysics/)

## Description
The system is based on a very basic spring-mass model using the Euler method to solve the ODE. It calcuates forces resulting 
from both stretching and bending. The latter results from the angle between two edges differing from the resting angle (the 
angle the simulation was started in). Visualization of both types of forces, as well as stretch / compression of edges can be enabled.

- Status: Inactive
- Technology: Emscripten to compile C++ to webassembly.

## Building
With Emscripten installed and active in the current shell run

```
mkdir build
cd build
emcmake cmake ..
make
```

Then to open the demo in the browser run

```
cd ..
cd html
python -m SimpleHTTPServer 8080
```

and open http://localhost:8080 in your browser.

(Testet with Emscripten 1.39.12)
