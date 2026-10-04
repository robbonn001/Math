#pragma once
#include <iostream>
#include <cstdlib>
#include "memdata.h"

template<class T>
class Vector {
	MemData<T> storage_;
	size_t front_;  // индекс первого элемента
	size_t back_;   // индекс ПОСЛЕ последнего элемента

public:
	Vector(size_t size = 0, const T* array = nullptr);
	Vector(std::initializer_list<T>);
	Vector(const Vector<T>&); 
	Vector(Vector<T>&&) noexcept;
	~Vector() = default;

	template <class Type>
	class Iterator {
		Type* cur;
	public:
		Iterator(Type* _cur = nullptr) :cur(_cur) {}
		Iterator(const Iterator& other) :cur(other.cur) {}
		bool operator==(const Iterator<Type>& other) const noexcept {
			return cur == other.cur;
		}
		bool operator!=(const Iterator<Type>& other) const noexcept {
			return !((*this) == other);
		}
		Iterator<Type>& operator++() {
			cur++;
			return (*this);
		}
		Iterator<Type> operator++(int) {
			return Iterator<Type>(cur++);
		}
		Iterator<Type>& operator--() {
			cur--;
			return (*this);
		}
		Iterator<Type> operator--(int) {
			return Iterator<Type>(cur--);
		}
		T& operator*() {
			return (*cur);
		}
		const T& operator*() const {
			return (*cur);
		}
		void operator=(const Iterator<Type>& it) {
			cur = it.cur;
		}
		void operator+= (const Iterator<Type>& it) {
			cur += it.cur;
		}
		void operator-= (const Iterator<Type>& it) {
			cur -= it.cur;
		}
		Iterator<Type> operator+ (const Iterator<Type>& it) {
			return Iterator<Type>((*this) += it);
		}
		Iterator<Type> operator- (const Iterator<Type>& it) {
			return Iterator<Type>((*this) -= it);
		}
	};

	template<class Type> class Iterator;
	typedef Iterator<T> iterator;

	template<class Type> class Iterator;
	typedef Iterator<const T> const_iterator;

	iterator begin() noexcept {	return iterator(&storage_[front_]);}
	iterator end() noexcept { return iterator(&storage_[back_]);}
	const_iterator begin() const noexcept { return iterator(&storage_[front_]); }
	const_iterator end() const noexcept { return iterator(&storage_[back_]); }

	bool isEmpty() const noexcept {
		return front_ == back_;
	}
	bool isFull() const noexcept {
		return back_ == storage_.getCapacity();
	}
	size_t getSize() const noexcept {
		return back_ - front_;
	}
	size_t getCapacity() const noexcept {
		return storage_.getCapacity();
	}

	void resize(size_t new_size);
	void defragment() noexcept;
	void reserve(size_t new_capacity) {
		storage_.reserve(new_capacity);
	}

	//MemData<T>& getData() noexcept {
	//	return storage_;
	//}
	//const MemData<T>& getData() const noexcept {
	//	return storage_;
	//}
	
	void pushFront(const T&) noexcept;
	void pushFrontMany(const T*, size_t) noexcept;
	void pushBack(const T&) noexcept;
	void pushBackMany(const T*, size_t) noexcept;
	void insert(const T&, size_t);
	void insertMany(const T*, size_t, size_t);

	void popFront();
	void popFrontMany(size_t);
	void popBack(); 
	void popBackMany(size_t);
	void erase(size_t);
	void eraseMany(size_t, size_t);

	Vector<T>& operator=(const Vector<T>&) noexcept;
	Vector<T>& operator=(Vector<T>&&) noexcept;
	T& operator[](size_t) noexcept;
	const T& operator[](size_t) const noexcept;

private:
	//служебный метод получения (геттер) физического индекса по относительному
	size_t getDataIndex(size_t i) const noexcept {
		return i + front_;
	}
};

template<class Type>
std::ostream& operator<< (std::ostream& out, const Vector<Type>& vector) {
	size_t size = vector.getSize();
	for (size_t i = 0; i < size; i++) {
		out << vector[i] << ' ';
	}
	return out;
}

template<class Type>
std::istream& operator>> (std::istream& in, Vector<Type>& vector) {
	Type element;
	while (in >> element) {
		vector.pushBack(element);
	}
	return in;
}

template<class T>
Vector<T>::Vector(size_t size, const T* array) :storage_(size, array), front_(0), back_(size) {}

template<class T>
Vector<T>::Vector(std::initializer_list<T> list) :storage_(list), front_(0), back_(list.size()) {}

template<class T>
Vector<T>::Vector(const Vector<T>& other): storage_(other.storage_), front_(other.front_), back_(other.back_) {}

template<class T>
Vector<T>::Vector(Vector<T>&& other) noexcept : storage_(std::move(other.storage_)), front_(other.front_), back_(other.back_) {
	other.front_ = 0;
	other.back_ = 0;
}

template<class T>
void Vector<T>::resize(size_t new_size) {

	size_t needed = front_ + new_size;
	if (new_size <= getSize()) {
		back_ = needed;
		return;
	}

	size_t current_capacity = getCapacity();

	if (needed > current_capacity) {
		// Если front большой — сначала дефрагментируем
		if (front_ > current_capacity / 2) {
			defragment(); // front_ = 0, back_ = getSize()
			storage_.reserve(storage_.calculateCapacity_(new_size));
		}
	}

	back_ = front_ + new_size;
}

template<class T>
void Vector<T>::defragment() noexcept
{
	if (front_ == 0) return;  // уже в начале

	size_t current_size = getSize();

	// Сдвигаем элементы в начало
	for (size_t i = 0; i < current_size; ++i) {
		storage_[i] = std::move(storage_[getDataIndex(i)]);
	}

	front_ = 0;
	back_ = current_size;
}

template<class T>
void Vector<T>::pushFront(const T& element) noexcept {
	if (front_ > 0) {
		--front_;
	}
	else if (isFull()) {
		storage_.reallocateWithShiftRight(storage_.calculateCapacity_(getSize() + 1), 0);
		back_++;
	}
	else { // Если front_ == 0, но вектор не полный, сдвигаем элементы вправо
		storage_.shiftRight(0, getSize());
		back_++;
	}
	storage_[front_] = element;
}

template<class T>
void Vector<T>::pushFrontMany(const T* elements, size_t size) noexcept {
	for (int i = size - 1; i >= 0; i--) { //идем с конца т.к. кладем в начало
		pushFront(elements[i]);
	}
}
template<class T>
void Vector<T>::pushBack(const T& element) noexcept {
	if (isFull()) {
		storage_.realloc(storage_.calculateCapacity_(getSize() + 1), back_);
	}
	storage_[back_++] = element;
}
template<class T>
void Vector<T>::pushBackMany(const T* elements, size_t size) noexcept {
	for (int i = 0; i < size; i++) { //идем с начала т.к. кладем в конец
		pushBack(elements[i]);
	}
}
template<class T>
void Vector<T>::insert(const T& element, size_t index) {
	size_t validIndex = getDataIndex(index);
	if (validIndex > getSize()|| validIndex < 0) {
		throw std::out_of_range("ERROR: Insert index out of range!");
	}
	if (validIndex == 0) {
		pushFront(element);
	}
	else if (validIndex == getSize()) {
		pushBack(element);
	}
	else {
		if (isFull()) {
			storage_.reallocateWithShiftRight(storage_.calculateCapacity_(getSize() + 1), validIndex);
			front_ = 0;
		}
		else {
			storage_.shiftRight(validIndex, back_ - validIndex);
		}
		++back_;
		storage_[validIndex] = element;
	}
}

template<class T>
void Vector<T>::insertMany(const T* elements, size_t size, size_t index) {
	size_t validIndex = getDataIndex(index);
	if (validIndex > getSize()) {
		throw std::out_of_range("ERROR: Insert (many) index out of range!");
	}
	for (int j = 0; j < size; j++) {
		insert(elements[j], validIndex + j);
	}
}

template<class T>
void Vector<T>::popFront() {
	if (getSize() == 0) {
		throw std::logic_error("ERROR: Empty vector! Can't pop front");
	}
	++front_;
}
template<class T>
void Vector<T>::popFrontMany(size_t count) {
	if (getSize() < count) {
		throw std::logic_error("ERROR: Popping (front) too many elements!");
	}
	for (size_t i = 0; i < count; ++i) {
		popFront();
	}
}
template<class T>
void Vector<T>::popBack() {
	if (getSize() == 0) {
		throw std::logic_error("ERROR: Empty vector! Can't pop back");
	}
	--back_;
}
template<class T>
void Vector<T>::popBackMany(size_t count) {
	if (getSize() < count) {
		throw std::logic_error("ERROR: Popping (back) too many elements!");
	}
	for (size_t i = 0; i < count; ++i) {
		popBack();
	}
}
template<class T>
void Vector<T>::erase(size_t index) {
	size_t validIndex = getDataIndex(index);
	if (validIndex >= getSize() || validIndex < 0) {
		throw std::out_of_range("ERROR: Erase index out of range!");
	}
	if (validIndex == 0) {
		popFront();
	}
	else if (validIndex == getSize() - 1) {
		popBack();
	}
	else {
		storage_.shiftLeft(validIndex, back_ - validIndex);
		--back_;
	}
}
template<class T>
void Vector<T>::eraseMany(size_t i, size_t count) {
	if (i + count > storage_.size_) {
		throw std::logic_error("ERROR: Erasing too many elements!");
	}
	for (int j = 0; j < count; j++) {
		erase(i);
	}
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) noexcept {
	if (this != &other) {
		storage_ = other.storage_;
		front_ = other.front_;
		back_ = other.back_;
	}
	return (*this);
}
template<class T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
	if (this != &other) {
		storage_ = std::move(other.storage_);
		front_ = other.front_;
		other.front_ = 0;
		back_ = other.back_;
		other.back_ = 0;
	}
	return (*this);
}

template<class T>
const T& Vector<T>::operator[](size_t i) const noexcept {
	return storage_[getDataIndex(i)];
}

template<class T>
T& Vector<T>::operator[](size_t i) noexcept {
	return storage_[getDataIndex(i)];
}


