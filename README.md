# Particle Simulator using verlet integration
A small 2D particle physics simulator written in C using [verlet integration](https://en.wikipedia.org/wiki/Verlet_integration "Wikipedia - Verlet Integration") to calculate motion updates.
![](https://media1.giphy.com/media/v1.Y2lkPTc5MGI3NjExcHFnODE2bzk5d2xnanZpaGcxYmw2Zm5rZXlocGhtN25qZW9tdGJoZyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/sF8HZyjNekqF8MLzRG/giphy.gif)
Quite therapeutic to play around with

# Dependencies
This project depends on [raylib](https://github.com/raysan5/raylib) for graphics rendering.

# Compilation
``gcc main.c -Wextra -Wall particle.c -lraylib -lm``

# Controls
* Spawn particles by pressing/holding *LMB*
* Accelerate particles to cursor by holding *SPACE*
* Remove all particles by pressing *C*
* Close window by pressing *ESC*
