#pragma once
#include "Buffer.h"

class TimeItem {
    byte value;
    byte limit;
    byte init;
    
    TimeItem* next;
    TimeItem* prev;

protected:
    virtual int get_max() { return limit - 1; }

public:
    TimeItem(int limit, int init = 0) : value(init), limit(limit), init(init) { 
        next = prev = 0;
    }

public:
    void SetNext(TimeItem* item) {
        next = item;
        item->prev = this;
    }
    void SetPrevious(TimeItem* item) {
        prev = item;
        item->next = this;
    }

    TimeItem* Next() const { return next; }
    TimeItem* Previous() const { return prev; }


    void Increase() {
        if (value == get_max()) {
            if (prev) prev->Increase();
            value = init;
        }
        else {
            ++value;
        }
    }
    void Decrease() {
        if (value == init) {
            if (prev) prev->Decrease();
            value = get_max();
        }
        else {
            --value;
        }
    }

public:    
    int Value() const { return value; }
    operator int() const { return value; }

    TimeItem& operator=(int v) {
        if (v < init) {
            v = init;
        }
        else {
            int m = get_max();
            if (v > m) v = m;
        }
        value = v;
        return *this;
    }
    TimeItem& operator++() { Increase(); return *this; }
    TimeItem& operator--() { Decrease(); return *this; }
};

class Clock : public TimeItem {
    TimeItem* mm;
    TimeItem* hh;

public:
    Clock() : TimeItem(60) {
        mm = new TimeItem(60);
        hh = new TimeItem(24);

        hh->SetNext(mm);
        mm->SetNext(this);
    }
    ~Clock() {
        delete mm;
        delete hh;
    }
public:
    TimeItem& Second() { return *this; }
    TimeItem& Minute() { return *mm; }
    TimeItem& Hour() { return *hh; }

    void Begin(int hour, int minute, int second) {
        Hour() = hour;
        Minute() = minute;
        Second() = second;
    }
    void Begin(LPCHAR time); // ví dụ time = "12" ==> 12:12:12
};

class Calendar : public TimeItem {
    TimeItem* MM;
    TimeItem* yy;

protected:
    int get_max() override { return DaysOfMonth(); }

public:
    int DaysOfMonth() {
        switch (MM->Value()) {
        case 2: return yy->Value() & 3 ? 28 : 29;
        case 4: case 6: case 9: case 11: return 30;
        }
        return 31;
    }
public:
    Calendar() : TimeItem(0, 1) { // TimeItem(limit, init)
        MM = new TimeItem(13, 1);
        yy = new TimeItem(100);

        yy->SetNext(MM);
        MM->SetNext(this);
    }
    ~Calendar() {
        delete MM;
        delete yy;
    }

public:
    TimeItem& Day() { return *this; }
    TimeItem& Month() { return *MM; }
    TimeItem& Year() { return *yy; }

    void Begin(int year, int month, int day) {
        Year() = year;
        Month() = month;
        Day() = day;
    }
};
