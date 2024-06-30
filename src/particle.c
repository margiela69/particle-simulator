#include "particle.h"

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

Vector2 gravity = {0.0f, 1000.0f};
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

void clearScreen(Particle* particles, int* count)
{
    if (*count > 0)
    {
        memset(particles, 0, *count * sizeof(particles[0]));
        *count = 0;
    }
}