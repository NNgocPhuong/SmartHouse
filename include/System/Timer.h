#pragma once
#include "Counter.h"

class Timer : public Counter {
protected:
	int init_val;

public:
	// Lấy số lần đã chạy của Timer trong một chu trình
	int Passed() const { return init_val - reg_val; }

	// Lấy giá trị của chu trình
	int Interval() const { return init_val; }

public:
	// Khởi tạo Time, mặc định 1000 ms
	Timer(int interval = 1000) { init_val = interval; }

	// Khởi động lại Timer
	void Reset() override { 
		Counter::Start(init_val); 
	}

	// Dừng hoặc chạy Timer
	void Enable(bool enable = true) { enable ? Reset() : Stop(); }

protected:
	// Phương thức ảo được gọi khi khởi động lại Timer
	virtual void on_restart() { }

	// Phương thức ảo được gọi khi hết một chu trình
	void on_interrupt() override { 
		on_restart();
		Counter::Start(init_val);
	}
};
