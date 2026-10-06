//
// Created by Иван Бояринцев on 20.12.25.
//

#ifndef GAMETIMER_H
#define GAMETIMER_H



#ifndef COUNTDOWN_TIMER_H
#define COUNTDOWN_TIMER_H

#include <chrono>

class CountdownTimer {
private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;
    using Seconds = std::chrono::seconds;

    TimePoint start_time_;          // Время старта
    Seconds initial_duration_;      // Начальная длительность
    Seconds remaining_time_;        // Оставшееся время
    bool is_running_ = false;       // Запущен ли таймер

public:
    CountdownTimer(int minutes, int seconds = 0);
    CountdownTimer(): initial_duration_(60), remaining_time_(60) {  };
    void start();

    void update();

    bool is_finished() const;

    int get_total_seconds() const;

    int get_minutes() const;
    int get_seconds() const;

    std::string to_string() const;

    bool is_running() const { return is_running_; }

    void set_time(int minutes, int seconds = 0);

    void add_time(int seconds);
    void add_minutes(int minutes);
};

#endif // COUNTDOWN_TIMER_H


#endif //GAMETIMER_H
