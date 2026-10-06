#pragma once
#include <initializer_list>
#include <cstdlib>      // дл€ рандома
#include <ctime>
#include <string>       // дл€ std::getline
#include <sstream>      // дл€ std::istringstream

#define MEM_STEP 16     //шаг: сколько выдел€етс€ €чеек пам€ти минимум при выделении пам€ти

template <class T>
class MemData {
    T* data_;
    size_t capacity_;
public:
	MemData(size_t size = 0, const T* data = nullptr);
	MemData(std::initializer_list<T> list);
	MemData(const MemData<T>&) = delete;
    MemData(MemData<T>&&) noexcept;
    ~MemData();

    size_t capacity() const noexcept {
        return capacity_;
    }
    const T* data() const noexcept {
        return data_;
    }
    T* data() noexcept { 
        return data_;
    }
	T& operator[](size_t index) noexcept {
		return data_[index];
	}
	const T& operator[](size_t index) const noexcept {
		return data_[index];
	}
	T& at(size_t index);
	const T& at(size_t index) const;

	void allocateRaw(size_t capacity); // перевыделение пам€ти без сохранени€ данных (возможно уменьшение capasity)
	void clear() noexcept; // освобождение пам€ти
	void realloc(size_t capacity, size_t start, size_t size); // перевыделение пам€ти с сохранением данных в диапазоне [start, start + size)
	void shiftRight(size_t start, size_t count);  // сдвиг элементов вправо
	void shiftLeft(size_t start, size_t count);   // сдвиг элементов влево
	void reallocWithShiftRight(size_t newCapacity, size_t start, size_t count);

	size_t calculateCapacity_(size_t capacity); // вычисление нового размера пам€ти с учетом MEM_STEP

	MemData<T>& operator=(const MemData<T>&) = delete;
	MemData<T>& operator=(MemData<T>&&) noexcept;
};

template <class T>
size_t MemData<T>::calculateCapacity_(size_t capacity) {
	return ((capacity / MEM_STEP) + 1) * MEM_STEP;
}

template<class T>
MemData<T>::MemData(size_t size, const T* array) : capacity_(0), data_(nullptr)
{
	allocateRaw(calculateCapacity_(size));
	for (size_t i = 0; i < size; ++i) {
		(*this)[i] = array ? array[i] : T();
	}
}

template<class T>
MemData<T>::MemData(std::initializer_list<T> list) : capacity_(0), data_(nullptr)
{
	allocateRaw(calculateCapacity_(list.size()));
	auto it = list.begin();
	for (size_t i = 0; i < list.size(); ++i) {
		data_[i] = *(it + i);
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
T& MemData<T>::at(size_t index) {
	if (index >= capacity_ || index < 0) {
		throw std::out_of_range("ERROR: Index out of range!");
	}
	return data_[index];
}

template<class T>
const T& MemData<T>::at(size_t index) const {
	if (index >= capacity_ || index < 0) {
		throw std::out_of_range("ERROR: Index out of range!");
	}
	return data_[index];
}

template<class T>
void MemData<T>::allocateRaw(size_t capacity)
{
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
void MemData<T>::realloc(size_t capacity, size_t start, size_t count)
{
	if (start + count > capacity_) {
		throw std::out_of_range("ERROR: Reallocate with shift right out of range!");
	}

	T* newData = nullptr;
	try {
		newData = new T[capacity];
	}
	catch (const std::bad_alloc&) {
		throw std::bad_alloc();
	}
	for (size_t i = start; i < count + start; ++i) {
		newData[i] = std::move(data_[i]);
	}
	clear();
	data_ = newData;
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
void MemData<T>::reallocWithShiftRight(size_t newCapacity, size_t start, size_t count)
{
	if (start + count > capacity_) {
		throw std::out_of_range("ERROR: Reallocate with shift right out of range!");
	}

	T* newData = nullptr;
	try {
		newData = new T[newCapacity];
	}
	catch (const std::bad_alloc&) {
		throw std::bad_alloc();
	}

	size_t i = 0;
	//  опируем до start
	for (; i < start; ++i) {
		newData[i] = std::move(data_[i]);
	}
	//  опируем со сдвигом на 1 элемент вправо, чтобы освободить место дл€ вставки
	for (; i < start + count; ++i) {
		newData[i+1] = std::move(data_[i]);
	}

	delete[] data_;
	data_ = newData;
	capacity_ = newCapacity;
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
