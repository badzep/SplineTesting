#include <iostream>
#include <cmath>
#include <string>

class MotionState {
public:
    float x, v, a;
    
    MotionState(float x, float v, float a)
        : x(x), v(v), a(a) {}
        
    std::string to_string() const {
        return "x: " + std::to_string(x) + " v: " + std::to_string(v) + " a: " + std::to_string(a);
    }
};

class AsymmetricalProfile {
private:
    float max_accel, max_decel, max_vel, targetPosition;
    MotionState motionState{0, 0, 0};
    float dt1, dt2, dt3, profileDuration;
    
public:
    AsymmetricalProfile(float max_accel, float max_decel, float max_vel, float targetPosition)
        : max_accel(max_accel), max_decel(max_decel), max_vel(max_vel), targetPosition(targetPosition) {
        calculate();
    }
    
    void calculate() {
        dt1 = std::fabs(max_vel) / std::fabs(max_accel);
        dt3 = std::fabs(max_vel) / std::fabs(max_decel);
        float averageDt = (dt1 + dt3) / 2;
        dt2 = std::fabs(targetPosition) / std::fabs(max_vel) - averageDt;
        if (dt2 < 0) {
            dt2 = 0;

            if (std::fabs(max_accel) > std::fabs(max_decel)) {
                max_accel = std::fabs(max_decel);
            }
            else {
                max_decel = std::fabs(max_accel);
            }

            dt1 = std::sqrt(std::fabs(targetPosition) / std::fabs(max_accel));
            dt3 = std::sqrt(std::fabs(targetPosition) / std::fabs(max_decel));
        }

        profileDuration = dt1 + dt2 + dt3;
    }

    MotionState getState(float seconds) {
        float accel = 0, velocity = 0, position = 0;

        if (seconds <= dt1) {
            // accel 
            accel = std::fabs(max_accel);
            velocity = seconds * accel;  
            position = 0.5 * accel * std::pow(seconds, 2);
        }
        else if (seconds <= dt1 + dt2) {
            // cruise 
            accel = 0;
            velocity = getState(dt1).v;
            position = getState(dt1).x + max_vel * (seconds - dt1);
        }
        else if (seconds <= dt1 + dt2 + dt3) {
            // decel 
            accel = std::fabs(max_decel);
            float coastVelocity = getState(dt1).v;
            velocity = coastVelocity - (seconds - dt1 - dt2) * accel;
            float endOfdt2 = dt1 + dt2;
            float endofdt2Pos = getState(endOfdt2).x;
            position = endofdt2Pos + coastVelocity * (seconds - endOfdt2) - 0.5 * accel * std::pow(seconds - endOfdt2, 2);
            accel = -accel;
        }
        else {
            // done 
            accel = 0;
            velocity = 0;
            position = targetPosition;
        }
        return MotionState(position, velocity, accel);
    }
};
