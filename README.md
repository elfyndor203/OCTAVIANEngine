# OCTAVIAN (WIP, README WIP)
OCTAVIAN is a WIP 2D game engine written in C. It uses a highly modular handle-based ECS design, allowing for individual system customization or overhauling. A default set of world, rendering, physics, and input modules are included. 

Currently working on: Physics

A simple example game is available here: https://github.com/elfyndor203/OCTAVIANEngine-Examples.git

## Dependencies
- OpenGL 3.3
- GLAD
- GLFW
- stb_image
- Box2D 3.1.0

## Architecture 
Systems (renderer, physics, etc.) are dependent on the OCTAVIANEngine Core only. 
Systems should not be compile-time dependencies of each other. 
A system's public data is stored in Entity Contexts. Systems may share data with each other by declaring and requesting specific fields.

At engine init, systems are wired together and told how to access each request field. 




