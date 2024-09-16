#pragma once
#include "Array.h"

template <class T>
class Matrix : public Array<T> {
	int rows, cols;
	
	void create_matrix(int m, int n, const T* data = 0) {
		rows = m; cols = n == 0 ? m : n;

		Array<T>::Resize(rows * cols);
		Array<T>::operator=(data);
	}

public:
	Matrix() : rows(0), cols(0) { }
	Matrix(int m, int n = 0) { create_matrix(m, n); }
	Matrix(int m, int n, const T* values) { create_matrix(m, n, values); }

	Matrix(const Matrix& src) : rows(src.rows), cols(src.cols), Array<T>(src) { }

public:
	T& operator()(int i, int j) { return Array<T>::operator[](i * cols + j); }

	int Rows() const { return rows; }
	int Columns() const { return cols; }

	Matrix& Resize(int m, int n) {
		create_matrix(m, n);
		return *this;
	}
	Matrix& operator=(const Matrix& src) {
		create_matrix(src.rows, src.cols);
		Array<T>::operator=(src.Handle());

		return *this;
	}
	Matrix& operator*=(const T factor) {
		for (auto& x : *this) {
			x *= factor;
		}
		return *this;
	}
	Matrix& operator+=(const Matrix& matrix) {
		int k = 0;
		for (auto& x : *this) {
			x += matrix.GetAt(k++);
		}
		return *this;
	}
	Matrix& operator-=(const Matrix& matrix) {
		int k = 0;
		for (auto& x : *this) {
			x -= matrix.GetAt(k++);
		}
		return *this;
	}

	friend Matrix operator+(const Matrix& A, const Matrix& B) {
		Matrix m = A;
		return m += B;
	}
	friend Matrix operator-(const Matrix& A, const Matrix& B) {
		Matrix m = A;
		return m -= B;
	}

	friend Matrix operator*(const Matrix& matrix, const T factor) {
		Matrix m = matrix;
		return m *= factor;
	}
	friend Matrix operator*(const T factor, const Matrix& matrix) {
		return matrix * factor;
	}
	friend Matrix operator*(const Matrix& A, const Matrix& B) {
		Matrix C(A.rows, B.cols);
		//T* a = A.Handle();
		//T* c = C.Handle();

		//for (int i = 0; i < C.rows; i++, a += A.cols) {
		//	T* b = B.Handle();

		//	for (int j = 0; j < C.cols; j++, b++) {
		//		T* cb = b;
		//		T s = 0;
		//		for (int k = 0; k < A.cols; k++, cb += B.cols) {
		//			s += (*(a + k)) * (*cb);
		//		}

		//		*(c++) = s;
		//	}
		//}

		int n = 0;
		T* ra = A.Handle();
		T* cb = B.Handle();

		for (auto& x : C) {
			T* b = cb;

			x = 0;
			for (int i = 0; i < A.cols; i++, b += B.cols) {
				x += ra[i] * (*b);
			}

			if (++n == C.cols) {
				n = 0;
				ra += A.cols;
				cb = B.Handle();
			}
			else {
				++cb;
			}
		}

		return C;
	}

	Matrix& Transform() {
		int t = rows; rows = cols; cols = t;
		return *this;
	}
public:
	void ForEach(void(*callback)(int, int, T&)) {
		for (int i = 0, k = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++, k++) {
				callback(i, j, Array<T>::operator[](k));
			}
		}
	}
};
