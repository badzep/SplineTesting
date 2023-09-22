#pragma once


#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>


#include "Constants.hpp"
#include "Vector.hpp"


class Motion {
public:
    Vec<float> position;
    Vec<float> velocity;
    Vec<float> acceleration;

    Motion(const Vec<float> position, const Vec<float> velocity, const Vec<float> acceleration): position(position), velocity(velocity), acceleration(acceleration) {}
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
    float total_duration = 5.0f;

    void print_parameters() {
        std::cout << "spline.total_duration = " << this->total_duration << ";"<< "\n";
        for (HermitePoint &point: this->points) {
            std::cout << "spline.add_point(HermitePoint{Vec<float>{" << point.position.x << ", " << point.position.y << "}, Vec<float>{" << point.velocity.x << ", " << point.velocity.y << "}});" << "\n";
        }
    }

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
        return (index / this->get_index_total()) * this->total_duration;
    }

    float time_to_index(const float time) {
        return (time / this->total_duration) * this->get_index_total();
    }

    float get_time_scale() {
        return this->get_index_total() / this->total_duration;
    }

    Vec<float> get_position_at(const float index) {
        unsigned int start_index = std::floor(index);

        const HermitePoint start_point = this->points[start_index];
        const HermitePoint end_point = this->points[start_index + 1];

        const float region_index = index - (float) start_index;
        const float index_squared = region_index * region_index;
        const float index_cubed = index_squared * region_index;

        return start_point.position * (2.0f * index_cubed - 3.0f * index_squared + 1.0f) 
            + start_point.velocity * (index_cubed - 2.0f * index_squared + region_index)
            + end_point.position * (-2.0f * index_cubed + 3.0f * index_squared)
            + end_point.velocity * (index_cubed - index_squared);
    }

    Motion get_motion_at(const float index) {
        unsigned int start_index = std::floor(index);

        const HermitePoint start_point = this->points[start_index];
        const HermitePoint end_point = this->points[start_index + 1];

        const float region_index = index - (float) start_index;
        const float index_squared = region_index * region_index;
        const float index_cubed = index_squared * region_index;

        return {
            start_point.position * (2.0f * index_cubed - 3.0f * index_squared + 1.0f) 
            + start_point.velocity * (index_cubed - 2.0f * index_squared + region_index)
            + end_point.position * (-2.0f * index_cubed + 3.0f * index_squared)
            + end_point.velocity * (index_cubed - index_squared),

            start_point.position * (6.0f * (index_squared - region_index)) 
            + start_point.velocity * (3.0f * index_squared - 4.0f * region_index + 1.0f)
            + end_point.position * (6.0f * (region_index - index_squared))
            + end_point.velocity * (3.0f * index_squared - 2.0f * region_index),

            start_point.position * (12.0f * region_index - 6.0f) 
            + start_point.velocity * (6.0f * region_index - 4.0f)
            + end_point.position * (6.0f - 12.0f * region_index)
            + end_point.velocity * (6.0f * region_index - 2.0f),

            // start_point.position * (12.0f)
            // + start_point.velocity * (6.0f)
            // + end_point.position * (-12.0f)
            // + end_point.velocity * (6.0f)
        };
    }
};
