#pragma once


#include "Spline.hpp"
#include "Vec.hpp"


static bool paused = false;
static float current_time = 0;
static Font font;

inline void DrawLineField(const Vec2f start, const Vec2f end, const float thickness, const Color color) {
    DrawLineEx(start.field_to_screen().to_raylib(), end.field_to_screen().to_raylib(), thickness, color);
}

inline void DrawCircleField(const Vec2f position, const float radius, const Color color) {
    DrawCircleV(position.field_to_screen().to_raylib(), radius, color);
}

inline void DrawPolyField(const Vec2f position, const int sides, const float radius, const float rotation, const Color color) {
    DrawPoly(position.field_to_screen().to_raylib(), sides, radius, -rotation, color);
}

inline void DrawTextField(const char* text, const Vec2f position, const float font_size, const float spacing, const Color color) {
    DrawTextEx(font, text, position.field_to_screen().to_raylib(), font_size, spacing, color);
}
