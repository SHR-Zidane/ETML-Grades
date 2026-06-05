#include <raylib.h>

int main() {

    const int ScreenWidth = 800;
    const int ScreenHeight = 450;

    InitWindow(ScreenWidth, ScreenHeight, "Test");

    Vector2 ballPosition = { ScreenWidth / 2.0f, ScreenHeight / 2.0f };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 1.0f;
        if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 1.0f;
        if (IsKeyDown(KEY_UP)) ballPosition.y -= 1.0f;
        if (IsKeyDown(KEY_DOWN)) ballPosition.y += 1.0f;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText
        DrawCircleV(ballPosition, 50, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
