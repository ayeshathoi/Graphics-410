# **`Computer-Graphics`**

### Language & Library Used
- `CPP`, `OpenGL`
  
---

## ***`Offline1 : OpenGL`*** 

<p align="center">
  <img src="https://github.com/ayeshathoi/Graphics-410/blob/main/openGL/openGL%20offline%201/Clock.gif"
  height="400px" width="40%" alt="clock">
  <img src="https://github.com/ayeshathoi/Graphics-410/blob/main/openGL/openGL%20offline%201/octahedron.gif"
  height="400px" width="40%" alt="sphere octahedron">
</p>

---


### Offline 
- Analog Clock
- Sphere to/ From Octahedron

---

<p align="center">
  <img src="https://github.com/ayeshathoi/Graphics-410/blob/main/openGL/online1/cube1.gif"
  height="400px" width="40%" alt="clock">
  <img src="https://github.com/ayeshathoi/Graphics-410/blob/main/openGL/online1/sphere1.gif"
  height="400px" width="40%" alt="sphere octahedron">
</p>

---

### Online
- Cube motion in a floor 
- Sphere motion in a floor 
- Half Octahedron


---

## **`Offline2 : Raster based Graphics pipeline`**

<p align="center">
  <img src="https://github.com/ayeshathoi/Graphics-410/blob/main/Rasterization/Test%20Cases%20(Updated%202%20Aug)/3/out.bmp"
  height="400px" width="40%" alt="rasterization">
</p>

---

Implementation of Raster based graphics pipeline.
- `Stage 1:` modeling transformation
- `Stage 2:` view transformation
- `Stage 3:` projection transformation
- `Stage 4:` clipping & scan conversion using Z-buffer algorithm

A demo of the input can be found in `Test Cases` folder. There are 5 testcases, from which one of the output is given above. <br/>


---

## **`Offline3 : Implementation-of-Ray-Tracing`**

<p align="center">
<img src="https://github.com/ayeshathoi/Graphics-410/blob/main/Ray%20Tracing%20Offline/withtex.bmp" height="50%" width="50%" alt="ray tracing" />
</p>

---

This project implements the following :

- **A fully controllable camera :** camera moves in all directions by pressing different keys.
- **Creates plane and shapes of different structure**: takes input from a file and draws spheres, triangle, quadric surface, Pyramids and a floor(2d plane).
- **Illumination with the Phong Lighting Model** : computes the ambient, specular and diffuse component of each object and light them accordingly.
- **Recursive Reflection** : reflection is computed recursively for an object according to the variable `recursion_level`.
- **Snapshot** : A snapshot can be taken pressing "0" button
- **Texture** : Texture can be added in the floor pressing "space" button. Otherwise, floor will be without texture in the snapshot.



