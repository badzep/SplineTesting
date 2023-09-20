#include <raylib.h>


#include <format>


#include "Hermite.hpp"
#include "Constants.hpp"



void drag_points(HermiteSpline &spline, Camera2D &camera) {
    Vec<float> clicked_position = Vec<float>(GetScreenToWorld2D(GetMousePosition(), camera));
    for (HermitePoint &spline_point: spline.get_points()) {
        if (spline_point.position.get_distance_to(clicked_position.subtract(Vec<float>(GetMouseDelta()))) <= DRAG_DISTANCE) {
            spline_point.position.x = clicked_position.x;
            spline_point.position.y = clicked_position.y;
            return;
        }
    }
    for (HermitePoint &spline_point: spline.get_points()) {
        if (spline_point.get_velocity_end_point().get_distance_to(clicked_position.subtract(Vec<float>(GetMouseDelta()))) <= DRAG_DISTANCE) {
            spline_point.velocity = clicked_position.subtract(spline_point.position).divide(VELOCITY_DISPLAY_MULTIPLIER);
            return;
        }
    }
}


int main() {
    InitWindow((int) WINDOW.x, (int) WINDOW.y, "Hermite Spline Test");
    SetTargetFPS(200);

    HermiteSpline spline;
    Camera2D camera = {};
    
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

    camera.zoom = 1.0f;
    camera.offset = {(float) WINDOW.x / 2.0f, (float) WINDOW.y / 2.0f}; // center camera

    float time = 0;
    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_ONE)) {
            SetTargetFPS(10);
        }

        if (IsKeyPressed(KEY_TWO)) {
            SetTargetFPS(60);
        }

        if (IsKeyPressed(KEY_THREE)) {
            SetTargetFPS(150);
        }

        if (IsKeyPressed(KEY_FOUR)) {
            SetTargetFPS(10000);
        }

        float camera_speed = BASE_CAMERA_MOVEMENT_SPEED;
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            camera_speed =  SPRINT_CAMERA_MOVEMENT_SPEED;
        }
        if (IsKeyDown(KEY_W)) {
            camera.target.y -= camera_speed / (float) GetFPS();
        }

        if (IsKeyDown(KEY_A)) {
            camera.target.x -= camera_speed / (float) GetFPS();
        }

        if (IsKeyDown(KEY_S)) {
            camera.target.y += camera_speed / (float) GetFPS();
        }

        if (IsKeyDown(KEY_D)) {
            camera.target.x += camera_speed / (float) GetFPS();
        }

        if (IsKeyDown(KEY_E)) {
            camera.zoom += CAMERA_ZOOM_SPEED / (float) GetFPS();
        }

        if (IsKeyDown(KEY_Q)) {
            camera.zoom -= CAMERA_ZOOM_SPEED / (float) GetFPS();
            camera.zoom = std::max(0.01f, camera.zoom);
        }

        const Motion point = spline.get_point_at(time);
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            drag_points(spline,camera);
        }
        
        float path_length = 0;
        BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(BLACK);
                float path_time = 0;
                while (true) {
                    path_length += spline.get_position_at(path_time).get_distance_to(spline.get_position_at(path_time + PATH_DELTA));
                    DrawLineV(spline.get_position_at(path_time).to_raylib(), spline.get_position_at(path_time + PATH_DELTA).to_raylib(), PATH_COLOR);
                    path_time += PATH_DELTA;
                    if (path_time > ((float) spline.get_point_count() - 1.0f) - PATH_DELTA) {
                        break;
                    }
                }
                for (HermitePoint &spline_point: spline.get_points()) {
                    DrawLineV(spline_point.position.to_raylib(), spline_point.position.add(spline_point.velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER)).to_raylib(), BLUE);
                    DrawCircleV(spline_point.get_velocity_end_point().to_raylib(), 2.0f, PATH_COLOR);
                    DrawCircleV(spline_point.position.to_raylib(), 2.5f, PATH_COLOR);
                }

                DrawLineV(point.position.to_raylib(), point.get_velocity_end_point().to_raylib(), RED);
                DrawLineV(point.get_velocity_end_point().to_raylib(), point.get_acceleration_end_point().to_raylib(), ORANGE);
                DrawCircleV(point.position.to_raylib(), 5.0f, GREEN);
            EndMode2D();
            DrawText(std::format("Total path length {} m", path_length).c_str(), 20, 20, 20, WHITE);
        EndDrawing();

        time += TIME_DELTA;
        if (time > (float) spline.get_point_count() - 1.0f) {
            time -= ((float) spline.get_point_count() - 1.0f);
        }
    }
    return 0;
}
