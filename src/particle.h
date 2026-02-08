#ifndef PARTICLE_H
#define PARTICLE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#define SCREENWIDTH 1280
#define SCREENHEIGHT 720

#define CIRCLE_RADIUS 300
#define MAX_PARTICLES 2500
#define PARTICLE_RADIUS 5

typedef struct Particle
{
    Vector2 pos_cur;
    Vector2 pos_old;
    Vector2 acceleration;
    Color color;
} Particle;

typedef struct {
    Particle *elements;
    size_t count;
    size_t capacity;
} particles_da;

//Particle particles[MAX_PARTICLES];

void updatePositions(particles_da *particles, float dt);
void accelerate(Particle *particle, Vector2 acc);
void applyGravity(particles_da *particles);
void applyConstraint(particles_da *particles);
void solveCollision(particles_da *particles);
void applyColor(particles_da *particles, float dt);
void accelerateToMouse(particles_da *particles, float strength);
void update(particles_da *particles, float dt);
void clearScreen(particles_da *particles);
void free_particles(particles_da *particles);

#endif
