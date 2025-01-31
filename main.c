#include "raylib.h"
#include <stdio.h>
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Juan's Particle Simulation");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Variables
    Vector2 particlesSpeed = { 2.0f, 2.0f };
    Vector2 particlesPosition = { (float)screenWidth / 2, (float)screenHeight / 2 };
    int particlesRadius = 10;
    
    struct Particle {
        Vector2 position;
        Vector2 speed;
        Vector2 mass;
        int radius;
    };

    struct Particle particles[100];

    for (int i = 0; i < 100; i++) {
        particles[i].position = (Vector2){ GetRandomValue(0, screenWidth), GetRandomValue(0, screenHeight) };
        particles[i].speed = (Vector2){ GetRandomValue(-2, 2), GetRandomValue(-2, 2) };
        particles[i].mass = (Vector2){ 1.0f, 1.0f };
        particles[i].radius = particlesRadius;
    }

    char particlesAmount = 100;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawFPS(10, 10);

            char particlesText[20];
            sprintf(particlesText, "Particles: %d", particlesAmount);
            DrawText(particlesText, 10, 30, 10, BLACK);

            // Draw particles
            for (int i = 0; i < particlesAmount; i++)
            {
                DrawCircleV(particles[i].position, particles[i].radius, RED);
            }

            // Move particles
            for (int i = 0; i < particlesAmount; i++)
            {
                particles[i].position.x += particles[i].speed.x;
                particles[i].position.y += particles[i].speed.y;

                // // Bounce particles
                // if (particles[i].position.y >= screenHeight - particles[i].radius || particles[i].position.y <= particles[i].radius)
                // {
                //     particles[i].speed.y *= -1;
                // }

                // if (particles[i].position.x >= screenWidth - particles[i].radius || particles[i].position.x <= particles[i].radius)
                // {
                //     particles[i].speed.x *= -1;
                // }

                // Handle collisions

                // Collide with walls
                if (particles[i].position.y >= screenHeight - particles[i].radius || particles[i].position.y <= particles[i].radius)
                {
                    particles[i].speed.y *= -1;
                }

                // Collide with each other
                for (int j = 0; j < particlesAmount; j++)
                {
                    if (i != j)
                    {
                        if (CheckCollisionCircles(particles[i].position, particles[i].radius, particles[j].position, particles[j].radius))
                        {
                            // Swap speeds
                            Vector2 temp = particles[i].speed;
                            particles[i].speed = particles[j].speed;
                            particles[j].speed = temp;
                        }
                    }
                }
                

                // Apply mass effect (gravity)
                particles[i].speed.y += 0.1f; // Gravity effect
            }

            // Collide with each other

        EndDrawing();
            //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

