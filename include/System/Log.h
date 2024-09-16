#pragma once
#include <Arduino.h>

const char endl = '\n';
class Log {
public:
	template <class T> Log& operator<<(const T& obj) {
		Serial.print(obj);
		return *this;
	}
};

