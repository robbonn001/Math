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
	Vector(const std::initializer_list<T> list);
	Vector(const Vector<T>&); 
	Vector(Vector<T>&&) noexcept;
	~Vector() = default;

	template <class Type>
	class Iterator {
		Type* current_;
	public:
		Iterator(Type* current = nullptr) :current_(current) {}
		Iterator(const Iterator& other) :current_(other.current_) {}
		Iterator<Type>& operator++() {
			++current_;
			return (*this);
		}
		Iterator<Type> operator++(int) {
			return Iterator<Type>(current_++);
		}
		Iterator<Type>& operator--() {
			--current_;
			return (*this);
		}
		Iterator<Type> operator--(int) {
			return Iterator<Type>(current_--);
		}
		Type& operator*() {
			return (*current_);
		}
		Type* operator->() {
			return current_;
		}
		const Type& operator*() const {
			return (*current_);
		}
		Iterator<Type>& operator=(const Iterator<Type>& it) {
			current_ = it.current_;
			return (*this);
		}
		Iterator<Type>& operator+= (size_t offset) {
			current_ += offset;
			return (*this);
		}
		Iterator<Type>& operator-= (size_t offset) {
			current_ -= offset;
			return (*this);
		}
		Iterator<Type> operator+ (size_t offset) {
			return Iterator<Type>(current_ + offset);
		}
		Iterator<Type> operator- (size_t offset) {
			return Iterator<Type>(current_ - offset);
		}

		bool operator==(const Iterator<Type>& other) const noexcept { return current_ == other.current_; }
		bool operator!=(const Iterator<Type>& other) const noexcept { return current_ != other.current_; }
		bool operator<(const Iterator<Type>& other) const noexcept { return current_ < other.current_; }
		bool operator<=(const Iterator<Type>& other) const noexcept { return current_ <= other.current_; }
		bool operator>(const Iterator<Type>& other) const noexcept { return current_ > other.current_; }
		bool operator>=(const Iterator<Type>& other) const noexcept { return current_ >= other.current_; }
	 };

	template<class Type> class Iterator;
	typedef Iterator<T> iterator;

	template<class Type> class Iterator;
	typedef Iterator<const T> const_iterator;

	const_iterator cbegin() const noexcept { return const_iterator(&storage_[front_]); }
	const_iterator cend() const noexcept { return const_iterator(&storage_[back_]); }
	iterator begin() noexcept {	return iterator(&storage_[front_]); }
	iterator end() noexcept { return iterator(&storage_[back_]); }

	bool isEmpty() const noexcept {
		return front_ == back_;
	}
	bool isFull() const noexcept {
		return back_ == storage_.capacity();
	}
	size_t size() const noexcept {
		return back_ - front_;
	}
	size_t capacity() const noexcept {
		return storage_.capacity();
	}

	T& front();
	const T& front() const;
	T& back();
	const T& back() const;

	MemData<T>& data() noexcept {
		return storage_;
	}
	const MemData<T>& data() const noexcept {
		return storage_;
	}

	void resize(size_t new_size);
	void defragment() noexcept;
	void realloc(size_t new_capacity) { // перевыделение памяти с сохранением данных
		storage_.realloc(new_capacity, front_, size());
	}

	void clear() noexcept {
		front_ = 0;
		back_ = 0;
		storage_.clear();
	}

	void pushFront(const T&);
	void pushFrontMany(const T*, size_t);
	void pushBack(const T&);
	void pushBackMany(const T*, size_t);
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
	bool operator==(const Vector<T>&) const noexcept;
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
	size_t size = vector.size();
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
Vector<T>::Vector(const std::initializer_list<T> list) :storage_(list), front_(0), back_(list.size()) {}

template<class T>
Vector<T>::Vector(const Vector<T>& other): storage_(other.size()), front_(other.front_), back_(other.back_) {
	for (size_t i = 0; i < size(); ++i) {
		(*this)[i] = other[i]; // копируем элементы, учитывая смещение front_
	}
}

template<class T>
Vector<T>::Vector(Vector<T>&& other) noexcept : storage_(std::move(other.storage_)), front_(other.front_), back_(other.back_) {
	other.front_ = 0;
	other.back_ = 0;
}

template<class T>
T& Vector<T>::front()
{
	if (isEmpty()) {
		throw std::logic_error("ERROR: Empty vector! No front element.");
	}
	return storage_[front_];
}

template<class T>
const T& Vector<T>::front() const
{
	if (isEmpty()) {
		throw std::logic_error("ERROR: Empty vector! No front element.");
	}
	return storage_[front_];
}

template<class T>
T& Vector<T>::back()
{
	if (isEmpty()) {
		throw std::logic_error("ERROR: Empty vector! No back element.");
	}
	return storage_[back_ - 1];
}

template<class T>
const T& Vector<T>::back() const
{
	if (isEmpty()) {
		throw std::logic_error("ERROR: Empty vector! No back element.");
	}
	return storage_[back_ - 1];
}

template<class T>
void Vector<T>::resize(size_t new_size) {

	size_t needed = front_ + new_size;
	if (new_size <= size()) {
		back_ = needed;
		return;
	}

	size_t current_capacity = capacity();

	if (needed > current_capacity) {
		// Если front большой — сначала дефрагментируем
		if (front_ > current_capacity / 2) {
			defragment(); // front_ = 0, back_ = size()
		}
		storage_.realloc(storage_.calculateCapacity_(new_size), 0, size());
	}

	back_ = front_ + new_size;
}

template<class T>
void Vector<T>::defragment() noexcept
{
	if (front_ == 0) return;  // уже в начале

	size_t current_size = size();

	// Сдвигаем элементы в начало
	for (size_t i = 0; i < current_size; ++i) {
		storage_[i] = std::move(storage_[getDataIndex(i)]);
	}

	front_ = 0;
	back_ = current_size;
}

template<class T>
void Vector<T>::pushFront(const T& element) {
	if (front_ > 0) {
		--front_;
	}
	else if (isFull()) {
		storage_.reallocWithShiftRight(storage_.calculateCapacity_(size() + 1), 0, size());
		back_++;
	}
	else { // Если front_ == 0, но вектор не полный, сдвигаем элементы вправо
		storage_.shiftRight(0, size());
		back_++;
	}
	storage_[front_] = element;
}

template<class T>
void Vector<T>::pushFrontMany(const T* elements, size_t size) {
	for (int i = size - 1; i >= 0; i--) { //идем с конца т.к. кладем в начало
		pushFront(elements[i]);
	}
}
template<class T>
void Vector<T>::pushBack(const T& element) {
	if (isFull()) {
		storage_.realloc(storage_.calculateCapacity_(size() + 1), front_, size());
	}
	storage_[back_++] = element;
}
template<class T>
void Vector<T>::pushBackMany(const T* elements, size_t size) {
	for (int i = 0; i < size; i++) { //идем с начала т.к. кладем в конец
		pushBack(elements[i]);
	}
}
template<class T>
void Vector<T>::insert(const T& element, size_t index) {
	size_t validIndex = getDataIndex(index);
	if (validIndex > size()) {
		throw std::out_of_range("ERROR: Insert index out of range!");
	}
	if (validIndex == 0) {
		pushFront(element);
	}
	else if (validIndex == size()) {
		pushBack(element);
	}
	else {
		if (isFull()) {
			storage_.reallocWithShiftRight(storage_.calculateCapacity_(size() + 1), validIndex, back_ - validIndex);
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
	if (index > this->size()) {
		throw std::out_of_range("ERROR: Insert (many) index out of range!");
	}
	for (int j = 0; j < size; j++) {
		insert(elements[j], index + j);
	}
}

template<class T>
void Vector<T>::popFront() {
	if (size() == 0) {
		throw std::logic_error("ERROR: Empty vector! Can't pop front");
	}
	++front_;
}
template<class T>
void Vector<T>::popFrontMany(size_t count) {
	if (size() < count) {
		throw std::logic_error("ERROR: Popping (front) too many elements!");
	}
	for (size_t i = 0; i < count; ++i) {
		popFront();
	}
}
template<class T>
void Vector<T>::popBack() {
	if (size() == 0) {
		throw std::logic_error("ERROR: Empty vector! Can't pop back");
	}
	--back_;
}
template<class T>
void Vector<T>::popBackMany(size_t count) {
	if (size() < count) {
		throw std::logic_error("ERROR: Popping (back) too many elements!");
	}
	for (size_t i = 0; i < count; ++i) {
		popBack();
	}
}
template<class T>
void Vector<T>::erase(size_t index) {
	size_t validIndex = getDataIndex(index);
	if (validIndex >= size() || validIndex < 0) {
		throw std::out_of_range("ERROR: Erase index out of range!");
	}
	if (validIndex == 0) {
		popFront();
	}
	else if (validIndex == size() - 1) {
		popBack();
	}
	else {
		storage_.shiftLeft(validIndex, back_ - validIndex);
		--back_;
	}
}
template<class T>
void Vector<T>::eraseMany(size_t index, size_t count) {
	if (index + count > size()) {
		throw std::logic_error("ERROR: Erasing too many elements!");
	}
	for (int j = 0; j < count; j++) {
		erase(index + j);
	}
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) noexcept { 
	if (this != &other) {
		front_ = other.front_;
		back_ = other.back_;
		storage_.allocateRaw(other.capacity()); // резервируем память под элементы и удаляем старые
		for (size_t i = 0; i < size(); ++i) {
			(*this)[i] = other[i]; // копируем элементы, учитывая смещение front_
		}
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
bool Vector<T>::operator==(const Vector<T>& other) const noexcept
{
	if (size() != other.size()) return false;
	for (size_t i = 0; i < size(); ++i) {
		if ((*this)[i] != other[i]) return false;
	}
	return true;
}

template<class T>
const T& Vector<T>::operator[](size_t i) const noexcept {
	return storage_[getDataIndex(i)];
}

template<class T>
T& Vector<T>::operator[](size_t i) noexcept {
	return storage_[getDataIndex(i)];
}


