#include "Timer.hpp"


/**
 * P: Proportional
 * I: Integral
 * D: Derivative
 */
class PID {
protected:
    const float k_proportional;
    const float k_integral;
    const float k_derivative;

    Timer timer;
    bool first_loop = true;

    float integral = 0;
    float last_error = 0;

public:
    constexpr PID(const float k_proportional, const float k_integral, const float k_derivative): k_proportional(k_proportional), k_integral(k_integral), k_derivative(k_derivative) {}

    float calculate(const float error) {
        if (this->first_loop) {
            this->first_loop = false;
            this->timer.start();
            return this->k_proportional * error;
        }

        const float delta_time = this->timer.get_duration();
        this->timer.reset();

        this->integral += error * delta_time;
        const float derivative = (error - this->last_error) / delta_time;
        this->last_error = error;
        return this->k_proportional * error + this->k_integral * this->integral + this->k_derivative * derivative;
    }
};
