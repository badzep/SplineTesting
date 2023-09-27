#pragma once


#include <cmath>
#include <string>
#include <format>
#include <raylib.h>


template<const unsigned char DIMENSIONS, typename T>
class Vec {
public:
    T values[DIMENSIONS];
};

template<const unsigned char DIMENSIONS>
using Vecnf = Vec<DIMENSIONS, float>;
template<const unsigned char DIMENSIONS>
using Vecnd = Vec<DIMENSIONS, double>;

template<typename T>
using Vec2 = Vec<2, T>;
template<typename T>
using Vec2 = Vec<2, T>;

template<typename T>
using Vec3 = Vec<3, T>;
template<typename T>
using Vec3 = Vec<3, T>;

using Vec2f = Vec<2, float>;
using Vec3f = Vec<3, float>;

using Vec2d = Vec<2, double>;
using Vec3d = Vec<3, double>;

template<typename T>
class Vec<2, T> {
    public:
    T x;
    T y;

    Vec() {    }
    constexpr Vec(const T x, const T y): x(x), y(y) {    }
    // Vec(const T x, const T y): x(x), y(y) {    }

    explicit Vec(const Vector2 vector): x(vector.x), y(vector.y) {    }

    ~Vec() = default;

    [[nodiscard]] Vec<2, T> operator+(Vec<2, T> const other_vector) const {
        return {this->x + other_vector.x, this->y + other_vector.y};
    }
    [[nodiscard]] Vec<2, T> operator+(const T other) const {
        return {this->x + other, this->y + other};
    }
    void operator+=(Vec<2, T> const other_vector) {
        this->x += other_vector.x;
        this->y += other_vector.y;
    }
    void operator+=(const T other) {
        this->x += other;
        this->y += other;
    }
    [[nodiscard]] Vec<2, T> add(const Vec<2, T> other_vector) const {
        return {this->x + other_vector.x, this->y + other_vector.y};
    }
    [[nodiscard]] Vec<2, T> add(const T other) const {
        return {this->x + other, this->y + other};
    }
    void add_in_place(const Vec<2, T> other_vector) {
        this->x += other_vector.x;
        this->y += other_vector.y;
    }
    void add_in_place(const T other) {
        this->x += other;
        this->y += other;
    }

    [[nodiscard]] Vec<2, T> operator-(Vec<2, T> const other_vector) const {
        return {this->x - other_vector.x, this->y - other_vector.y};
    }
    [[nodiscard]] Vec<2, T> operator-(const T other) const {
        return {this->x - other, this->y - other};
    }
    void operator-=(Vec<2, T> const other_vector) {
        this->x -= other_vector.x;
        this->y -= other_vector.y;
    }
    void operator-=(const T other) {
        this->x -= other;
        this->y -= other;
    }
    [[nodiscard]] Vec<2, T> subtract(const Vec<2, T> other_vector) const {
        return {this->x - other_vector.x, this->y - other_vector.y};
    }
    [[nodiscard]] Vec<2, T> subtract(const T other) const {
        return {this->x - other, this->y - other};
    }
    void subtract_in_place(const Vec<2, T> other_vector) {
        this->x -= other_vector.x;
        this->y -= other_vector.y;
    }
    void subtract_in_place(const T other) {
        this->x -= other;
        this->y -= other;
    }

    [[nodiscard]] Vec<2, T> operator*(Vec<2, T> const other_vector) const {
        return {this->x * other_vector.x, this->y * other_vector.y};
    }
    [[nodiscard]] Vec<2, T> operator*(const T other) const {
        return {this->x * other, this->y * other};
    }
    void operator*=(Vec<2, T> const other_vector) {
        this->x *= other_vector.x;
        this->y *= other_vector.y;
    }
    void operator*=(const T other) {
        this->x *= other;
        this->y *= other;
    }
    [[nodiscard]] Vec<2, T> multiply(const Vec<2, T> other_vector) const {
        return {this->x * other_vector.x, this->y * other_vector.y};
    }
    [[nodiscard]] Vec<2, T> multiply(const T other) const {
        return {this->x * other, this->y * other};
    }
    void multiply_in_place(const Vec<2, T> other_vector) {
        this->x *= other_vector.x;
        this->y *= other_vector.y;
    }
    void multiply_in_place(const T other) {
        this->x *= other;
        this->y *= other;
    }

    [[nodiscard]] Vec<2, T> operator/(Vec<2, T> const other_vector) const {
        return {this->x / other_vector.x, this->y / other_vector.y};
    }
    [[nodiscard]] Vec<2, T> operator/(const T other) const {
        return {this->x / other, this->y / other};
    }
    void operator/=(Vec<2, T> const other_vector) {
        this->x /= other_vector.x;
        this->y /= other_vector.y;
    }
    void operator/=(const T other) {
        this->x /= other;
        this->y /= other;
    }
    [[nodiscard]] Vec<2, T> divide(const Vec<2, T> other_vector) const {
        return {this->x / other_vector.x, this->y / other_vector.y};
    }
    [[nodiscard]] Vec<2, T> divide(const T other) const {
        return {this->x / other, this->y / other};
    }
    void divide_in_place(const Vec<2, T> other_vector) {
        this->x /= other_vector.x;
        this->y /= other_vector.y;
    }
    void divide_in_place(const T other) {
        this->x /= other;
        this->y /= other;
    }

    [[nodiscard]] Vec<2, T> power(const Vec<2, T> other_vector) const {
        return {std::pow(this->x, other_vector.x), std::pow(this->y, other_vector.y)};
    }
    [[nodiscard]] Vec<2, T> power(const T other) const {
        return {std::pow(this->x, other), std::pow(this->y, other)};
    }
    void power_in_place(const Vec<2, T> other_vector) {
        this->x = std::pow(this->x, other_vector.x);
        this->y = std::pow(this->y, other_vector.y);
    }
    void power_in_place(const T other) {
        this->x = std::pow(this->x, other);
        this->y = std::pow(this->y, other);
    }

    [[nodiscard]] Vec<2, T> abs() const {
        return {std::abs(this->x), std::abs(this->y)};
    }
    void abs_in_place() {
        this->x = std::abs(x);
        this->y = std::abs(y);
    }

    [[nodiscard]] Vec<2, T> sqrt() const {
        return {std::sqrt(this->x), std::sqrt(this->y)};
    }
    void sqrt_in_place() {
        this->x = std::sqrt(x);
        this->y = std::sqrt(y);
    }

    [[nodiscard]] Vec<2, T> round() const {
        return {std::round(this->x), std::round(this->y)};
    }
    void round_in_place() {
        this->x = std::round(x);
        this->y = std::round(y);
    }

    T get_distance_to(Vec<2, T> other_vector) const {
        return std::sqrt(std::pow(this->x - other_vector.x, 2) + std::pow(this->y - other_vector.y, 2));
    }

    [[nodiscard]] T magnitude() const {
        return std::sqrt(this->x * this->x + this->y * this->y);
    }

    [[nodiscard]] T atan2() const {
        return std::atan2(this->y, this->x);
    }

    [[nodiscard]] Vec<2, T> normalize() const {
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
    
    [[nodiscard]] Vec<2, T> rotate(const T angle) {
        const float magnitude = this->magnitude();
        return {magnitude * std::cos(angle), magnitude * std::sin(angle)};
    }

    [[nodiscard]] Vec<2, T> min(const T minimum) const {
        return {std::min(this->x, minimum), std::min(this->y, minimum)};
    }
    [[nodiscard]] Vec<2, T> min(const Vec<2, T> minimum) const {
        return {std::min(this->x, minimum.x), std::min(this->y, minimum.y)};
    }

    [[nodiscard]] Vec<2, T> max(const T maximum) const {
        return {std::max(this->x, maximum), std::max(this->y, maximum)};
    }
    [[nodiscard]] Vec<2, T> max(const Vec<2, T> maximum) const {
        return {std::max(this->x, maximum.x), std::max(this->y, maximum.y)};
    }

    void min_in_place(const T minimum) {
        this->x = std::min(this->x, minimum);
        this->y = std::min(this->y, minimum);
    }
    void min_in_place(const Vec<2, T> minimum) {
        this->x = std::min(this->x, minimum.x);
        this->y = std::min(this->y, minimum.y);
    }

    void max_in_place(const T maximum) {
        this->x = std::max(this->x, maximum);
        this->y = std::max(this->y, maximum);
    }
    void max_in_place(const Vec<2, T> maximum) {
        this->x = std::max(this->x, maximum.x);
        this->y = std::max(this->y, maximum.y);
    }

    [[nodiscard]] Vec<2, T> range(const T minimum, const T maximum) const {
        return {std::max(std::min(this->x, minimum), maximum), std::max(std::min(this->y, minimum), maximum)};
    }
    [[nodiscard]] Vec<2, T> range(const Vec<2, T> minimum, const Vec<2, T> maximum) const {
        return {std::max(std::min(this->x, minimum.x), maximum.x), std::max(std::min(this->y, minimum.y), maximum.y)};
    }

    void range_in_place(const T minimum, const T maximum) {
        this->x = std::max(std::min(this->x, minimum), maximum);
        this->y = std::max(std::min(this->y, minimum), maximum);
    }
    void range_in_place(const Vec<2, T> minimum, const Vec<2, T> maximum) {
        this->x = std::max(std::min(this->x, minimum.x), maximum.x);
        this->y = std::max(std::min(this->y, minimum.y), maximum.y);
    }

    [[nodiscard]] Vector2 to_raylib() const {
        return {this->x, this->y};
    }

    /**
     * Also works in reverse
     */
    [[nodiscard]] Vec<2, T> field_to_screen() const {
        return {-this->y, -this->x};
    }

    /**
     * Only exists for the sake of clarity
     */
    [[nodiscard]] Vec<2, T> screen_to_field() const {
        return {-this->y, -this->x};
    }

    [[nodiscard]] Vec<3, T> to_3d(const T z) const {
        return {this->x, this->y, z};
    }
};

template<typename T> 
class Vec<3, T> {
public:
    T x;
    T y;
    T z;

    Vec() {    }

    Vec(const T x, const T y, const T z): x(x), y(y), z(z) {    }

    explicit Vec(const Vector3 vector): x(vector.x), y(vector.y), z(vector.z) {    }

    ~Vec() = default;

    [[nodiscard]] Vec<3, T> operator+(Vec<3, T> const other_vector) const {
        return {this->x + other_vector.x, this->y + other_vector.y, this->z + other_vector.z};
    }
    [[nodiscard]] Vec<3, T> operator+(const T other) const {
        return {this->x + other, this->y + other, this->z + other};
    }
    void operator+=(Vec<3, T> const other_vector) {
        this->x += other_vector.x;
        this->y += other_vector.y;
        this->z += other_vector.z;
    }
    void operator+=(const T other) {
        this->x += other;
        this->y += other;
        this->z += other;
    }
    [[nodiscard]] Vec<3, T> add(const Vec<3, T> other_vector) const {
        return {this->x + other_vector.x, this->y + other_vector.y, this->z + other_vector.z};
    }
    [[nodiscard]] Vec<3, T> add(const T other) const {
        return {this->x + other, this->y + other, this->z + other};
    }
    void add_in_place(const Vec<3, T> other_vector) {
        this->x += other_vector.x;
        this->y += other_vector.y;
        this->z += other_vector.z;
    }
    void add_in_place(const T other) {
        this->x += other;
        this->y += other;
        this->z += other;
    }

    [[nodiscard]] Vec<3, T> operator-(Vec<3, T> const other_vector) const {
        return {this->x - other_vector.x, this->y - other_vector.y, this->z - other_vector.z};
    }
    [[nodiscard]] Vec<3, T> operator-(const T other) const {
        return {this->x - other, this->y - other, this->z - other};
    }
    void operator-=(Vec<3, T> const other_vector) {
        this->x -= other_vector.x;
        this->y -= other_vector.y;
        this->z -= other_vector.z;
    }
    void operator-=(const T other) {
        this->x -= other;
        this->y -= other;
        this->z -= other;
    }
    [[nodiscard]] Vec<3, T> subtract(const Vec<3, T> other_vector) const {
        return {this->x - other_vector.x, this->y - other_vector.y, this->z - other_vector.z};
    }
    [[nodiscard]] Vec<3, T> subtract(const T other) const {
        return {this->x - other, this->y - other, this->z - other};
    }
    void subtract_in_place(const Vec<3, T> other_vector) {
        this->x -= other_vector.x;
        this->y -= other_vector.y;
        this->z -= other_vector.z;
    }
    void subtract_in_place(const T other) {
        this->x -= other;
        this->y -= other;
        this->z -= other;
    }

    [[nodiscard]] Vec<3, T> operator*(Vec<3, T> const other_vector) const {
        return {this->x * other_vector.x, this->y * other_vector.y, this->z * other_vector.z};
    }
    [[nodiscard]] Vec<3, T> operator*(const T other) const {
        return {this->x * other, this->y * other, this->z * other};
    }
    void operator*=(Vec<3, T> const other_vector) {
        this->x *= other_vector.x;
        this->y *= other_vector.y;
        this->z *= other_vector.z;
    }
    void operator*=(const T other) {
        this->x *= other;
        this->y *= other;
        this->z *= other;
    }
    [[nodiscard]] Vec<3, T> multiply(const Vec<3, T> other_vector) const {
        return {this->x * other_vector.x, this->y * other_vector.y, this->z * other_vector.z};
    }
    [[nodiscard]] Vec<3, T> multiply(const T other) const {
        return {this->x * other, this->y * other, this->z * other};
    }
    void multiply_in_place(const Vec<3, T> other_vector) {
        this->x *= other_vector.x;
        this->y *= other_vector.y;
        this->z *= other_vector.z;
    }
    void multiply_in_place(const T other) {
        this->x *= other;
        this->y *= other;
        this->z *= other;
    }

    [[nodiscard]] Vec<3, T> operator/(Vec<3, T> const other_vector) const {
        return {this->x / other_vector.x, this->y / other_vector.y, this->z / other_vector.z};
    }
    [[nodiscard]] Vec<3, T> operator/(const T other) const {
        return {this->x / other, this->y / other, this->z / other};
    }
    void operator/=(Vec<3, T> const other_vector) {
        this->x /= other_vector.x;
        this->y /= other_vector.y;
        this->z /= other_vector.z;
    }
    void operator/=(const T other) {
        this->x /= other;
        this->y /= other;
        this->z /= other;
    }
    [[nodiscard]] Vec<3, T> divide(const Vec<3, T> other_vector) const {
        return {this->x / other_vector.x, this->y / other_vector.y, this->z / other_vector.z};
    }
    [[nodiscard]] Vec<3, T> divide(const T other) const {
        return {this->x / other, this->y / other, this->z / other};
    }
    void divide_in_place(const Vec<3, T> other_vector) {
        this->x /= other_vector.x;
        this->y /= other_vector.y;
        this->z /= other_vector.z;
    }
    void divide_in_place(const T other) {
        this->x /= other;
        this->y /= other;
        this->z /= other;
    }

    [[nodiscard]] Vec<3, T> power(const Vec<3, T> other_vector) const {
        return {std::pow(this->x, other_vector.x), std::pow(this->y, other_vector.y), std::pow(this->z, other_vector.z)};
    }
    [[nodiscard]] Vec<3, T> power(const T other) const {
        return {std::pow(this->x, other), std::pow(this->y, other), std::pow(this->z, other)};
    }
    void power_in_place(const Vec<3, T> other_vector) {
        this->x = std::pow(this->x, other_vector.x);
        this->y = std::pow(this->y, other_vector.y);
        this->z = std::pow(this->z, other_vector.z);
    }
    void power_in_place(const T other) {
        this->x = std::pow(this->x, other);
        this->y = std::pow(this->y, other);
        this->z = std::pow(this->z, other);
    }

    [[nodiscard]] Vec<3, T> abs() const {
        return {std::abs(this->x), std::abs(this->y), std::abs(this->z)};
    }
    void abs_in_place() {
        this->x = std::abs(x);
        this->y = std::abs(y);
        this->z = std::abs(z);
    }

    [[nodiscard]] Vec<3, T> sqrt() const {
        return {std::sqrt(this->x), std::sqrt(this->y), std::sqrt(this->z)};
    }
    void sqrt_in_place() {
        this->x = std::sqrt(x);
        this->y = std::sqrt(y);
        this->z = std::sqrt(z);
    }

    [[nodiscard]] Vec<3, T> round() const {
        return {std::round(this->x), std::round(this->y)};
    }
    void round_in_place() {
        this->x = std::round(x);
        this->y = std::round(y);
    }

    T get_distance_to(Vec<3, T> other_vector) const {
        return std::sqrt(std::pow(this->x - other_vector.x, 2) + std::pow(this->y - other_vector.y, 2)  + std::pow(this->z - other_vector.z, 2));
    }

    [[nodiscard]] T magnitude() const {
        return std::sqrt(this->x * this->x + this->y * this->y, this->z * this->z);
    }

    [[nodiscard]] Vec<3, T> normalize() const {
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
