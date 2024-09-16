#pragma once
template <class T>
class Array {
	T* data;
	int length;

	void create_data(int size) {
		data = new T[length = size];
	}
	void copy_data(const T* src);
	void free_data() {
		if (data) delete[] data;
	}

public:
	Array() : data(0), length(0) { }
	Array(int size, const T* values = 0) {
		create_data(size);
		copy_data(values);
	}
	Array(const Array& src) {
		create_data(src.length);
		copy_data(src.data);
	}
	~Array() { free_data(); }

public:
	int Length() const { return length; }
	T* Handle() const { return data; }

	T& GetAt(int index) const { return data[index]; }
	void SetAt(int index, const T& value) { data[index] = value; }

	Array& Resize(int size) {
		free_data(); // xóa rồi mới cấp phát động lại
		create_data(size);

		return *this;
	}
public:
	T& operator[](int index) { return data[index]; }
	Array& operator=(const T* values) {
		copy_data(values);
		return *this;
	}
	Array& operator=(const Array& src) { // Gán xâu, na ná mapping bên Buffer
		free_data();
		create_data(src.length);
		copy_data(src.data);

		return *this;
	}

public:
	class Iterator {
		T* ptr;
	public:
		Iterator(T* p) : ptr(p) { }
		T& operator*() { return *ptr; }
		Iterator& operator++() { ++ptr; return *this; }

		friend bool operator!=(const Iterator& a, const Iterator& b) {
			return a.ptr != b.ptr;
		}
	};

	friend Iterator begin(const Array& a) { return Iterator(a.data); }
	friend Iterator end(const Array& a) { return Iterator(a.data + a.length); }
};

template<class T>
void Array<T>::copy_data(const T* src) {
	if (src) {
		for (int i = 0; i < length; i++) {
			data[i] = src[i];
		}
	}
}
