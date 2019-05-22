/*
 * simpler-timer.hpp
 *
 * A simple timer class that allows timing of scoped regions.
 */

#ifndef SIMPLE_TIMER_HPP_
#define SIMPLE_TIMER_HPP_

#include <cinttypes>
#include <stdexcept>
#include <chrono>
#include <string>
#include <cstdio>

#if !defined(ENABLE_TIMER) || !ENABLE_TIMER
#define DISABLE_TIMER
#endif

template <typename CLOCK>
class SimpleTimerT {
    using nanos_type = std::int64_t;

public:
    enum State {
        STARTED, STOPPED
    };

    SimpleTimerT(bool createStarted = true) : elapsed_(0), mode_(STOPPED) {
        if (createStarted) start();
    }

    void checkState(State expected, const char *msg) {
        if (mode_ != expected) {
            if (mode_ != STARTED && mode_ != STOPPED) {
                throw std::logic_error("bad state (maybe uninit?)");
            } else {
                throw std::logic_error(msg);
            }
        }
    }

    void start() {
        checkState(STOPPED, "counter already started");
        last_stamp_ = nanos();
        mode_ = STARTED;
    }

    void stop() {
        checkState(STARTED, "counter already stopped");
        elapsed_ += nanos() - last_stamp_;
        mode_ = STOPPED;
    }

    bool isStarted() {
        return mode_ == STARTED;
    }

    nanos_type elapsedNanos() {
        return elapsed_ + (isStarted() ? nanos() - last_stamp_ : 0);
    }

    template <typename DURATION>
    DURATION elapsedDuration() {
        return std::chrono::duration_cast<DURATION>(std::chrono::nanoseconds(elapsedNanos()));
    }

    template <typename DURATION>
    typename DURATION::rep elapsed() {
        return elapsedDuration<DURATION>().count();
    }

private:

    nanos_type elapsed_, last_stamp_;
    State mode_;

    static nanos_type nanos() {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(CLOCK::now().time_since_epoch()).count();
    }
};

using SimpleTimer = SimpleTimerT<std::chrono::high_resolution_clock>;

class LoggingTimer : public SimpleTimer {
public:
    const static int DEFAULT_WIDTH = 0;

    LoggingTimer(std::string message, int width = DEFAULT_WIDTH)
#ifndef DISABLE_TIMER
            :
            message_{std::move(message)},
            nameWidth(width),
            printed_(false)
#endif
            {}

    void printElapsed() {
#ifndef DISABLE_TIMER
        double ms = elapsedNanos() / 1000000.0;
        std::fprintf(stderr, "%-*s: %7.4f ms\n", nameWidth, message_.c_str(), ms);
        printed_ = true;
#endif
    }

    ~LoggingTimer() {
#ifndef DISABLE_TIMER
        if (!printed_) {
            printElapsed();
        }
#endif
    }

private:
#ifndef DISABLE_TIMER
    std::string message_;
    int nameWidth;
    bool printed_;
#endif
};



#endif /* SIMPLE_TIMER_HPP_ */
