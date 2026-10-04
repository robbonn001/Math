#pragma once
#include "Vector.h"

template <class T>
class MathVector : public Vector<T> {
private:
	size_t _start_index;
public:
	MathVector(size_t size = 0, const T* array = nullptr);
	MathVector(std::initializer_list<T> data);
	MathVector(const MathVector<T>& other);
	~MathVector() = default;
	inline size_t size()const noexcept {
		return this->Vector<T>::getSize();
	}

	MathVector<T> operator* (double value)const noexcept;
	MathVector<T>& operator*=(double value)noexcept;

	MathVector<T> operator+ (const MathVector<T>& other);
	MathVector<T> operator- (const MathVector<T>& other);
	double operator* (const MathVector<T>& other);

	MathVector<T>& operator+=(const MathVector<T>& other);
	MathVector<T>& operator-=(const MathVector<T>& other);
	MathVector<T>& operator=(const MathVector<T>& other);

	const T& operator[](size_t index)const;
	T& operator[](size_t index);

	bool operator==(MathVector<T>& other)const;
	bool operator!=(MathVector<T>& other)const;
};

template <class T>
MathVector<T>::MathVector(size_t size, const T* array) : Vector<T>(size, array), _start_index(0) {
	this->reserve(size);
}

template <class T>
MathVector<T>::MathVector(std::initializer_list<T> data) : Vector<T>(data), _start_index(0) {
	this->reserve(data.size());
}

template <class T>
MathVector<T>::MathVector(const MathVector<T>& other) : Vector<T>(other), _start_index(0) {
	this->reserve(other.getSize());
}

template <class T>
MathVector<T> MathVector<T>::operator*(double value)const noexcept {
	MathVector<T> res(*this);
	res *= value;
	return res;
}

template <class T>
MathVector<T>& MathVector<T>::operator*=(double value)noexcept {
	for (int i = 0;i < this->getSize();++i) {
		(*this)[i] *= value;
	}
	return (*this);
}

template <class T>
MathVector<T> MathVector<T>::operator+(const MathVector<T>& other) {
	MathVector<T> res(*this);
	for (int i = 0;i < this->size();++i) {
		res[i] = (*this)[i] + other[i];
	}
	return res;
}

template <class T>
MathVector<T> MathVector<T>::operator-(const MathVector<T>& other) {
	MathVector<T> res(*this);
	for (int i = 0;i < this->size();++i) {
		res[i] = (*this)[i] - other[i];
	}
	return res;
}

template <class T>
double MathVector<T>::operator* (const MathVector<T>& other) {
	double res = 0.0;
	for (int i = 0;i < this->size();++i) {
		res += (*this)[i] * other[i];
	}
	return res;
}

template <class T>
MathVector<T>& MathVector<T>::operator+=(const MathVector<T>& other) {
	(*this) = (*this) + other;
	return *this;
}

template <class T>
MathVector<T>& MathVector<T>::operator-=(const MathVector<T>& other) {
	(*this) = (*this) - other;
	return *this;
}

template <class T>
MathVector<T>& MathVector<T>::operator=(const MathVector<T>& other) {
	if (&other != this) {
		(*this).Vector<T>::operator=(other);
		_start_index = other._start_index;
	}
	return (*this);
}

template <class T>
const T& MathVector<T>::operator[](size_t index)const {
	return (*this).Vector<T>::operator[](index);
}

template <class T>
T& MathVector<T>::operator[](size_t index) {
	return (*this).Vector<T>::operator[](index);
}

template <class T>
bool MathVector<T>::operator==(MathVector<T>& other)const {
	return ((*this).Vector<T>::operator==(other) && _start_index == other._start_index);
}

template <class T>
bool MathVector<T>::operator!=(MathVector<T>& other)const {
	return !((*this) == other);
}