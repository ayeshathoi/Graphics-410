# **`Computer-Graphics`**

The project implements the following :
- Analog Clock
- Sphere to/ From Octahedron 
![alt text](https://www.youtube.com/watch?v=nc07fEKwIBI)

# `Offline2`
Implementation of Raster based graphics pipeline.
- `Stage 1:` modeling transformation
- `Stage 2:` view transformation
- `Stage 3:` projection transformation
- `Stage 4:` clipping & scan conversion using Z-buffer algorithm

A demo of the input can be found in `Test Cases` folder.

The output is given below <br/>
![alt text](https://github.com/ayeshathoi/Graphics-410/blob/main/Rasterization/Test%20Cases%20(Updated%202%20Aug)/5/out.bmp)



## **`Implementation-of-Ray-Tracing`**

This project implements the following :
- **A fully controllable camera :** camera moves in all directions by pressing different keys.
- **Creates plane and shapes of different structure**: takes input from a file and draws spheres, triangle, quadric surface, Pyramids and a floor(2d plane).
- **Illumination with the Phong Lighting Model** : computes the ambient, specular and diffuse component of each object and light them accordingly.
- **Recursive Reflection** : reflection is computed recursively for an object according to the variable `recursion_level`.
- **Snapshot** : A snapshot can be taken pressing "0" button
- **Texture** : Texture can be added in the floor pressing "space" button. Otherwise, floor will be without texture in the snapshot.

---
- ![without texture](https://github.com/ayeshathoi/Graphics-410/blob/main/Ray%20Tracing%20Offline/without_tex.bmp)
- ![with texture](https://github.com/ayeshathoi/Graphics-410/blob/main/Ray%20Tracing%20Offline/withtex.bmp)
### Language & Library Used
- `CPP`, `OpenGL`
  

