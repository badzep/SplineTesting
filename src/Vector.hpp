#pragma once


#include <cmath>
#include <raylib.h>
#include <string>
#include <format>


#include "Constants.hpp"


template<typename T> 
class Vec3 {
public:
    T x;
    T y;
    T z;

    Vec3() {    }

    Vec3(const T x, const T y, const T z): x(x), y(y), z(z) {    }

    explicit Vec3(const Vector3 vector): x(vector.x), y(vector.y), z(vector.z) {    }

    ~Vec3() = default;

    [[nodiscard]] Vec3<T> operator+(Vec3<T> const other_vector) const {
        return {this->x + other_vector.x, this->y + other_vector.y, this->z + other_vector.z};
    }
    [[nodiscard]] Vec3<T> operator+(const T other) const {
        return {this->x + other, this->y + other, this->z + other};
    }
    void operator+=(Vec3<T> const other_vector) {
        this->x += other_vector.x;
        this->y += other_vector.y;
        this->z += other_vector.z;
    }
    void operator+=(const T other) {
        this->x += other;
        this->y += other;
        this->z += other;
    }
    [[nodiscard]] Vec3<T> add(const Vec3<T> other_vector) const {
        return {this->x + other_vector.x, this->y + other_vector.y, this->z + other_vector.z};
    }
    [[nodiscard]] Vec3<T> add(const T other) const {
        return {this->x + other, this->y + other, this->z + other};
    }
    void add_in_place(const Vec3<T> other_vector) {
        this->x += other_vector.x;
        this->y += other_vector.y;
        this->z += other_vector.z;
    }
    void add_in_place(const T other) {
        this->x += other;
        this->y += other;
        this->z += other;
    }

    [[nodiscard]] Vec3<T> operator-(Vec3<T> const other_vector) const {
        return {this->x - other_vector.x, this->y - other_vector.y, this->z - other_vector.z};
    }
    [[nodiscard]] Vec3<T> operator-(const T other) const {
        return {this->x - other, this->y - other, this->z - other};
    }
    void operator-=(Vec3<T> const other_vector) {
        this->x -= other_vector.x;
        this->y -= other_vector.y;
        this->z -= other_vector.z;
    }
    void operator-=(const T other) {
        this->x -= other;
        this->y -= other;
        this->z -= other;
    }
    [[nodiscard]] Vec3<T> subtract(const Vec3<T> other_vector) const {
        return {this->x - other_vector.x, this->y - other_vector.y, this->z - other_vector.z};
    }
    [[nodiscard]] Vec3<T> subtract(const T other) const {
        return {this->x - other, this->y - other, this->z - other};
    }
    void subtract_in_place(const Vec3<T> other_vector) {
        this->x -= other_vector.x;
        this->y -= other_vector.y;
        this->z -= other_vector.z;
    }
    void subtract_in_place(const T other) {
        this->x -= other;
        this->y -= other;
        this->z -= other;
    }

    [[nodiscard]] Vec3<T> operator*(Vec3<T> const other_vector) const {
        return {this->x * other_vector.x, this->y * other_vector.y, this->z * other_vector.z};
    }
    [[nodiscard]] Vec3<T> operator*(const T other) const {
        return {this->x * other, this->y * other, this->z * other};
    }
    void operator*=(Vec3<T> const other_vector) {
        this->x *= other_vector.x;
        this->y *= other_vector.y;
        this->z *= other_vector.z;
    }
    void operator*=(const T other) {
        this->x *= other;
        this->y *= other;
        this->z *= other;
    }
    [[nodiscard]] Vec3<T> multiply(const Vec3<T> other_vector) const {
        return {this->x * other_vector.x, this->y * other_vector.y, this->z * other_vector.z};
    }
    [[nodiscard]] Vec3<T> multiply(const T other) const {
        return {this->x * other, this->y * other, this->z * other};
    }
    void multiply_in_place(const Vec3<T> other_vector) {
        this->x *= other_vector.x;
        this->y *= other_vector.y;
        this->z *= other_vector.z;
    }
    void multiply_in_place(const T other) {
        this->x *= other;
        this->y *= other;
        this->z *= other;
    }

    [[nodiscard]] Vec3<T> operator/(Vec3<T> const other_vector) const {
        return {this->x / other_vector.x, this->y / other_vector.y, this->z / other_vector.z};
    }
    [[nodiscard]] Vec3<T> operator/(const T other) const {
        return {this->x / other, this->y / other, this->z / other};
    }
    void operator/=(Vec3<T> const other_vector) {
        this->x /= other_vector.x;
        this->y /= other_vector.y;
        this->z /= other_vector.z;
    }
    void operator/=(const T other) {
        this->x /= other;
        this->y /= other;
        this->z /= other;
    }
    [[nodiscard]] Vec3<T> divide(const Vec3<T> other_vector) const {
        return {this->x / other_vector.x, this->y / other_vector.y, this->z / other_vector.z};
    }
    [[nodiscard]] Vec3<T> divide(const T other) const {
        return {this->x / other, this->y / other, this->z / other};
    }
    void divide_in_place(const Vec3<T> other_vector) {
        this->x /= other_vector.x;
        this->y /= other_vector.y;
        this->z /= other_vector.z;
    }
    void divide_in_place(const T other) {
        this->x /= other;
        this->y /= other;
        this->z /= other;
    }

    [[nodiscard]] Vec3<T> power(const Vec3<T> other_vector) const {
        return {std::pow(this->x, other_vector.x), std::pow(this->y, other_vector.y), std::pow(this->z, other_vector.z)};
    }
    [[nodiscard]] Vec3<T> power(const T other) const {
        return {std::pow(this->x, other), std::pow(this->y, other), std::pow(this->z, other)};
    }
    void power_in_place(const Vec3<T> other_vector) {
        this->x = std::pow(this->x, other_vector.x);
        this->y = std::pow(this->y, other_vector.y);
        this->z = std::pow(this->z, other_vector.z);
    }
    void power_in_place(const T other) {
        this->x = std::pow(this->x, other);
        this->y = std::pow(this->y, other);
        this->z = std::pow(this->z, other);
    }

    [[nodiscard]] Vec3<T> abs() const {
        return {std::abs(this->x), std::abs(this->y), std::abs(this->z)};
    }
    void abs_in_place() {
        this->x = std::abs(x);
        this->y = std::abs(y);
        this->z = std::abs(z);
    }

    [[nodiscard]] Vec3<T> sqrt() const {
        return {std::sqrt(this->x), std::sqrt(this->y), std::sqrt(this->z)};
    }
    void sqrt_in_place() {
        this->x = std::sqrt(x);
        this->y = std::sqrt(y);
        this->z = std::sqrt(z);
    }

    [[nodiscard]] Vec3<T> round() const {
        return {std::round(this->x), std::round(this->y)};
    }
    void round_in_place() {
        this->x = std::round(x);
        this->y = std::round(y);
    }

    T get_distance_to(Vec3<T> other_vector) const {
        return std::sqrt(std::pow(this->x - other_vector.x, 2) + std::pow(this->y - other_vector.y, 2)  + std::pow(this->z - other_vector.z, 2));
    }

    [[nodiscard]] T magnitude() const {
        return std::sqrt(this->x * this->x + this->y * this->y, this->z * this->z);
    }

    [[nodiscard]] Vec3<T> normalize() const {
        return this->divide(this->magnitude());
    }

    void normalize_in_place() {
        this->divide_in_place(this->magnitude());
    }
    
    [[nodiscard]] std::string to_string() const {
        return std::string("<") + std::to_string(this->x) + std::string(", ") + std::to_string(this->y) + ", " + std::to_string(this->z) + std::string(">");
    }
    [[nodiscard]] std::string to_string_2f() const {
        return std::format("<{0:.2f}, {1:.2f}, {2:.2f}>", this->x, this->y, this->z);
    }
    
    [[nodiscard]] Vector3 to_raylib() const {
        return {this->x, this->y, this->z};
    }
};


template<typename T> 
class Vec2 {
public:
    T x;
    T y;

    Vec2() {    }
    Vec2(const T x, const T y): x(x), y(y) {    }

    explicit Vec2(const Vector2 vector): x(vector.x), y(vector.y) {    }

    ~Vec2() = default;

    [[nodiscard]] Vec2<T> operator+(Vec2<T> const other_vector) const {
        return {this->x + other_vector.x, this->y + other_vector.y};
    }
    [[nodiscard]] Vec2<T> operator+(const T other) const {
        return {this->x + other, this->y + other};
    }
    void operator+=(Vec2<T> const other_vector) {
        this->x += other_vector.x;
        this->y += other_vector.y;
    }
    void operator+=(const T other) {
        this->x += other;
        this->y += other;
    }
    [[nodiscard]] Vec2<T> add(const Vec2<T> other_vector) const {
        return {this->x + other_vector.x, this->y + other_vector.y};
    }
    [[nodiscard]] Vec2<T> add(const T other) const {
        return {this->x + other, this->y + other};
    }
    void add_in_place(const Vec2<T> other_vector) {
        this->x += other_vector.x;
        this->y += other_vector.y;
    }
    void add_in_place(const T other) {
        this->x += other;
        this->y += other;
    }

    [[nodiscard]] Vec2<T> operator-(Vec2<T> const other_vector) const {
        return {this->x - other_vector.x, this->y - other_vector.y};
    }
    [[nodiscard]] Vec2<T> operator-(const T other) const {
        return {this->x - other, this->y - other};
    }
    void operator-=(Vec2<T> const other_vector) {
        this->x -= other_vector.x;
        this->y -= other_vector.y;
    }
    void operator-=(const T other) {
        this->x -= other;
        this->y -= other;
    }
    [[nodiscard]] Vec2<T> subtract(const Vec2<T> other_vector) const {
        return {this->x - other_vector.x, this->y - other_vector.y};
    }
    [[nodiscard]] Vec2<T> subtract(const T other) const {
        return {this->x - other, this->y - other};
    }
    void subtract_in_place(const Vec2<T> other_vector) {
        this->x -= other_vector.x;
        this->y -= other_vector.y;
    }
    void subtract_in_place(const T other) {
        this->x -= other;
        this->y -= other;
    }

    [[nodiscard]] Vec2<T> operator*(Vec2<T> const other_vector) const {
        return {this->x * other_vector.x, this->y * other_vector.y};
    }
    [[nodiscard]] Vec2<T> operator*(const T other) const {
        return {this->x * other, this->y * other};
    }
    void operator*=(Vec2<T> const other_vector) {
        this->x *= other_vector.x;
        this->y *= other_vector.y;
    }
    void operator*=(const T other) {
        this->x *= other;
        this->y *= other;
    }
    [[nodiscard]] Vec2<T> multiply(const Vec2<T> other_vector) const {
        return {this->x * other_vector.x, this->y * other_vector.y};
    }
    [[nodiscard]] Vec2<T> multiply(const T other) const {
        return {this->x * other, this->y * other};
    }
    void multiply_in_place(const Vec2<T> other_vector) {
        this->x *= other_vector.x;
        this->y *= other_vector.y;
    }
    void multiply_in_place(const T other) {
        this->x *= other;
        this->y *= other;
    }

    [[nodiscard]] Vec2<T> operator/(Vec2<T> const other_vector) const {
        return {this->x / other_vector.x, this->y / other_vector.y};
    }
    [[nodiscard]] Vec2<T> operator/(const T other) const {
        return {this->x / other, this->y / other};
    }
    void operator/=(Vec2<T> const other_vector) {
        this->x /= other_vector.x;
        this->y /= other_vector.y;
    }
    void operator/=(const T other) {
        this->x /= other;
        this->y /= other;
    }
    [[nodiscard]] Vec2<T> divide(const Vec2<T> other_vector) const {
        return {this->x / other_vector.x, this->y / other_vector.y};
    }
    [[nodiscard]] Vec2<T> divide(const T other) const {
        return {this->x / other, this->y / other};
    }
    void divide_in_place(const Vec2<T> other_vector) {
        this->x /= other_vector.x;
        this->y /= other_vector.y;
    }
    void divide_in_place(const T other) {
        this->x /= other;
        this->y /= other;
    }

    [[nodiscard]] Vec2<T> power(const Vec2<T> other_vector) const {
        return {std::pow(this->x, other_vector.x), std::pow(this->y, other_vector.y)};
    }
    [[nodiscard]] Vec2<T> power(const T other) const {
        return {std::pow(this->x, other), std::pow(this->y, other)};
    }
    void power_in_place(const Vec2<T> other_vector) {
        this->x = std::pow(this->x, other_vector.x);
        this->y = std::pow(this->y, other_vector.y);
    }
    void power_in_place(const T other) {
        this->x = std::pow(this->x, other);
        this->y = std::pow(this->y, other);
    }

    [[nodiscard]] Vec2<T> abs() const {
        return {std::abs(this->x), std::abs(this->y)};
    }
    void abs_in_place() {
        this->x = std::abs(x);
        this->y = std::abs(y);
    }

    [[nodiscard]] Vec2<T> sqrt() const {
        return {std::sqrt(this->x), std::sqrt(this->y)};
    }
    void sqrt_in_place() {
        this->x = std::sqrt(x);
        this->y = std::sqrt(y);
    }

    [[nodiscard]] Vec2<T> round() const {
        return {std::round(this->x), std::round(this->y)};
    }
    void round_in_place() {
        this->x = std::round(x);
        this->y = std::round(y);
    }

    T get_distance_to(Vec2<T> other_vector) const {
        return std::sqrt(std::pow(this->x - other_vector.x, 2) + std::pow(this->y - other_vector.y, 2));
    }

    [[nodiscard]] T magnitude() const {
        return std::sqrt(this->x * this->x + this->y * this->y);
    }

    [[nodiscard]] T atan2() const {
        return std::atan2(this->y, this->x);
    }

    [[nodiscard]] Vec2<T> normalize() const {
        return this->divide(this->magnitude());
    }

    void normalize_in_place() {
        this->divide_in_place(this->magnitude());
    }
    
    [[nodiscard]] std::string to_string() const {
        return std::string("<") + std::to_string(this->x) + std::string(", ") + std::to_string(this->y) + std::string(">");
    }
    [[nodiscard]] std::string to_string_2f() const {
        return std::format("<{0:.2f}, {1:.2f}>", this->x, this->y);
    }
    
    [[nodiscard]] Vector2 to_raylib() const {
        return {this->x, this->y};
    }

    /**
     * Also works in reverse
     */
    [[nodiscard]] Vec2<T> field_to_screen() const {
        return {-this->y, -this->x};
    }

    /**
     * Only exists for the sake of clarity
     */
    [[nodiscard]] Vec2<T> screen_to_field() const {
        return {-this->y, -this->x};
    }

    [[nodiscard]] Vec3<T> to_3d(const T z) const {
        return {this->x, this->y, z};
    }
};

inline void DrawLineField(const Vec2<float> start, const Vec2<float> end, const float thickness, const Color color) {
    DrawLineEx(start.field_to_screen().to_raylib(), end.field_to_screen().to_raylib(), thickness, color);
}

inline void DrawCircleField(const Vec2<float> position, const float radius, const Color color) {
    DrawCircleV(position.field_to_screen().to_raylib(), radius, color);
}

inline void DrawPolyField(const Vec2<float> position, const unsigned int sides, const float radius, const float rotation, const Color color) {
    DrawPoly(position.field_to_screen().to_raylib(), sides, radius, -rotation, color);
}

inline void DrawTextField(const char* text, const Vec2<float> position, const float font_size, const float spacing, const Color color) {
    DrawTextEx(font, text, position.field_to_screen().to_raylib(), font_size, spacing, color);
}
