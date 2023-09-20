#include <raylib.h>


#include "Hermite.hpp"


constexpr Vector2 WINDOW = {1000, 1000};
constexpr float VELOCITY_DISPLAY_MULTIPLIER = 0.1f;
constexpr float ACCELERATION_DISPLAY_MULTIPLIER = 0.01f;

constexpr Color PATH_COLOR = {10, 100, 200, 200};

constexpr float TIME_DELTA = 1e-2; // time step size when moving across spline
constexpr float PATH_DELTA = 1e-2; // time step size when drawing path

int main() {
    InitWindow((int) WINDOW.x, (int) WINDOW.y, "Hermite Spline Test");
    SetTargetFPS(60);

    HermiteSpline spline;
    
    spline.add_point({{-300, 300},
                      {0,    -2000}});
    spline.add_point({{-150, -100},
                      {0,    500}});
    spline.add_point({{100,  -100},
                      {-300, 100}});
    spline.add_point({{200, -100},
                      {0,   500}});
    spline.add_point({{0, 0},
                      {0, 0}});

    Camera2D camera = {};
    camera.zoom = 1.0f;
    camera.offset = {(float) WINDOW.x / 2.0f, (float) WINDOW.y / 2.0f}; // center camera

    float time = 0;
    while (!WindowShouldClose()) {
        const Motion point = spline.get_point_at(time);
        BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(BLACK);
                float path_time = 0;
                while (true) {
                    DrawLineV(spline.get_position_at(path_time).to_raylib(), spline.get_position_at(path_time + PATH_DELTA).to_raylib(), PATH_COLOR);
                    path_time += PATH_DELTA;
                    if (path_time > ((float) spline.get_point_count() - 1.0f) - PATH_DELTA) {
                        break;
                    }
                }
                for (HermitePoint &spline_point: spline.get_points()) {
                    DrawLineV(spline_point.position.to_raylib(), spline_point.position.add(spline_point.velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER)).to_raylib(), BLUE);
                    DrawCircleV(spline_point.position.to_raylib(), 2.5f, PATH_COLOR);
                }
                DrawLineV(point.position.to_raylib(), point.position.add(point.velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER)).to_raylib(), RED);
                DrawLineV(point.position.add(point.velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER)).to_raylib(), point.position.add(point.velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER)).add(point.acceleration.multiply(ACCELERATION_DISPLAY_MULTIPLIER)).to_raylib(), ORANGE);
                DrawCircleV(point.position.to_raylib(), 5.0f, GREEN);

            EndMode2D();
        EndDrawing();

        time += TIME_DELTA;
        if (time > (float) spline.get_point_count() - 1.0f) {
            time -= ((float) spline.get_point_count() - 1.0f);
        }
    }
    return 0;
}
