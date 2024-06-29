#include <stdio.h>
#include <string.h>

#include "raylib.h"
#include "raymath.h"

#define DARK CLITERAL(Color){15, 15, 15, 255}
#define SCREENWIDTH 1920
#define SCREENHEIGHT 1080
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

typedef struct Link
{
    Particle particle_1;
    Particle particle_2;
    float target_dist;
} Link;

Vector2 gravity = {0.0f, 1000.0f};

void updatePositions(Particle* particles, float dt, int* count)
{
    for (int i = 0; i < *count; i++)
    {
        Particle* particle = &(particles[i]);

        Vector2 velocity = {(particle->pos_cur.x - particle->pos_old.x), (particle->pos_cur.y - particle->pos_old.y)};

        particle->pos_old = particle->pos_cur;

        // pos_cur = pos_cur + velocity + acceleration * dt * dt
        particle->pos_cur.x += velocity.x + (particle->acceleration.x * dt * dt);
        particle->pos_cur.y += velocity.y + (particle->acceleration.y * dt * dt);

        particle->acceleration.x = 0;
        particle->acceleration.y = 0;
    }
}

void accelerate(Particle* particle, Vector2 acc)
{
    particle->acceleration.x += acc.x;
    particle->acceleration.y += acc.y;
}

void applyGravity(Particle* particles, int* count)
{
    for (int i = 0; i < *count; i++)
    {
        accelerate(&(particles[i]), gravity);
    }
}

void applyConstraint(Particle* particles, int* count)
{
    for (int i = 0; i < *count; i++)
    {
        Particle* particle = &(particles[i]);

        Vector2 center = {SCREENWIDTH / 2, SCREENHEIGHT / 2};
        float circle_radius = CIRCLE_RADIUS;

        Vector2 to_center = Vector2Subtract(particle->pos_cur, center);
        float distance = Vector2Length(to_center);

        if (distance > circle_radius - PARTICLE_RADIUS)
        {
            Vector2 n = {(to_center.x / distance), (to_center.y / distance)};
            particle->pos_cur.x = center.x + n.x * (circle_radius - PARTICLE_RADIUS);
            particle->pos_cur.y = center.y + n.y * (circle_radius - PARTICLE_RADIUS);
        }
    }
}

void solveCollision(Particle* particles, int* count)
{
    for (int i = 0; i < *count; ++i)
    {
        Particle* particle1 = &(particles[i]);

        for (int j = i+1; j < *count; ++j)
        {
            Particle* particle2 = &particles[j];
            Vector2 collision_axis = Vector2Subtract(particle1->pos_cur, particle2->pos_cur);
            float distance = Vector2Length(collision_axis);

            if (distance < (float)PARTICLE_RADIUS * 2)
            {
                float delta = ((float)PARTICLE_RADIUS * 2) - distance;

                Vector2 n = {((collision_axis.x / distance) * 0.5f * delta), ((collision_axis.y / distance) * 0.5f * delta)};

                particle1->pos_cur.x += n.x;
                particle1->pos_cur.y += n.y;
                particle2->pos_cur.x -= n.x;
                particle2->pos_cur.y -= n.y;
            }
        }
    }
}

void applyColor(Particle* particles, int* count, float dt)
{
    for (int i = 0; i < *count; i++)
    {
        Particle* particle = &particles[i];
        Vector2 vel = {(particle->pos_cur.x - particle->pos_old.x), (particle->pos_cur.y - particle->pos_old.y)};

        float speed = fabs(sqrt(vel.x * vel.x + vel.y * vel.y));
        float color = (speed / dt) * 0.2f;
        if (color > 255)
        {
            color = 255;
        }
        Color speed_col = {255, 255 - color, 255 - color, 255};
        particle->color = speed_col;
    }
}

void accelerateToMouse(Particle* particles, int* count, float strength);

void update(Particle* particles, int* count, float dt)
{
    int sub_steps = 8;
    float sub_dt = dt / (float)sub_steps;

    for (int i = 0; i < sub_steps; i++)
    {
        applyGravity(particles, count);

        if (IsKeyDown(KEY_LEFT_CONTROL))
        {
            accelerateToMouse(particles, count, 3000); 
        }

        applyConstraint(particles, count);
        solveCollision(particles, count);
        applyColor(particles, count, sub_dt);
        updatePositions(particles, sub_dt, count);
    }
}

Particle particles[MAX_PARTICLES] = {0};

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
        printf("Array is full, cannot add more particles\n");
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

void accelerateToMouse(Particle* particles, int* count, float strength)
{
    for (int i = 0; i < *count; i++)
    {
        Vector2 mouse_pos = GetMousePosition();
        Particle* cur = &particles[i];
        Vector2 disp = Vector2Subtract(mouse_pos, cur->pos_cur);
        float distance = Vector2Length(disp);
        if (distance < 0.0001)
        {
            continue;
        }
            
        cur->acceleration.x += disp.x * strength / distance;
        cur->acceleration.y += disp.y * strength / distance;
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
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Particle simulator");
    MaximizeWindow();

    SetTargetFPS(144);

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