#include <stdio.h>
#include <string.h>

#include "raylib.h"
#include "raymath.h"
#include "particle.h"

#define DARK CLITERAL(Color){15, 15, 15, 255}

Particle genParticle(void)
{
    int x = GetRandomValue(100 + PARTICLE_RADIUS, 700 - PARTICLE_RADIUS);
    int y = GetRandomValue(100 + PARTICLE_RADIUS, 500 - PARTICLE_RADIUS);

    Particle randParticle = {.pos_cur.x=x, .pos_cur.y=y, .pos_old.x=x, .pos_old.y=y, .color=RAYWHITE};
    return randParticle;
}

int addParticle(Particle particle, Particle *particles, int *counter, int size)
{
     if (*counter < size)
     {
        particles[*counter] = particle;
        (*counter)++;

        return 1;
     }
     else
     {
        printf("Array is full, cannot add more than %d particles\n", MAX_PARTICLES);
        return 0;
     }
}

void drawParticles(Particle* particles, int* counter)
{
    for (int i = 0; i < *counter; i++)
    {
        DrawCircle(particles[i].pos_cur.x, particles[i].pos_cur.y, PARTICLE_RADIUS, particles[i].color);
        //DrawCircleLines(particles[i].pos_cur.x, particles[i].pos_cur.y, PARTICLE_RADIUS, DARK);
    }
}

int main(void)
{   
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Particle simulator");
    MaximizeWindow();

    SetTargetFPS(144);

    Particle particles[MAX_PARTICLES] = {0};
    int frame_count = 0;
    int counter = 0;
    char title[1024];

    while(!WindowShouldClose()) 
    {
        float dt = GetFrameTime();
        frame_count++;

        if (IsKeyPressed(KEY_C))
        {
            clearScreen(particles, &counter);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            if (frame_count % 4 == 0)
            {
            Vector2 mouse_pos = GetMousePosition();
            Particle mouseParticle = {.pos_cur.x=(float)mouse_pos.x, .pos_cur.y=(float)mouse_pos.y, .pos_old.x=(float)mouse_pos.x, .pos_old.y=(float)mouse_pos.y - 1.0f, .color=RAYWHITE};
            addParticle(mouseParticle, particles, &counter, MAX_PARTICLES);
            }
        }

        update(particles, &counter, dt);

        sprintf(title, "FPS : %d | Particles : %d", GetFPS(), counter);
        SetWindowTitle(title);

        BeginDrawing();
        ClearBackground(DARK);

        DrawCircleLines(SCREENWIDTH / 2, SCREENHEIGHT / 2, CIRCLE_RADIUS, GRAY); //draw constraint
        //DrawCircle(SCREENWIDTH / 2, SCREENHEIGHT / 2, 3, RED); //center dot
        //DrawRectangleLines(100, 100, 600, 600, GRAY);
        //DrawLine(SCREENWIDTH / 2, 0, SCREENWIDTH / 2, SCREENHEIGHT, GRAY);
        //DrawLine(0, SCREENHEIGHT / 2, SCREENWIDTH, SCREENHEIGHT / 2, GRAY);

        drawParticles(particles, &counter);
        
        EndDrawing();
    }
    CloseWindow();

    return 0;
}