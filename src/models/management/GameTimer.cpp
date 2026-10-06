//
// Created by Иван Бояринцев on 20.12.25.
//

#include "GameTimer.h"
#include "GameTimer.h"
#include <sstream>
#include <iomanip>

CountdownTimer::CountdownTimer(int minutes, int seconds)
    : initial_duration_(minutes * 60 + seconds),
      remaining_time_(initial_duration_) {
}

void CountdownTimer::start() {
    start_time_ = Clock::now();
    is_running_ = true;
}

void CountdownTimer::update() {
    if (!is_running_) return;

    auto now = Clock::now();
    auto elapsed = std::chrono::duration_cast<Seconds>(now - start_time_);

    if (elapsed < remaining_time_) {
        remaining_time_ = initial_duration_ - elapsed;
    } else {
        remaining_time_ = Seconds(0);
        is_running_ = false;
    }
}

bool CountdownTimer::is_finished() const {
    return remaining_time_.count() <= 0;
}

int CountdownTimer::get_total_seconds() const {
    return static_cast<int>(remaining_time_.count());
}

int CountdownTimer::get_minutes() const {
    return get_total_seconds() / 60;
}

int CountdownTimer::get_seconds() const {
    return get_total_seconds() % 60;
}

std::string CountdownTimer::to_string() const {
    int minutes = get_minutes();
    int seconds = get_seconds();

    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds;
    return ss.str();
}

void CountdownTimer::set_time(int minutes, int seconds) {
    initial_duration_ = Seconds(minutes * 60 + seconds);
    remaining_time_ = initial_duration_;
}

void CountdownTimer::add_time(int seconds) {
    remaining_time_ += Seconds(seconds);
    if (remaining_time_.count() < 0) {
        remaining_time_ = Seconds(0);
    }
}

void CountdownTimer::add_minutes(int minutes) {
    add_time(minutes * 60);
}