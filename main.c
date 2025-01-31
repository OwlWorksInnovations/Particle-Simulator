#include "raylib.h"
#include "raymath.h" // For Vector2Normalize and other math utilities
#include <stdio.h>
#include <math.h>    // For sqrt()

// Particle structure
struct Particle {
    Vector2 position;
    Vector2 speed;
    float mass;
    int radius;
};

void HandleCollision(struct Particle *p1, struct Particle *p2) {
    // Calculate collision normal (direction vector between centers)
    Vector2 collisionNormal = Vector2Normalize((Vector2){
        p2->position.x - p1->position.x,
        p2->position.y - p1->position.y
    });

    // Calculate relative velocity
    Vector2 relativeVelocity = (Vector2){
        p2->speed.x - p1->speed.x,
        p2->speed.y - p1->speed.y
    };

    // Calculate the velocity along the collision normal (dot product)
    float velocityAlongNormal = Vector2DotProduct(relativeVelocity, collisionNormal);

    // If particles are moving away from each other, no collision response is needed
    if (velocityAlongNormal > 0) return;

    // Calculate impulse (magnitude of the force applied during collision)
    float impulse = (-2.0f * velocityAlongNormal) / (p1->mass + p2->mass);

    // Apply impulse to update speeds
    p1->speed.x -= impulse * p2->mass * collisionNormal.x;
    p1->speed.y -= impulse * p2->mass * collisionNormal.y;

    p2->speed.x += impulse * p1->mass * collisionNormal.x;
    p2->speed.y += impulse * p1->mass * collisionNormal.y;

    // Correct positions to prevent overlap
    float overlap = (p1->radius + p2->radius) - Vector2Distance(p1->position, p2->position);
    if (overlap > 0) {
        Vector2 correction = Vector2Scale(collisionNormal, overlap / 2.0f);
        p1->position = Vector2Subtract(p1->position, correction);
        p2->position = Vector2Add(p2->position, correction);
    }
}

int main(void) {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Juan's Particle Simulation");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Variables
    int particlesRadius = 10;
    float gravity = 0.98f; // Gravity as a float

    struct Particle particles[100];

    for (int i = 0; i < 100; i++) {
        particles[i].position = (Vector2){ GetRandomValue(0, screenWidth), GetRandomValue(0, screenHeight) };
        particles[i].speed = (Vector2){ GetRandomValue(-2, 2), GetRandomValue(-2, 2) };
        particles[i].mass = 1.0f; // Mass as a float
        particles[i].radius = particlesRadius;
    }

    int particlesAmount = 100;

    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        char particlesText[20];
        sprintf(particlesText, "Particles: %d", particlesAmount);

        // Move particles and handle collisions
        for (int i = 0; i < particlesAmount; i++) {
            float maxSpeed = 5.0f; // Define a maximum speed limit

            for (int i = 0; i < particlesAmount; i++) {
                // Limit speed to prevent particles from moving too fast
                particles[i].speed.x = fminf(fmaxf(particles[i].speed.x, -maxSpeed), maxSpeed);
                particles[i].speed.y = fminf(fmaxf(particles[i].speed.y, -maxSpeed), maxSpeed);
            }
            // Apply gravity
            particles[i].speed.y += gravity * particles[i].mass;

            // Update position
            particles[i].position.x += particles[i].speed.x;
            particles[i].position.y += particles[i].speed.y;

            // Bounce particles off walls
            if (particles[i].position.y >= screenHeight - particles[i].radius) {
                particles[i].position.y = screenHeight - particles[i].radius; // Correct position
                particles[i].speed.y *= -1; // Reverse speed
            }
            if (particles[i].position.y <= particles[i].radius) {
                particles[i].position.y = particles[i].radius; // Correct position
                particles[i].speed.y *= -1; // Reverse speed
            }
            if (particles[i].position.x >= screenWidth - particles[i].radius) {
                particles[i].position.x = screenWidth - particles[i].radius; // Correct position
                particles[i].speed.x *= -1; // Reverse speed
            }
            if (particles[i].position.x <= particles[i].radius) {
                particles[i].position.x = particles[i].radius; // Correct position
                particles[i].speed.x *= -1; // Reverse speed
            }

            // Handle collisions between particles
            for (int j = i + 1; j < particlesAmount; j++) {
                if (CheckCollisionCircles(particles[i].position, particles[i].radius, particles[j].position, particles[j].radius)) {
                    HandleCollision(&particles[i], &particles[j]);
                }
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawFPS(10, 10);
            DrawText(particlesText, 10, 30, 10, BLACK);

            // Draw particles
            for (int i = 0; i < particlesAmount; i++) {
                DrawCircleV(particles[i].position, particles[i].radius, RED);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}