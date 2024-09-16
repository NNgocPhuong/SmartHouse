#pragma once
class Counter {
protected:
	int reg_val;
	void on_created();

protected:
	// Phương thức ảo được gọi khi giá trị thanh ghi bị thay đổi
	virtual void on_changed() {}

	// Phương thức ảo được gọi khi giá trị thanh ghi về 0
	virtual void on_interrupt() {}

public:
	// Đọc giá trị thanh ghi
	virtual int Value() const { return reg_val; }

public:
	// Hàm tạo
	Counter() {
		reg_val = 0;
		on_created();
	}

	// Khởi động lại bộ đếm
	virtual void Reset() { reg_val = 0; }

	// Giảm giá trị của thanh ghi đi 1 đơn vị
	virtual void CountDown() { 
		if (reg_val) {
			--reg_val;

			on_changed();
			if (reg_val == 0) {
				on_interrupt();
			}
		}
	}

	// Dừng bộ đếm
	virtual void Stop() { reg_val = 0; }

	// Bắt đầu chạy bộ đếm từ giá trị thanh ghi ban đầu
	void Start(int value) { reg_val = value; }
};

class SecondCounter : public Counter {
protected:
	int seconds{ 0 };

public:
	SecondCounter() { }

protected:
	virtual bool running() { return true; }
	virtual void on_one_second() { }

	void on_interrupt() override {
		++seconds;

		on_one_second();
		if (running()) {
			reg_val = 1000;
		}
	}

public:
	void Reset() override {
		reg_val = 1000;
		seconds = 0;
	}
	int Value() const override { return seconds; }
};