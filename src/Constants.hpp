#pragma once


#include "Vec.hpp"
#include <raylib.h>
#include <string>


constexpr static inline float mm_to_inches(const float mm) {
	return mm * 0.0393696f;
}

constexpr const static inline Vector2 WINDOW = {1000, 1000};
constexpr const static inline float DURATION_DISPLAY_MULTIPLIER = 4.0f;
constexpr const static inline float VELOCITY_DISPLAY_MULTIPLIER = 0.25f;
constexpr const static inline float ACCELERATION_DISPLAY_MULTIPLIER = 0.1f;

constexpr const static inline Color PATH_COLOR = {10, 200, 100, 200};

constexpr const static inline float PATH_INDEX_DELTA = 1e-3; // time step size when drawing path

constexpr const static inline float GRAB_DISTANCE = 15.0f;


constexpr const static inline float GRID_SIZE = 6.0f;

constexpr const static inline float MAX_VELOCITY = 60.0f;
constexpr const static inline float MAX_ACCELERATION = 240.0f;

constexpr const static inline float ONE_FOOT = 1.0f * 12.0f;
constexpr const static inline float TWO_FEET = 24.0f;

constexpr const static inline float ONE_INCH = 1.0f;
constexpr const static inline float TWO_INCHES = 1.0f;

constexpr const static inline float NODE_SIZE = 0.07f * 12.0f;
constexpr const static inline float PATH_HEIGHT = 0.1f * 12.0f;
constexpr const static inline float VELOCITY_NODE_HEIGHT = PATH_HEIGHT + NODE_SIZE;

constexpr const static inline float ROBOT_SIZE = 15.0f;
constexpr const static inline float ROBOT_HEIGHT = 8.0f;
constexpr const static inline float ROBOT_NODE_HEIGHT = ROBOT_HEIGHT + NODE_SIZE;

constexpr const static inline float WALL_HEIGHT = mm_to_inches(300.0f);
constexpr const static inline float GOAL_ELEVATION = 6.0f;

constexpr const static inline float NET_SPACING_X = 4.0f / 20.0f;
const float NET_SPACING_Y = std::hypot(24.0f, WALL_HEIGHT - (6.0f)) / (120.0f) * 12.0f;
constexpr const static inline Vec2<unsigned char> NET_SIZE = {20, 10};
const static float NET_ANGLE = std::atan((WALL_HEIGHT - 6.0f) / (24.0f));
constexpr const static inline Color ROBOT_COLOR = {200, 10, 200, 175};
