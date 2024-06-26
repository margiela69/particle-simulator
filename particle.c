#include <stdio.h>
#include <string.h>

#include "raylib.h"
#include "raymath.h"

#define DARK CLITERAL(Color){15, 15, 15, 255}
#define SCREENWIDTH 800
#define SCREENHEIGHT 800

#define MAX_PARTICLES 100
#define PARTICLE_RADIUS 10

typedef struct Particle
{
    Vector2 pos_cur;
    Vector2 pos_old;
    Vector2 acceleration;
    Color color;
} Particle;

Vector2 gravity = {0.0f, 1000.0f};

void updatePosition(Particle* particle, float dt)
{
    Vector2 velocity = {(particle->pos_cur.x - particle->pos_old.x), (particle->pos_cur.y - particle->pos_old.y)};

    particle->pos_old = particle->pos_cur;

    // pos_cur = pos_cur + velocity + acceleration * dt * dt
    particle->pos_cur.x += velocity.x + (particle->acceleration.x * dt * dt);
    particle->pos_cur.y += velocity.y + (particle->acceleration.y * dt * dt);

    particle->acceleration.x = 0;
    particle->acceleration.y = 0;
}

void accelerate(Particle* particle, Vector2 acc)
{
    particle->acceleration.x += acc.x;
    particle->acceleration.y += acc.y;
}

void applyConstraint(Particle* particle)
{
    Vector2 center = {400.0f, 400.0f};
    float circle_radius = 375.0f;

    Vector2 to_center = Vector2Subtract(particle->pos_cur, center);
    float distance = Vector2Length(to_center);

    if (distance > circle_radius - PARTICLE_RADIUS)
    {
        Vector2 n = {(to_center.x / distance), (to_center.y / distance)};
        particle->pos_cur.x = center.x + n.x * (circle_radius - PARTICLE_RADIUS);
        particle->pos_cur.y = center.y + n.y * (circle_radius - PARTICLE_RADIUS);
    }
}

void solveCollision(Particle* particles, int* count)
{
    for (int i = 0; i < *count; ++i)
    {
        Particle* particle1 = &particles[i];

        for (int j = i+1; j < *count; ++j)
        {
            Particle* particle2 = &particles[j];
            Vector2 collision_axis = Vector2Subtract(particle1->pos_cur, particle2->pos_cur);
            float distance = Vector2Length(collision_axis);

            if (distance < (float)PARTICLE_RADIUS)
            {
                float delta = (float)PARTICLE_RADIUS * 2 - distance;
                //Vector2 n = {((collision_axis.x / distance) * ((float)PARTICLE_RADIUS / 100) * delta), ((collision_axis.y / distance) * ((float)PARTICLE_RADIUS / 100) * delta)};
                Vector2 n = {((collision_axis.x / distance) * 0.05f * delta), ((collision_axis.y / distance) * 0.05f * delta)};

                particle1->pos_cur.x += n.x;
                particle1->pos_cur.y += n.y;

                particle2->pos_cur.x -= n.x;
                particle2->pos_cur.y -= n.y;
            }
        }
    }
}

Particle particles[MAX_PARTICLES] = {0};

Particle genParticle(void)
{
    int x = GetRandomValue(100 + PARTICLE_RADIUS, 700 - PARTICLE_RADIUS);
    int y = GetRandomValue(100 + PARTICLE_RADIUS, 500 - PARTICLE_RADIUS);

    //int min = 0;
    //int max = 255;

    //Color randColor = {GetRandomValue(min, max), GetRandomValue(min, max), GetRandomValue(min, max), 255};

    Particle randParticle = {.pos_cur.x=x, .pos_cur.y=y, .pos_old.x=x, .pos_old.y=y, .color=RAYWHITE};
    return randParticle;
}

int addParticle(Particle particle, Particle *particles, int *counter, int size)
{
     if (*counter < size)
     {
        particles[*counter] = particle;
        (*counter)++;
        printf("Succesfully added particle to array!\n");
        return 1;
     }
     else
     {
        printf("Array is full, cannot add more particles\n");
        return 0;
     }
}

void drawParticles(Particle *particles, int *counter)
{
    for (int i = 0; i < *counter; i++)
    {
        DrawCircle(particles[i].pos_cur.x, particles[i].pos_cur.y, PARTICLE_RADIUS, particles[i].color);
        DrawCircleLines(particles[i].pos_cur.x, particles[i].pos_cur.y, PARTICLE_RADIUS, DARK);
    }
}

void clearScreen(Particle* particles, int* count)
{
    if (*count > 0)
    {
        memset(particles, 0, *count * sizeof(particles[0]));
        *count = 0;
    }
}

int main(void)
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Hi from Raylib");

    SetTargetFPS(144);

    int counter = 0;
    char counter_string[1024]; // buffer to hold string of counter

    while(!WindowShouldClose()) 
    {
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_C))
        {
            clearScreen(particles, &counter);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mouse_pos = GetMousePosition();
            Particle mouseParticle = {.pos_cur.x=(float)mouse_pos.x, .pos_cur.y=(float)mouse_pos.y, .pos_old.x=(float)mouse_pos.x, .pos_old.y=(float)mouse_pos.y, .color=RAYWHITE};
            addParticle(mouseParticle, particles, &counter, MAX_PARTICLES);
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            Particle randParticle = genParticle();

            addParticle(randParticle, particles, &counter, MAX_PARTICLES);
        }
        
        for (int i = 0; i < counter; i++) //update
        {
                accelerate(&particles[i], gravity);
                applyConstraint(&particles[i]);
                solveCollision(particles, &counter);
                updatePosition(&particles[i], dt);
        }

        BeginDrawing();
        ClearBackground(DARK);

        DrawCircleLines(SCREENWIDTH / 2, SCREENHEIGHT / 2, 375, GRAY);
        DrawCircle(SCREENWIDTH / 2, SCREENHEIGHT / 2, 3, RED);
        //DrawRectangleLines(100, 100, 600, 600, GRAY);
        //DrawLine(SCREENWIDTH / 2, 0, SCREENWIDTH / 2, SCREENHEIGHT, GRAY);
        //DrawLine(0, SCREENHEIGHT / 2, SCREENWIDTH, SCREENHEIGHT / 2, GRAY);

        drawParticles(particles, &counter);

        sprintf(counter_string, "Particle objects: %d", counter);
        DrawText(counter_string, 10, 10, 20, GRAY); // draw amount of particles
        DrawFPS(10, 30);
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}