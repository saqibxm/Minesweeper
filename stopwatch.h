#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <chrono>

namespace mines {

class Stopwatch {
    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<double>;
public:
    void start() {
        if (!running) {
            start_time = Clock::now();
            running = true;
        }
    }

    void stop() {
        if (running) {
            auto now = Clock::now();
            Duration elapsed_seconds = now - start_time;
            accumulated_time += elapsed_seconds.count();
            running = false;
        }
    }

    Duration::rep elapsed() const {
        if (running) {
            auto now = Clock::now();
            Duration elapsed_seconds = now - start_time;
            return accumulated_time + elapsed_seconds.count();
        } else {
            return accumulated_time;
        }
    }

    void reset() {
        start_time = Clock::now();
        accumulated_time = 0;
        running = false;
    }

    bool is_running() const { return running; }

private:
    std::chrono::time_point<Clock> start_time;
    Duration::rep accumulated_time = 0;
    bool running = false;
};

} // namespace mines

#endif // STOPWATCH_HPP