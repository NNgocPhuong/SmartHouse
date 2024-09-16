// System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "../include/System/System.h"

const int MAX_RAM = 1 << 10;
byte main_buffer[MAX_RAM];

Buffer::Buffer() : handle(main_buffer), capacity(MAX_RAM), it(main_buffer) { }


void System::Tick() {
    for (auto one : timers) {
        one->CountDown();
    }
}

void System::Reset() {
    for (auto one : timers) {
        one->Reset();
    }
}

long total = 0;
void System::Loop() {
    long m = millis(); // trả về một số - là thời gian (tính theo mili giây) kể từ lúc mạch Arduino bắt đầu chạy
    while (total < m) {
        ++total;
        Tick();
    }
}

void Counter::on_created() {
    _system.Register(this);
}

void Keyboard::on_restart() {

    if (!state) {
        state = new int[Count()];
        clear();
    }

    int i = 0;
    for (auto key : *this) {
        int& s = state[i++];

        int v = get_key(key);
        if (v > 0) {
            if (++s > max_count_down) {
                on_key_down(key, s);
            }
            continue;
        }

        if (s == 0) continue;
        if (v < 0 && --s == 0) on_key_up(key, s);
    }
}

void Clock::Begin(LPCHAR time) {
    Buffer b(main_buffer + 512, 512);
    b << time;

    b.Seek(0);
    Hour() = b.GetInt64();
    Minute() = b.GetInt64();
    Second() = b.GetInt64();
}