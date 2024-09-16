#pragma once
#include "../System/System.h"

class Port : public Register<int> {
protected:
	byte* pins{ 0 };
	byte length{ 8 };
	bool invert{ false };

public:
	Port(int length = 8) : length(length) { }

	/// <summary>
	/// Kết nối đến các chân
	/// </summary>
	/// <param name="pins">mảng các chân</param>
	/// <param name="active_level">mức tích cực</param>
	void Connect(byte* pins, int active_level) {
		Connect(pins);
		this->invert = active_level == 0; // active_level có giá trị bằng 0 thì invert = true, nếu ko thì ko đảo ==> invert = false
	}

	/// <summary>
	/// Kết nối đến các chân
	/// </summary>
	/// <param name="pins">mảng các chân</param>
	void Connect(byte* pins) {
		this->pins = pins;
	}
};

extern int digitalRead(byte);
extern void digitalWrite(byte, byte);

class OutPort : public Port {
protected:
	virtual void write_one_bit(int pin, int value) { 
		digitalWrite(pin, value);
	}

public:
	OutPort(int length) : Port(length) { }

	void Write() {
		Write(this->value);
	}
	virtual void Write(int value) {
		Register<int> r = invert ? ~value : value;

		for (int i = 0; i < length; i++) {
			write_one_bit(pins[i], r[i]); // đã nạp chồng toán tử chỉ số cho Register
		}
	}
};

class InPort : public Port {
protected:
	virtual int read_one_bit(int pin) { return digitalRead(pin); }

public:
	InPort(int length) : Port(length) { }
	Register<int>& Read() {
		Register<int> r;
		for (int i = 0; i < length; i++) {
			if (read_one_bit(pins[i])) r.Set(i); // đọc giá trị vào r tạm xong mới gán vào value của Register
		}
		value = r;
		if (invert) value ^= -1; // -1 chính là 255 hay 0b1111 1111

		return *(Register<int>*)this;
	}
};

class LED7Seg : public OutPort {
public:

	LED7Seg(bool common_anode = true) : OutPort(7) { 
		invert = common_anode;
	}
	void Write(int value) override {
		static int codes[] = {
			0x3f,
			0x06,
			0x5b,
			0x4f,
			0x66,
			0x6d,
			0x7d,
			0x07,
			0x7f,
			0x6f,
		};

		OutPort::Write(codes[value]);
	}
};

template <int n>
class Multi7LED : OutPort, Timer {
	
	byte digits[n] = { };
	LED7Seg data;

public:
	Multi7LED(int ms_scan_inteval = 5)
		: Timer(ms_scan_inteval)
		, OutPort(n) {

		value = -1;
	
	}

	void SetControlBus(byte* pins, int active_level = 1) {
		Connect(pins, active_level);
	}
	void SetDataBus(byte* data_bus, int active_level = 0) {
		data.Connect(data_bus, active_level);
	}

	void Reset() override {
		Timer::Reset();
		value = 0;
	}

	void Write(int value) {
		Reset();
		for (int i = 0; i < n; i++) {
			digits[i] = value % 10;
			value /= 10;
		}
	}
	int SelectedIndex() { return n - value - 1; }

protected:
	virtual	void write_data(int v) { 
		data.Write(v); 
	}
	void on_restart() override {
		if (value < 0) return;

		OutPort::Write(1 << value);	// chọn LED
		write_data(digits[value]);	// ghi dữ liệu

		if (++value == n) {
			value = 0;
		}
	}
};