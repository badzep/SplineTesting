#ifndef SPLINETESTING_TIMER_HPP
#define SPLINETESTING_TIMER_HPP

#include <chrono>

class Timer {
protected:
    std::chrono::system_clock::time_point start_timestamp;
    std::chrono::system_clock::time_point end_timestamp;
    bool started = false;
public:
    /**
     * Start timing
     */
    void start();

    /**
     * Stop timing
     */
    void stop();

    /**
     * Get the duration for which the timer ran (in seconds) floored to the nanosecond
     * @return Duration with a minimum value of 1e-9 (1 nanosecond), so it is always safe to use as a divisor
     */
    [[nodiscard]] double get_duration();

    [[nodiscard]] bool has_started() const;

    void reset();
};

#endif //SPLINETESTING_TIMER_HPP
