#pragma once
#pragma once
#include "Array.h"

template <class T>
class Matrix2D {
	T** data;
	int rows, cols;

	void create_data(int m, int n);
	void delete_data();
public:
	Matrix2D() : data(0), rows(0), cols(0) { }
	Matrix2D(int m, int n = 0) {
		create_data(m, n ? n : m);
	}
	Matrix2D(int m, int n, T* values);
	Matrix2D(const Matrix2D& src) : data(0) { operator=(src); }

	~Matrix2D() { delete_data(); }

public:
	T* operator[](int i) { return data[i]; }
	T& operator()(int i, int j) { return data[i][j]; }

	Matrix2D& operator=(const Matrix2D&);
	Matrix2D& operator+=(const Matrix2D&);
	Matrix2D& operator-=(const Matrix2D&);

	Matrix2D& operator*=(const T);

	int Rows() const { return rows; }
	int Columns() const { return cols; }

	class IT {
		Matrix2D* m;
		int r, c;
	public:
		IT(Matrix2D* m, int r = 0) : m(m), r(r), c(0) { }

		IT& operator++() {
			if (++c >= m->rows) {
				++r; c = 0;
			}
			return *this;
		}
		T& operator*() { return m->data[r][c]; }
		bool operator!=(const IT& e) { return r != e.r || c != e.c; }
	};

	IT begin() { return IT(this); }
	IT end() { return IT(this, rows); }
};

template<class T>
void Matrix2D<T>::create_data(int m, int n) {
	data = new T * [rows = m];
	cols = n;
	for (int i = 0; i < rows; i++) {
		data[i] = new T[n];
	}
}

template<class T>
void Matrix2D<T>::delete_data() {
	if (data) {
		for (int i = 0; i < rows; i++)
			delete[] data[i];
		delete[] data;
	}
}

template<class T>
Matrix2D<T>::Matrix2D(int m, int n, T* values) {
	create_data(m, n);
	for (int i = 0, k = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			data[i][j] = data[k++];
		}
	}
}

template <class T>
Matrix2D<T>& Matrix2D<T>::operator=(const Matrix2D<T>& src) {
	delete_data();
	create_data(src.rows, src.cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			data[i][j] = src.data[i][j];
	}
	return *this;
}

template <class T>
Matrix2D<T>& Matrix2D<T>::operator+=(const Matrix2D<T>& src) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			data[i][j] += src.data[i][j];
	}
	return *this;
}

template <class T>
Matrix2D<T>& Matrix2D<T>::operator-=(const Matrix2D<T>& src) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			data[i][j] -= src.data[i][j];
	}
	return *this;
}

template <class T>
Matrix2D<T>& Matrix2D<T>::operator*=(const T factor) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			data[i][j] *= factor;
	}
	return *this;
}
