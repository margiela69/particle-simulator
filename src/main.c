#include <stdio.h>
#include <stdlib.h>
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

void resizeParticles(particles_da *particles)
{
    if (particles->capacity == 0) particles->capacity = 4;
    else particles->capacity *= 2;
    
    particles->elements = realloc(particles->elements, particles->capacity * sizeof(Particle));
    printf("allocating addition memory for array: new capacity = %zu\n", particles->capacity);
    if (particles->elements == NULL) {
        printf("An error occured when resizing particle array... Exiting\n");
        exit(EXIT_FAILURE);
    }
}

void addParticle(Particle particle, particles_da *particles)
{
    if (particles->capacity <= particles->count) {
        resizeParticles(particles);
    }
    
    particles->elements[particles->count++] = particle;
}

void drawParticles(particles_da *particles) //HERE
{
    for (int i = 0; i < particles->count; i++)
    {
        DrawCircle(particles->elements[i].pos_cur.x, particles->elements[i].pos_cur.y, PARTICLE_RADIUS, particles->elements[i].color);
        //DrawCircleLines(particles[i].pos_cur.x, particles[i].pos_cur.y, PARTICLE_RADIUS, DARK);
    }
}

int main(void)
{   
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Particle simulator");
    MaximizeWindow();

    SetTargetFPS(144);

    particles_da *particles = calloc(1, sizeof(particles_da));
    
    int frame_count = 0;
    char title[1024];

    while(!WindowShouldClose()) 
    {
        float dt = GetFrameTime();
        frame_count++;

        if (IsKeyPressed(KEY_C))
        {
            clearScreen(particles);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            if (frame_count % 4 == 0)
            {
            Vector2 mouse_pos = GetMousePosition();
            Particle mouseParticle = {.pos_cur.x=(float)mouse_pos.x, .pos_cur.y=(float)mouse_pos.y, .pos_old.x=(float)mouse_pos.x, .pos_old.y=(float)mouse_pos.y - 1.0f, .color=RAYWHITE};
            addParticle(mouseParticle, particles);
            }
        }

        update(particles, dt);

        sprintf(title, "FPS : %d | Particles : %d", GetFPS(), particles->count);
        SetWindowTitle(title);

        BeginDrawing();
        ClearBackground(DARK);

        DrawCircleLines(SCREENWIDTH / 2, SCREENHEIGHT / 2, CIRCLE_RADIUS, GRAY); //draw constraint
        //DrawCircle(SCREENWIDTH / 2, SCREENHEIGHT / 2, 3, RED); //center dot
        //DrawRectangleLines(100, 100, 600, 600, GRAY);
        //DrawLine(SCREENWIDTH / 2, 0, SCREENWIDTH / 2, SCREENHEIGHT, GRAY);
        //DrawLine(0, SCREENHEIGHT / 2, SCREENWIDTH, SCREENHEIGHT / 2, GRAY);

        drawParticles(particles); // HERE
        
        EndDrawing();
    }
    CloseWindow();
    free_particles(particles);
    return 0;
}
