#pragma once


#include <cassert>
#include <cmath>
#include <vector>


#include "Constants.hpp"
#include "Vector.hpp"


class Motion {
public:
    Vec<float> position;
    Vec<float> velocity;
    Vec<float> acceleration;

    Motion(const Vec<float> position, const Vec<float> velocity, const Vec<float> acceleration): position(position), velocity(velocity), acceleration(acceleration) {}

    Vec<float> get_velocity_end_point() const {
        return this->position.add(this->velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER));
    }

    Vec<float> get_acceleration_end_point() const {
        return this->position.add(this->velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER)).add(this->acceleration.multiply(ACCELERATION_DISPLAY_MULTIPLIER));
    }
};

class HermitePoint {
public:
    Vec<float> position;
    Vec<float> velocity;

    Vec<float> get_velocity_end_point() const {
        return this->position.add(this->velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER));
    }
};

class HermiteSpline {
protected:
    std::vector<HermitePoint> points;

public:
    std::vector<HermitePoint>& get_points() {
        return this->points;
    }

    [[nodiscard]] unsigned int get_point_count() const {
        return this->points.size();
    }

    void add_point(const HermitePoint end_point) {
        this->points.push_back(end_point);
    }

    float get_index_total() {
        return (int)(this->get_point_count() - 1);
    }

    float index_to_time(const float index) {
        return (index / this->get_index_total()) * DURATION;
    }

    float time_to_index(const float time) {
        return (time / DURATION) * this->get_index_total();
    }

    float get_time_scale() {
        return this->get_index_total() / DURATION;
    }

    Vec<float> get_position_at(const float index) {
        assert(this->points.size() > 1);

        unsigned int start_index = std::floor(index);

        const HermitePoint start_point = this->points[start_index];
        const HermitePoint end_point = this->points[start_index + 1];

        const float region_index = index - (float) start_index;

        const float index_squared = region_index * region_index;
        const float index_cubed = index_squared * region_index;

        const float start_position_coefficient = (2.0f * index_cubed - 3.0f * index_squared + 1.0f);
        const float start_velocity_coefficient = (index_cubed - 2.0f * index_squared + region_index);
        const float end_position_coefficient = (-2.0f * index_cubed + 3.0f * index_squared);
        const float end_velocity_coefficient = (index_cubed - index_squared);
        return {start_position_coefficient * start_point.position.x + start_velocity_coefficient * start_point.velocity.x + end_position_coefficient * end_point.position.x + end_velocity_coefficient * end_point.velocity.x,
                start_position_coefficient * start_point.position.y + start_velocity_coefficient * start_point.velocity.y + end_position_coefficient * end_point.position.y + end_velocity_coefficient * end_point.velocity.y};
    }

    Motion get_point_at(const float index) {
        assert(this->points.size() > 1);

        unsigned int start_index = std::floor(index);

        const HermitePoint start_point = this->points[start_index];
        const HermitePoint end_point = this->points[start_index + 1];

        const float region_index = index - (float) start_index;

        const float index_squared = region_index * region_index;
        const float index_cubed = index_squared * region_index;

        const float start_position_coefficient = (2.0f * index_cubed - 3.0f * index_squared + 1.0f);
        const float start_position_coefficient_d = (6.0f * (index_squared - region_index));
        const float start_position_coefficient_d2 = (12.0f * region_index - 6.0f);

        const float start_velocity_coefficient = (index_cubed - 2.0f * index_squared + region_index);
        const float start_velocity_coefficient_d = (3.0f * index_squared - 4.0f * region_index + 1.0f);
        const float start_velocity_coefficient_d2 = (6.0f * region_index - 4.0f);

        const float end_position_coefficient = (-2.0f * index_cubed + 3.0f * index_squared);
        const float end_position_coefficient_d = (6.0f * (region_index - index_squared));
        const float end_position_coefficient_d2 = (6.0f - 12.0f * region_index);

        const float end_velocity_coefficient = (index_cubed - index_squared);
        const float end_velocity_coefficient_d = (3.0f * index_squared - 2.0f * region_index);
        const float end_velocity_coefficient_d2 = (6.0f * region_index - 2.0f);

        return {
                {start_position_coefficient * start_point.position.x + start_velocity_coefficient * start_point.velocity.x + end_position_coefficient * end_point.position.x + end_velocity_coefficient * end_point.velocity.x,
                        start_position_coefficient * start_point.position.y + start_velocity_coefficient * start_point.velocity.y + end_position_coefficient * end_point.position.y + end_velocity_coefficient * end_point.velocity.y},
                {start_position_coefficient_d * start_point.position.x + start_velocity_coefficient_d * start_point.velocity.x + end_position_coefficient_d * end_point.position.x + end_velocity_coefficient_d * end_point.velocity.x,
                        start_position_coefficient_d * start_point.position.y + start_velocity_coefficient_d * start_point.velocity.y + end_position_coefficient_d * end_point.position.y + end_velocity_coefficient_d * end_point.velocity.y},
                {start_position_coefficient_d2 * start_point.position.x + start_velocity_coefficient_d2 * start_point.velocity.x + end_position_coefficient_d2 * end_point.position.x + end_velocity_coefficient_d2 * end_point.velocity.x,
                        start_position_coefficient_d2 * start_point.position.y + start_velocity_coefficient_d2 * start_point.velocity.y + end_position_coefficient_d2 * end_point.position.y + end_velocity_coefficient_d2 * end_point.velocity.y},
        };
    }
};
