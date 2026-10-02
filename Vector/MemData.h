#pragma once
#include <initializer_list>
#include <iostream>
#include <cstdlib>      // для рандома
#include <ctime>
#include <string>       // для std::getline
#include <sstream>      // для std::istringstream

#define MEM_STEP 15     //шаг: сколько выделяется ячеек памяти минимум при добавлении эл-тов

template <typename T>
class Vector;

template <typename T>
class MemData;

template <typename T> //<----иначе не компилируется
void quick_sort(MemData<T>& md);            // сортировки
template <typename T>
void shuffle(MemData<T>&);                  // перемешивания

template <typename T>
class MemData {
    T* _data;        // хранилище данных
    size_t _size;              // размер заполненной части хранилища
    size_t _capacity;          // вместимость хранилища

public:
    //конструкторы
    MemData(size_t size = 0);                       // по размеру + по умолчанию
    MemData(std::initializer_list<T>);    // по списку инициализации
    MemData(const T*, size_t);            // инициализации
    MemData(const MemData&);                        // копирования
    MemData(MemData&&) noexcept;                    // с move-семантикой
    //деструктор
    ~MemData();

    //публичные методы проверок
    bool is_empty() const noexcept {         // на пустоту
        return (_size == 0);
    }

    //геттеры
    size_t get_size() const noexcept {                           // размера
        return _size;
    }
    size_t get_capacity() const noexcept {                       // вместимости
        return _capacity;
    }
    const T* const get_data_const() const noexcept {   // хранилища (возвр указ-ль по которому НЕЛЬЗЯ менять)
        return _data;
    }
    inline T* const get_data_changeable() noexcept {          // хранилища (возвр указ-ль по которому МОЖНО менять)
        return _data;
    }

    //сеттеры памяти и размера заполненной части
    void set_memory(size_t) noexcept;                           // установка памяти без сохранения данных
    void reset_memory(size_t size, size_t start_index = 0);     // перевыделение памяти с сохранением данных (УБРАН NOEXCEPT)
    void clear_memory() noexcept;                               // очистка памяти
    inline void set_size(size_t size) {                         //установка размера заполненной части
        if (size > _capacity) {
            throw std::invalid_argument("ERROR: Size is bigger than capacity!");
        }
        else {
            _size = size;
        }
    }

    //операторы
    MemData& operator=(const MemData&) noexcept;         // присваивания
    MemData& operator=(MemData&&) noexcept;              // присваивания с move-семантикой
    bool operator==(const MemData&) const noexcept;      // сравнения

    //друзьяшки:
    //-функции
    template <typename T> //<----иначе не компилируется
    friend void quick_sort(MemData<T>& md);            // сортировки
    template <typename T>
    friend void shuffle(MemData<T>&);                  // перемешивания

    //-классы
    friend class Vector<T>;

private:
    //служебные методы
    inline bool is_full() const noexcept {      // проверка на переполнение
        return (_size >= _capacity);
    }
};

//функции вне класса
int calculate_capacity(size_t);     //какую вместимость выставить при заданном кол-ве эл-тов
template <typename T>
void quick_sort_recursive(T* data, int left, int right);
template <typename T>
int partition(T* data, int left, int right);

//инстанцирование шаблона (генерация объектного файла под определенный тд):
//template class MemData<double>;
//template void quick_sort(MemData<double>&);
//template void shuffle(MemData<double>&);



//функции вне класса
template <typename T>
int partition(T* data, int left, int right) { //разбиение по Хоару
    T pivot = data[(left + right) / 2]; //опорный эл-т
    int i = left;
    int j = right;
    while (i <= j) {
        while (data[i] < pivot) i++;
        while (data[j] > pivot) j--;
        if (i <= j) {
            std::swap(data[i], data[j]);
            i++;
            j--;
        }
    }
    return i;
}

template <typename T>
void quick_sort_recursive(T* data, int left, int right) {
    if (left < right) {
        int pivot_index = partition(data, left, right);
        quick_sort_recursive(data, left, pivot_index - 1);
        quick_sort_recursive(data, pivot_index, right);
    }
}

//дружественные функции
template <typename T>
void quick_sort(MemData<T>& md) {				//метод сортировки Хоара
	if (md._size <= 1) {
		return;
	}
	quick_sort_recursive(md._data, 0, md._size - 1);
}
template <typename T>
void shuffle(MemData<T>& md) {				//перемешивание Фишер-Йетса
	if (md._size <= 1) {
		return;
	}
	srand(time(NULL));
	for (size_t i = md._size - 1; i > 0; i--) {
		size_t j = rand() % (i + 1);
		std::swap(md._data[i], md._data[j]);
	}
}

//функции вне класса
int calculate_capacity(size_t size) {
	int new_capacity = size / MEM_STEP;
	return (new_capacity + 1) * MEM_STEP;
}

//конструкторы
template <typename T>
MemData<T>::MemData(size_t size) {								//по размеру + по умолчанию
	_data = nullptr;
	set_memory(size);
}
template <typename T>
MemData<T>::MemData(std::initializer_list<T> list) {	//по списку инициализации
	_data = nullptr;
	set_memory(list.size());
	for (size_t i = 0; i < _size; i++) {
		_data[i] = *(list.begin() + i);
	}
}
template <typename T>
MemData<T>::MemData(const T* array, size_t size) {		//инициализации
	_size = size;
	_data = nullptr;
	set_memory(_size);
	if (!array) {
		for (size_t i = 0; i < _size; i++) {
			_data[i] = T();
		}
	}
	else {
		for (size_t i = 0; i < _size; i++) {
			_data[i] = array[i];
		}
	}
}
template <typename T>
MemData<T>::MemData(const MemData& other) {						//копирования
	_size = other._size;
	_data = nullptr;
	set_memory(_size);
	for (size_t i = 0; i < _size; i++) {
		_data[i] = other._data[i];
	}
}
template <typename T>
MemData<T>::MemData(MemData&& other) noexcept {					//с move-семантикой
	_size = other._size;
	_capacity = other._capacity;
	_data = other._data;
	other._size = 0;
	other._capacity = 0;
	other._data = nullptr;
}

//деструктор
template <typename T>
MemData<T>::~MemData() {
	if (_data) {
		delete[] _data;
	}
}

//сеттеры памяти
template <typename T>
void MemData<T>::set_memory(size_t size) noexcept {				//установка без сохр
	_size = size;
	_capacity = calculate_capacity(size);
	if (_data) {
		delete[] _data;
	}
	_data = new T[_capacity];
}
template <typename T>
void MemData<T>::reset_memory(size_t size, size_t start_index) {	//перевыделение с сохранением данных
	if (start_index >= size) {
		throw std::invalid_argument("ERROR: Can't reset memory! start_index >= size");
	}
	size_t new_capacity = calculate_capacity(size);
	size_t old_capacity = _capacity;
	size_t copy_size = _size;
	_size = size;
	if (old_capacity == new_capacity) {
		if (start_index == 0) {
			return;
		}
		T* temp = new T[start_index];
		for (size_t i = 0; i < start_index; i++) {
			temp[i] = _data[i];
		}
		for (size_t i = 0; i < copy_size; i++) {
			if (i + start_index < old_capacity) {
				_data[i] = _data[(i + start_index)];
			}
			else {
				_data[i] = temp[(i + start_index) % old_capacity];
			}
		}
		return;
	}
	_capacity = new_capacity;
	T* temp = new T[_capacity];
	for (size_t i = 0; i < copy_size; i++) {
		if (i >= _size) {
			break;
		}
		temp[i] = _data[(i + start_index) % old_capacity];
	}
	delete[]_data;
	_data = temp;
}
template <typename T>
void MemData<T>::clear_memory() noexcept {						//очистка памяти
	set_memory(0);
}

//операторы
template <typename T>
MemData<T>& MemData<T>::operator=(const MemData& other) noexcept {	//присваивания
	if (this != &other) {
		_size = other._size;
		_capacity = other._capacity;
		if (_data) {
			delete[] _data;
		}
		_data = new T[_capacity];
		for (size_t i = 0; i < _size; i++) {
			_data[i] = other._data[i];
		}
	}
	return *this;
}
template <typename T>
MemData<T>& MemData<T>::operator=(MemData&& other) noexcept {		//присваивания с move-семантикой
	if (this != &other) {
		_size = other._size;
		_capacity = other._capacity;
		delete[] _data;
		_data = other._data;
	}
	other._size = 0;
	other._capacity = 0;
	other._data = nullptr;
	return *this;
}
template <typename T>
bool MemData<T>::operator==(const MemData& other) const noexcept {			//сравнения
	if (_size != other._size) {
		return false;
	}
	else if (_data == other._data) {
		return true;
	}
	else if (_size == 0) {
		return true;
	}
	else {
		for (size_t i = 0; i < _size; i++) {
			if (_data[i] != other._data[i]) {
				return false;
			}
		}
		return true;
	}
}
