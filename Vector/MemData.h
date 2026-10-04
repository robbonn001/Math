#pragma once
#include <initializer_list>
#include <cstdlib>      // для рандома
#include <ctime>
#include <string>       // для std::getline
#include <sstream>      // для std::istringstream

#define MEM_STEP 16     //шаг: сколько выделяется ячеек памяти минимум при выделении памяти

template<class T>
class Vector;

template <class T>
class MemData {
    T* data_;
    size_t capacity_;
public:
	MemData(size_t capacity = 0, const T* data = nullptr);
	MemData(std::initializer_list<T> list);
    MemData(const MemData<T>&);
    MemData(MemData<T>&&) noexcept;
    ~MemData();

    size_t getCapacity() const noexcept {
        return capacity_;
    }
    const T* getData() const noexcept {
        return data_;
    }
    T* getData() noexcept { 
        return data_;
    }
	T& operator[](size_t index) noexcept {
		return data_[index];
	}
	const T& operator[](size_t index) const noexcept {
		return data_[index];
	}
	T& at(size_t index) noexcept;
	const T& at(size_t index) const noexcept;

	void reserve(size_t capacity); // перевыделение памяти без сохранения данных
	void clear() noexcept;
	void realloc(size_t capacity, size_t size = 0) noexcept; // перевыделение памяти с сохранением данных
	void shiftRight(size_t start, size_t count);  // сдвиг элементов вправо
	void shiftLeft(size_t start, size_t count);   // сдвиг элементов влево
	void reallocateWithShiftRight(size_t newCapacity, size_t start);

	int calculateCapacity_(size_t capacity);

    MemData<T>& operator=(const MemData<T>&) noexcept;
	MemData<T>& operator=(MemData<T>&&) noexcept;

	friend class Vector<T>;
};

template <class T>
int MemData<T>::calculateCapacity_(size_t capacity) {
	return (capacity / MEM_STEP + 1) * MEM_STEP;
}

template<class T>
MemData<T>::MemData(size_t size, const T* data)
{
	reserve(calculateCapacity_(size));
	for (size_t i = 0; i < size; ++i) {
		data_[i] = data ? data[i] : T();
	}
}

template<class T>
MemData<T>::MemData(std::initializer_list<T> list)
{
	reserve(calculateCapacity_(list.size()));
	auto it = list.begin();
	for (size_t i = 0; i < list.size(); ++i) {
		data_[i] = *(it + i);
	}
}

template<class T>
MemData<T>::MemData(const MemData<T>& other) : capacity_(other.capacity_), data_(nullptr) {
	if (other.data_) {
		data_ = new T[capacity_];
		for (size_t i = 0; i < capacity_; ++i) {
			data_[i] = other.data_[i];
		}
	}
}

template <class T>
MemData<T>::MemData(MemData<T>&& other) noexcept : capacity_(other.capacity_), data_(other.data_) {
	other.capacity_ = 0;
	other.data_ = nullptr;
}

template <class T>
MemData<T>::~MemData() {
	clear();
}

template<class T>
T& MemData<T>::at(size_t index) noexcept {
	if (index >= capacity_ || index < 0) {
		throw std::out_of_range("ERROR: Index out of range!");
	}
	return data_[index];
}

template<class T>
const T& MemData<T>::at(size_t index) const noexcept {
	if (index >= capacity_ || index < 0) {
		throw std::out_of_range("ERROR: Index out of range!");
	}
	return data_[index];
}

template<class T>
void MemData<T>::reserve(size_t capacity)
{
	if (capacity <= capacity_) {
		return;
	}
	capacity_ = capacity;
	T* newData = nullptr;
	try {
		newData = new T[capacity_];
	}
	catch (const std::bad_alloc&) {
		throw std::bad_alloc();
	}
	if (data_) {
		delete[] data_;
	}
	data_ = newData;
}

template<class T>
void MemData<T>::clear() noexcept {
	capacity_ = 0;
	if (data_) {
		delete[] data_;
		data_ = nullptr;
	}
}

template<class T>
void MemData<T>::realloc(size_t capacity, size_t size) noexcept
{
	T* data = new T[capacity];
	size_t end = size ? size : capacity_;
	for (size_t i = 0; i < end; ++i) {
		data[i] = std::move(data_[i]);
	}
	clear();
	data_ = data;
	capacity_ = capacity;
}

template<class T>
void MemData<T>::shiftRight(size_t start, size_t count)
{
	if (start + count > capacity_) {
		throw std::out_of_range("ERROR: Shift right out of range!");
	}
	for (size_t i = start + count; i > start; --i) {
		data_[i] = std::move(data_[i - 1]);
	}
}

template<class T>
void MemData<T>::shiftLeft(size_t start, size_t count)
{
	if (start + count > capacity_) {
		throw std::out_of_range("ERROR: Shift left out of range!");
	}
	for (size_t i = start; i < start + count; ++i) {
		data_[i] = std::move(data_[i + 1]);
	}
}

template<class T>
void MemData<T>::reallocateWithShiftRight(size_t newCapacity, size_t start)
{
	T* newData = new T[newCapacity];

	size_t i = 0;
	for (; i < start; ++i) {
		newData[i] = std::move(data_[i]);
	}
	// Копируем со сдвигом
	for (; i < capacity_; ++i) {
		newData[i+1] = std::move(data_[i]);
	}

	delete[] data_;
	data_ = newData;
	capacity_ = newCapacity;
}

template<class T>
MemData<T>& MemData<T>::operator=(const MemData<T>& other) noexcept
{
	if (this != &other) {
		capacity_ = other.capacity_;
		delete[] data_;
		data_ = new T[capacity_];
		for (size_t i = 0; i < capacity_; ++i) {
			data_[i] = other.data_[i];
		}
	}
	return *this;
}

template <class T>
MemData<T>& MemData<T>::operator=(MemData<T>&& other) noexcept {
	if (this != &other) {
		capacity_ = other.capacity_;
		delete[] data_;
		data_ = other.data_;
		other.capacity_ = 0;
		other.data_ = nullptr;
	}
	return *this;
}
