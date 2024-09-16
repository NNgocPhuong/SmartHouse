#pragma once

#include "../System/System.h"

class ScreenLine : public Buffer {
protected:
	void Copy(LPCHAR s) override {
		Buffer::Copy(s);
		Count() = Position();
	}
public:
	template<class T>
	ScreenLine& operator<<(T value) {
		return *(ScreenLine*)(&Buffer::operator<<(value));
	}

	ScreenLine& operator<<(char c) {
		*it++ = c;
		Count()++;

		return *this;
	}

	ScreenLine& operator<<(const TimeItem& item) {
		int v = item;
		if (v < 10) {
			return *this << '0' << char(v | 0x30);
		}
		return *this << v;
	}

	byte& Count() { return *Last(); }
	LPCHAR ToString() override {
		byte& n = Count();
		if (n == 0) return 0;

		Seek(n);
		while (it != &n) {
			*it++ = ' '; // chèn khoảng trắng nếu trống
		}
		n = 0; // chốt xâu
		return LPCHAR(handle);
	}
};

class ScreenBuffer : ScreenLine, ByteTable, Timer {
protected:
	// Phương thức ảo cập nhật một dòng màn hình
	virtual void update_device(int row, LPCHAR content) = 0;

	// Phương thức ảo được gọi khi kết thúc cập nhật màn hình
	virtual void on_update_completed() { }

	// Cập nhật tất cả các dòng màn hình
	void on_restart() override {
		for (int i = 0; i < Height(); i++) {
			auto& line = GetLine(i);
			auto content = line.ToString();

			if (content) update_device(i, content);
		}
		on_update_completed();
	}


public:
	ScreenBuffer(int rows, int cols, int update_interval = 100);

	// Lấy buffer của một dòng màn hình
	ScreenLine& GetLine(int index) {
		ScreenLine::Mapping(GetRow(index), RowSize());
		return *this;
	}

	int Width() const { return RowSize() - 1; }
	int Height() const { return Rows(); }
};