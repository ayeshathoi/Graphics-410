# **`Computer-Graphics`**

### Language & Library Used
- `CPP`, `OpenGL`
  
---

## ***`Offline1 : OpenGL`*** 
The project implements the following :
- Analog Clock
- Sphere to/ From Octahedron
  - Video Demonstration : [Youtube Link](https://www.youtube.com/watch?v=nc07fEKwIBI)
<p align=center">
  <img src="https://github.com/ayeshathoi/Graphics-410/blob/main/openGL/openGL%20offline%201/Clock.gif"
  height="400px" width="40%" alt="clock">
  <img src="https://github.com/ayeshathoi/Graphics-410/blob/main/openGL/openGL%20offline%201/octahedron.gif"
  height="400px" width="40%" alt="sphere octahedron">
</p>

---

## **`Offline2` : Raster based Graphics pipeline**
Implementation of Raster based graphics pipeline.
- `Stage 1:` modeling transformation
- `Stage 2:` view transformation
- `Stage 3:` projection transformation
- `Stage 4:` clipping & scan conversion using Z-buffer algorithm

A demo of the input can be found in `Test Cases` folder. There are 5 testcases, from which one of the output is given below <br/>
<p align=center">
  <img src="https://github.com/ayeshathoi/Graphics-410/blob/main/Rasterization/Test%20Cases%20(Updated%202%20Aug)/3/out.bmp"
  height="400px" width="40%" alt="rasterization">
</p>

---

## **`Offline3` : Implementation-of-Ray-Tracing**

---

This project implements the following :

- **A fully controllable camera :** camera moves in all directions by pressing different keys.
- **Creates plane and shapes of different structure**: takes input from a file and draws spheres, triangle, quadric surface, Pyramids and a floor(2d plane).
- **Illumination with the Phong Lighting Model** : computes the ambient, specular and diffuse component of each object and light them accordingly.
- **Recursive Reflection** : reflection is computed recursively for an object according to the variable `recursion_level`.
- **Snapshot** : A snapshot can be taken pressing "0" button
- **Texture** : Texture can be added in the floor pressing "space" button. Otherwise, floor will be without texture in the snapshot.

<img src="https://github.com/ayeshathoi/Graphics-410/blob/main/Ray%20Tracing%20Offline/withtex.bmp" height="400px" width="60%" alt="with Texture"/>



