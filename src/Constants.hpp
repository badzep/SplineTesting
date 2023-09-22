#pragma once


#include <raylib.h>
#include <string>


constexpr float BASE_DURATION_EDIT_MULTIPLIER = 0.25f;
constexpr float SPRINT_DURATION_EDIT_MULTIPLIER = BASE_DURATION_EDIT_MULTIPLIER * 10;

constexpr Vector2 WINDOW = {1000, 1000};
constexpr float VELOCITY_DISPLAY_MULTIPLIER = 0.25f;
constexpr float ACCELERATION_DISPLAY_MULTIPLIER = 0.05f;

constexpr Color PATH_COLOR = {10, 200, 100, 200};

constexpr float PATH_INDEX_DELTA = 1e-3; // time step size when drawing path

constexpr float GRAB_DISTANCE = 10.0f;

constexpr float BASE_CAMERA_MOVEMENT_SPEED = 100;
constexpr float SPRINT_CAMERA_MOVEMENT_SPEED = BASE_CAMERA_MOVEMENT_SPEED * 2.5f;
constexpr float BASE_CAMERA_ZOOM_SPEED = 4.0f;
constexpr float SPRINT_CAMERA_ZOOM_SPEED = BASE_CAMERA_ZOOM_SPEED * 5.0f;

constexpr float ROBOT_SIZE = 15.0f / 12.0f;
constexpr float GRID_SIZE = 6.0f;

constexpr float MAX_VELOCITY = 5.0f;
constexpr float MAX_ACCELERATION = 20.0f;

constexpr float ONE_FOOT = 1.0f;
constexpr float TWO_FEET = 2.0f;

constexpr float ONE_INCH = 1.0f / 12.0f;
constexpr float TWO_INCHES = 1.0f / 6.0f;