#pragma once
#include "List.h"
#include "Timer.h"
#include "Buffer.h"

class Keyboard : List<int>, public Timer {
protected:
	int* state;
	int max_count_down;

protected:
	void clear() {
		for (int i = 0; i < Count(); i++)
			state[i] = 0;
	}

public:
	Keyboard(int interval, int threshold) 
		: Timer(interval), state(0), max_count_down((threshold >> 1) << 1) { } // max_count_down luôn là số chẵn?
	~Keyboard() {
		if (state) delete[] state;
	}
public:
	void Add(int key) {
		this->AddLast(key);
	}
	Keyboard& operator<<(int key) {
		Add(key);
		return *this;
	}

protected:

	virtual int get_key(int keycode) = 0;
	virtual void on_key_down(int key, int& count) { }
	virtual void on_key_up(int key, int &count) { }

	void on_restart() override;
};