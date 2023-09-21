#pragma once

#include <cmath>
#include <raylib.h>
#include <string>
#include <format>

template<typename T> class Vec {
public:
    T x;
    T y;

    Vec(const T x, const T y): x(x), y(y) {    }

    explicit Vec(const Vector2 vector): x(vector.x), y(vector.y) {    }

    ~Vec() = default;

    [[nodiscard]] Vec<T> add(const Vec<T> other_vector) const {
        return {this->x + other_vector.x, this->y + other_vector.y};
    }
    [[nodiscard]] Vec<T> add(const T other) const {
        return {this->x + other, this->y + other};
    }
    void add_in_place(const Vec<T> other_vector) {
        this->x += other_vector.x;
        this->y += other_vector.y;
    }
    void add_in_place(const T other) {
        this->x += other;
        this->y += other;
    }

    [[nodiscard]] Vec<T> subtract(const Vec<T> other_vector) const {
        return {this->x - other_vector.x, this->y - other_vector.y};
    }
    [[nodiscard]] Vec<T> subtract(const T other) const {
        return {this->x - other, this->y - other};
    }
    void subtract_in_place(const Vec<T> other_vector) {
        this->x -= other_vector.x;
        this->y -= other_vector.y;
    }
    void subtract_in_place(const T other) {
        this->x -= other;
        this->y -= other;
    }

    [[nodiscard]] Vec<T> multiply(const Vec<T> other_vector) const {
        return {this->x * other_vector.x, this->y * other_vector.y};
    }
    [[nodiscard]] Vec<T> multiply(const T other) const {
        return {this->x * other, this->y * other};
    }
    void multiply_in_place(const Vec<T> other_vector) {
        this->x *= other_vector.x;
        this->y *= other_vector.y;
    }
    void multiply_in_place(const T other) {
        this->x *= other;
        this->y *= other;
    }

    [[nodiscard]] Vec<T> divide(const Vec<T> other_vector) const {
        return {this->x / other_vector.x, this->y / other_vector.y};
    }
    [[nodiscard]] Vec<T> divide(const T other) const {
        return {this->x / other, this->y / other};
    }
    void divide_in_place(const Vec<T> other_vector) {
        this->x /= other_vector.x;
        this->y /= other_vector.y;
    }
    void divide_in_place(const T other) {
        this->x /= other;
        this->y /= other;
    }

    [[nodiscard]] Vec<T> power(const Vec<T> other_vector) const {
        return {std::pow(this->x, other_vector.x), std::pow(this->y, other_vector.y)};
    }
    [[nodiscard]] Vec<T> power(const T other) const {
        return {std::pow(this->x), std::pow(this->y)};
    }
    void power_in_place(const Vec<T> other_vector) {
        this->x = std::pow(this->x, other_vector.x);
        this->y = std::pow(this->y, other_vector.y);
    }
    void power_in_place(const T other) {
        this->x = std::pow(this->x, other);
        this->y = std::pow(this->y, other);
    }

    [[nodiscard]] Vec<T> abs() const {
        return {std::abs(this->x), std::abs(this->y)};
    }
    void abs_in_place() {
        this->x = std::abs(x);
        this->y = std::abs(y);
    }

    [[nodiscard]] Vec<T> sqrt() const {
        return {std::sqrt(this->x), std::sqrt(this->y)};
    }
    void sqrt_in_place() {
        this->x = std::sqrt(x);
        this->y = std::sqrt(y);
    }

    [[nodiscard]] Vec<T> round() const {
        return {std::round(this->x), std::round(this->y)};
    }
    void round_in_place() {
        this->x = std::round(x);
        this->y = std::round(y);
    }

    T get_distance_to(Vec<T> other_vector) {
        return std::sqrt(std::pow(this->x - other_vector.x, 2) + std::pow(this->y - other_vector.y, 2));
    }

    [[nodiscard]] T magnitude() {
        return std::sqrt(this->x * this->x + this->y * this->y);
    }

    [[nodiscard]] T atan2() {
        return std::atan2(this->y, this->x);
    }

    [[nodiscard]] Vec<T> normalize() {
        // const float angle = this->atan2();
        // return {std::cos(angle), std::sin(angle)};
        return this->divide(this->magnitude());
    }

    void normalize_in_place() {
        this->divide_in_place(this->magnitude());
    }
    
    [[nodiscard]] std::string to_string() const {
        return std::string("<") + std::to_string(this->x) + std::string(", ") + std::to_string(this->y) + std::string(">");
    }
    [[nodiscard]] std::string to_string_2f() const {
        return std::format("<{0:.2f}, {0:.2f}>", this->x, this->y);
    }
    
    [[nodiscard]] Vector2 to_raylib() const {
        return {this->x, this->y};
    }
};