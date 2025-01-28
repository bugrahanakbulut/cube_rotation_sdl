#include <math.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600

#define WHITE 0xFFFFFFFF

#define DEG2RAD 0.01745329251

struct Point
{
    float x;
    float y;
    float z;
};

struct Cube
{
    struct Point vertices[8];
    struct Point indices[12];
    struct Point position;
    float size;
};

int draw_point(SDL_Renderer * renderer, struct Point point)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color)
    SDL_Rect rect = { point.x - 2.5f, point.y - 2.5f, 5, 5 };
    SDL_RenderFillRect(renderer, &rect);
}

int draw_point_world(SDL_Renderer * renderer, struct Point point)
{
    struct Point worldPoint = { point.x + SCREEN_WIDTH / 2, point.y + SCREEN_HEIGHT / 2, point.z };
    draw_point(renderer, worldPoint);
}

struct Cube create_cube()
{
    return (struct Cube) {
        .vertices = {
            { -1, -1, -1 },
            {  1, -1, -1 },
            {  1,  1, -1 },
            { -1,  1, -1 },
            { -1, -1,  1 },
            {  1, -1,  1 },
            {  1,  1,  1 },
            { -1,  1,  1 }
        },
        .indices = {
            // Front face (vertices 0, 1, 2, 3)
            {0, 1, 2},  // Triangle 1
            {0, 2, 3},  // Triangle 2

            // Back face (vertices 4, 5, 6, 7)
            {4, 5, 6},  // Triangle 1
            {4, 6, 7},  // Triangle 2

            // Left face (vertices 0, 3, 7, 4)
            {0, 3, 7},  // Triangle 1
            {0, 7, 4},  // Triangle 2

            // Right face (vertices 1, 2, 6, 5)
            {1, 2, 6},  // Triangle 1
            {1, 6, 5},  // Triangle 2

            // Top face (vertices 3, 2, 6, 7)
            {3, 2, 6},  // Triangle 1
            {3, 6, 7},  // Triangle 2

            // Bottom face (vertices 0, 1, 5, 4)
            {0, 1, 5},  // Triangle 1
            {0, 5, 4}   // Triangle 2
        },
        .position = { 0, 0, 0 },
        .size = 100
    };
}

int draw_line(SDL_Renderer * renderer, struct Point start, struct Point end)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0); // White color
    SDL_RenderDrawLine(renderer, start.x + SCREEN_WIDTH / 2, start.y + SCREEN_HEIGHT / 2, end.x + SCREEN_WIDTH / 2, end.y + SCREEN_HEIGHT / 2);
}

int draw_cube(SDL_Renderer * renderer, struct Cube cube)
{
    int verticesCount = sizeof(cube.vertices) / sizeof(cube.vertices[0]);

    for (int i = 0; i < verticesCount; i++)
    {
        struct Point vertex = cube.vertices[i];
        vertex.x = vertex.x * cube.size + cube.position.x;
        vertex.y = vertex.y * cube.size + cube.position.y;
        vertex.z = vertex.z * cube.size + cube.position.z;

        draw_point_world(renderer, vertex);
    }

    int indicesCount = sizeof(cube.indices) / sizeof(cube.indices[0]);

    for (int i = 0; i < indicesCount; i++)
    {
        struct Point index = cube.indices[i];

        struct Point first = cube.vertices[(int)index.x];
        struct Point second = cube.vertices[(int)index.y];
        struct Point third = cube.vertices[(int)index.z];

        first.x = first.x * cube.size + cube.position.x;
        first.y = first.y * cube.size + cube.position.y;
        first.z = first.z * cube.size + cube.position.z;

        second.x = second.x * cube.size + cube.position.x;
        second.y = second.y * cube.size + cube.position.y;
        second.z = second.z * cube.size + cube.position.z;

        third.x = third.x * cube.size + cube.position.x;
        third.y = third.y * cube.size + cube.position.y;
        third.z = third.z * cube.size + cube.position.z;

        draw_line(renderer, first, second);
        draw_line(renderer, second, third);
        draw_line(renderer, third, first);
    }
}

int rotate_cube(struct Cube * cube, float angleX, float angleY, float angleZ)
{
    int verticesCount = sizeof(cube->vertices) / sizeof(cube->vertices[0]);

    for (int i = 0; i < verticesCount; i++)
    {
        struct Point vertex = cube->vertices[i];

        float x = vertex.x;
        float y = vertex.y;
        float z = vertex.z;

        float cosX = cos(angleX * DEG2RAD), sinX = sin(angleX * DEG2RAD);
        float cosY = cos(angleY * DEG2RAD), sinY = sin(angleY * DEG2RAD);
        float cosZ = cos(angleZ * DEG2RAD), sinZ = sin(angleZ * DEG2RAD);

        // Rotate around X axis
        float x1 = x;
        float y1 = y * cosX - z * sinX;
        float z1 = y * sinX + z * cosX;

        // Rotate around Y axis
        float x2 = x1 * cosY + z1 * sinY;
        float y2 = y1;
        float z2 = -x1 * sinY + z1 * cosY;

        // Rotate around Z axis
        float x3 = x2 * cosZ - y2 * sinZ;
        float y3 = x2 * sinZ + y2 * cosZ;
        float z3 = z2;

        cube->vertices[i] = (struct Point) { x3, y3, z3 };
    }
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow("CubeRotation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    struct Cube cube = create_cube();

    int keep_running = 1;
    SDL_Event event;

    rotate_cube(&cube, 0, 30, 0);


    while (keep_running)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                keep_running = 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        keep_running = 0;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color
        SDL_RenderClear(renderer);

        rotate_cube(&cube, 0.5, 0.1, 0);

        draw_cube(renderer, cube);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
