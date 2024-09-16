#pragma once

template<class T>
class Register {
protected:
	T value;

public:
	Register(T init = 0) : value(init) { }

public:
	// Lập bit
	Register& Set(int i) {
		value |= (T(1) << i);
		return *this;
	}

	// Xóa toàn bộ các bit
	Register& Reset() {
		value = 0;
		return *this;
	}

	// Xóa bit
	Register& Reset(int i) {
		value &= ~(T(1) << i);
		return *this;
	}

	// Đảo bit
	Register& Invert(int i) {
		value ^= (T(1) << i);
		return *this;
	}

	// Lấy độ dài thanh ghi
	int Length() const { return sizeof(T); } // Nhân 8 ?

public:

	// Chuyển đổi sang dạng số
	operator T() { return value; }

	// Lấy giá trị của 1 bit
	int operator[](int index) {
		return (value & (T(1) << index) ? 1 : 0);
	}
};