#pragma once

typedef void(*far_proc)(); // tất cả các con trỏ hàm không có tham số và có kiểu trả về là void đều tên là far_proc
typedef unsigned char byte;
typedef char* PCHAR;
typedef const char* LPCHAR;

class Buffer {
	void mapping(void* src, int size = 128) {
		handle = it = (byte*)src;
		capacity = size;
	}

protected:
	byte* handle;
	byte* it;
	int capacity;

	virtual void Copy(LPCHAR s);

public:
	Buffer();
	Buffer(void* data, int size = 128) {
		mapping(data, size);
	}

public:
	Buffer& Mapping(void* handle, int size = 128) { 
		mapping(handle, size);
		return *this;
	}
	Buffer& Seek(int position) { it = handle + position; return *this; } // it = handle + position * sizeof(byte)

	int Position() const { return it - handle; } // do khoảng cách là 1 byte

	byte* First() { return handle; }
	byte* Last() { return handle + capacity - 1; } // do khoảng cách là 1 byte
public:
	virtual LPCHAR ToString() { *it = 0; return LPCHAR(handle); }
	operator LPCHAR() { return ToString(); }

	Buffer& operator++() { ++it; return *this; }

	byte& operator[](int index) { return handle[index]; }
	operator byte() { return *it; }

	LPCHAR ToString(int value);
	LPCHAR ToString(int value, int space);
	LPCHAR ToString(double value);

	long long GetInt64();

public:
	Buffer& operator<<(char c) { // gắn c tại vị trí it
		*it++ = c;
		return *this;
	}
	Buffer& operator<<(LPCHAR s) {
		if (s) Copy(s);
		return *this;
	}

	template<class T>
	Buffer& operator<<(T v) { 
		Copy(ToString(v));
		return *this;
	}
};

