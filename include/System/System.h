#pragma once
#include "Array.h"
#include "Buffer.h"
#include "ByteTable.h"
#include "DateTime.h"
#include "Keyboard.h"
#include "List.h"
#include "Log.h"
#include "Matrix.h"
#include "Register.h"
#include "Timer.h"

class System : public Log {
	List<Counter*> timers;
	int clock_cycle;

public:

	void Register(Counter* item) { // thêm counter
		timers.AddLast(item);
	}
	void Tick();

	static void Call(far_proc proc) {
		if (proc != 0) proc();
	}
public:
	far_proc on_milis;
	far_proc on_nanos;

	System() : on_milis(0), on_nanos(0), clock_cycle(600) { }

public:
	int& ClockCycle() {
		return clock_cycle;
	}
	
	void Reset();
	void Loop();
};

extern System _system;