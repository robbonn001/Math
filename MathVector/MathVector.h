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
	MathVector(MathVector<T>&& other) noexcept;
	~MathVector() = default;

	MathVector<T> operator* (T value) const noexcept;
	MathVector<T>& operator*=(T value) noexcept;

	MathVector<T> operator+ (const MathVector<T>& other) const;
	MathVector<T> operator- (const MathVector<T>& other) const;
	T operator* (const MathVector<T>& other) const;

	MathVector<T>& operator+=(const MathVector<T>& other);
	MathVector<T>& operator-=(const MathVector<T>& other);
	MathVector<T>& operator=(const MathVector<T>& other);
	MathVector<T>& operator=(MathVector<T>&& other) noexcept;

	bool operator==(const MathVector<T>& other) const noexcept;
	bool operator!=(const MathVector<T>& other) const noexcept;
};

template <class T>
MathVector<T>::MathVector(size_t size, const T* array) : Vector<T>(size, array), _start_index(0) {
	this->realloc(size);
}

template <class T>
MathVector<T>::MathVector(std::initializer_list<T> data) : Vector<T>(data), _start_index(0) {
	this->realloc(data.size());
}

template <class T>
MathVector<T>::MathVector(const MathVector<T>& other) : Vector<T>(other), _start_index(0) {
	this->realloc(other.size());
}

template<class T>
MathVector<T>::MathVector(MathVector<T>&& other) noexcept: Vector<T>(std::move(other)), _start_index(other._start_index) {
	other._start_index = 0;
}

template <class T>
MathVector<T> MathVector<T>::operator*(T value)const noexcept {
	MathVector<T> res(*this);
	res *= value;
	return res;
}

template <class T>
MathVector<T>& MathVector<T>::operator*=(T value)noexcept {
	for (int i = 0;i < this->size();++i) {
		(*this)[i] *= value;
	}
	return (*this);
}

template <class T>
MathVector<T> MathVector<T>::operator+(const MathVector<T>& other) const {
	if (this->size() != other.size()) {
		throw std::logic_error("ERROR: Vectors must be of the same size for addition!");
	}
	MathVector<T> res(*this);
	return res += other;
}

template <class T>
MathVector<T> MathVector<T>::operator-(const MathVector<T>& other) const {
	if (this->size() != other.size()) {
		throw std::logic_error("ERROR: Vectors must be of the same size for subtraction!");
	}
	MathVector<T> res(*this);
	return res -= other;
}

template <class T>
T MathVector<T>::operator* (const MathVector<T>& other) const {
	if (this->size() != other.size()) {
		throw std::logic_error("ERROR: Vectors must be of the same size for dot product!");
	}
	T res = T(0);
	for (int i = 0;i < this->size();++i) {
		res += (*this)[i] * other[i];
	}
	return res;
}

template <class T>
MathVector<T>& MathVector<T>::operator+=(const MathVector<T>& other) {
	if (this->size() != other.size()) {
		throw std::logic_error("ERROR: Vectors must be of the same size for dot product!");
	}
	for (int i = 0;i < this->size();++i) {
		(*this)[i] += other[i];
	}
	return *this;
}

template <class T>
MathVector<T>& MathVector<T>::operator-=(const MathVector<T>& other) {
	if (this->size() != other.size()) {
		throw std::logic_error("ERROR: Vectors must be of the same size for dot product!");
	}
	for (int i = 0;i < this->size();++i) {
		(*this)[i] -= other[i];
	}
	return *this;
}

template <class T>
MathVector<T>& MathVector<T>::operator=(const MathVector<T>& other) {
	if (&other != this) {
		Vector<T>::operator=(other);
		_start_index = other._start_index;
	}
	return (*this);
}

template <class T>
MathVector<T>& MathVector<T>::operator=(MathVector<T>&& other) noexcept {
	if (&other != this) {
		Vector<T>::operator=(std::move(other));
		_start_index = other._start_index;
		other._start_index = 0;
	}
	return (*this);
}

template <class T>
bool MathVector<T>::operator==(const MathVector<T>& other) const noexcept {
	return (Vector<T>::operator==(other) && _start_index == other._start_index);
}

template <class T>
bool MathVector<T>::operator!=(const MathVector<T>& other) const noexcept {
	return !((*this) == other);
}