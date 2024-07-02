#ifndef PARTICLE_H
#define PARTICLE_H

#include <string.h>

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

//Particle particles[MAX_PARTICLES];

void updatePositions(Particle* particles, float dt, int* count);
void accelerate(Particle* particle, Vector2 acc);
void applyGravity(Particle* particles, int* count);
void applyConstraint(Particle* particles, int* count);
void solveCollision(Particle* particles, int* count);
void applyColor(Particle* particles, int* count, float dt);
void accelerateToMouse(Particle* particles, int* count, float strength);
void update(Particle* particles, int* count, float dt);
void clearScreen(Particle* particles, int* count);

#endif
