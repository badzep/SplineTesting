#pragma once

#include <cmath>
#include <raylib.h>

template<typename T> class Vec {
public:
    T x;
    T y;

    Vec(const T x, const T y): x(x), y(y) {    }

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

    [[nodiscard]] Vector2 to_raylib() const {
        return {this->x, this->y};
    }
};