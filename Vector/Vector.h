#pragma once
#include <iostream>
#include <cstdlib>
#include "memdata.h"

template<class T>
class Vector;

template <typename T>
std::ostream& operator<< (std::ostream&, const Vector<T>&);	// вывода
template <typename T>
std::istream& operator>> (std::istream&, Vector<T>&);			// ввода
//сортировки и перемешивания
template <typename T>
void quick_sort(Vector<T>&);		//сортировки (Хоара)
template <typename T>
void shuffle(Vector<T>&);	//перемешивания (Фишер-Йетса)

template<class T>
class Vector {
	MemData<T> _mem;	// хранилище данных + размер  + вместимость
	size_t _front;				// индекс первого элемента
	size_t _back;				// индекс последнего элемента

public:
	//конструкторы
	Vector(size_t size = 0);					// конструктор по размеру + по умолчанию
	Vector(std::initializer_list<T>); // конструктор по списку инициализации
	Vector(T*, size_t);               // конструктор инициализации
	Vector(const Vector<T>&);         // конструктор копирования
	Vector(Vector<T>&&) noexcept;     // конструктор с move-семантикой
	//деструктор
	//~Vector() = default; //напишется компилятором

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

	iterator begin() noexcept {
		return iterator(&get_front());
	}
	iterator end() noexcept {
		return iterator(&get_back()+1);
	}

	//const_iterator begin() noexcept;
	//const_iterator end() noexcept;

	//публичные методы проверок
	inline bool is_empty() const noexcept {			// на пустоту
		return _mem.is_empty();
	}
	inline bool is_full() const noexcept {			// на переполнение
		return _mem.is_full();
	}

	//геттеры
	inline size_t get_size() const noexcept {		// размера
		return _mem._size;
	}
	inline size_t get_capacity() const noexcept {	// вместимости
		return _mem._capacity;
	}
	inline T& get_front() const {			// первого элемента (возвр. копию)
		if (_mem._size != 0) {
			return (_mem._data)[_front];
		}
		else {
			throw std::logic_error("ERROR: Vector is empty! Can't get front");
		}
	}
	inline T& get_back() const {			// последнего элемента (возвр. копию)
		if (_mem._size != 0) {
			return (_mem._data)[_back];
		}
		else {
			throw std::logic_error("ERROR: Vector is empty! Can't get back");
		}
	}
	inline MemData<T> get_mem_copy() const noexcept {            // копии мемдаты (для тестов)
		return _mem;
	}
	inline const MemData<T>& get_mem_original() const noexcept { // оригинала мемдаты (для тестов на move)
		return _mem;
	}

	//сеттеры
	inline T& front_ref() {				// первого элемента (возвр. ссылку)
		if (_mem._size != 0) {
			return (_mem._data)[_front];
		}
		else {
			throw std::logic_error("ERROR: Vector is empty! Can't set front");
		}
	}
	inline T& back_ref() {				// последнего элемента (возвр. ссылку)
		if (_mem._size != 0) {
			return (_mem._data)[_back];
		}
		else {
			throw std::logic_error("ERROR: Vector is empty! Can't set back");
		}
	}

	//публичные методы вставок
	void push_front(T) noexcept;					// 1 элемента в начало
	void push_front_many(T*, size_t) noexcept;	// нескольких в начало
	void push_back(T) noexcept;					// 1 элемента в конец
	void push_back_many(T*, size_t) noexcept;		// нескольких в конец
	void insert(T, size_t);						// 1 элемента по позиции
	void insert_many(T*, size_t, size_t);			// нескольких по позиции

	//удалений
	void pop_front();                               // 1 элемента из начала
	void pop_front_many(size_t);					// нескольких из начала
	void pop_back();                                // 1 элемента из конца
	void pop_back_many(size_t);						// нескольких из конца
	void erase(size_t);                             // 1 элемента по позиции
	void erase_many(size_t, size_t);				// нескольких по позиции

	//перегрузки операторов
	Vector<T>& operator=(const Vector<T>&) noexcept;      // присваивания
	Vector<T>& operator=(Vector<T>&&) noexcept;           // присваивания с move-семантикой
	T operator[](size_t) const noexcept;       // обращения по индексу (не изм., возвр. копию)
	T& operator[](size_t) noexcept;            // обращения по индексу (изм., возвр. ссылку)

	//дружественные функции
	//перегрузки ввода-вывода
	template <typename T>
	friend std::ostream& operator<< (std::ostream&, const Vector<T>&);	// вывода
	template <typename T>
	friend std::istream& operator>> (std::istream&, Vector<T>&);			// ввода
	//сортировки и перемешивания
	template <typename T>
	friend void quick_sort(Vector<T>&);		//сортировки (Хоара)
	template <typename T>
	friend void shuffle(Vector<T>&);	//перемешивания (Фишер-Йетса)

private:
	//служебный метод получения (геттер) физического индекса по относительному
	inline size_t get_mem_index(size_t i) const {
		return ((i + _front) % _mem._capacity);
	}
};

//инстанцирование шаблона (генерация объектного файла под определенный тд):
//template class Vector<double>;
//template std::ostream& operator<< (std::ostream& out, const Vector<double>& v1);
//template std::istream& operator>> (std::istream& in, Vector<double>& v1);
//template void shuffle(Vector<double>&);
//template void quick_sort(Vector<double>&);

//дружественные функции
//сортировки и перемешивания
template<typename T>
void quick_sort(Vector<T>& vector) {
	quick_sort(vector._mem);
}
template<typename T>
void shuffle(Vector<T>& vector) {
	shuffle(vector._mem);
}

//перегрузки ввода-вывода
template <typename T>
std::ostream& operator<< (std::ostream& out, const Vector<T>& vector) {	// вывода
	out << "{ ";
	size_t size = vector.get_size();
	if (size != 0) {
		out << vector._mem.get_data_const()[vector._front];
	}
	for (size_t i = 1; i < size; i++) {
		out << ", " << vector._mem.get_data_const()[vector.get_mem_index(i)];
	}
	out << " }";
	return out;
};
template <typename T>
std::istream& operator>> (std::istream& in, Vector<T>& vector) {			// ввода
	Vector<T> temp;
	T element;
	in >> element;
	while (in >> element) {
		temp.push_back(element);
	}
	vector = std::move(temp);
	return in;
};

//конструкторы
template<typename T>
Vector<T>::Vector(size_t size) :_mem(size) {
	/*MemData<T> temp(size);
	//_mem = (std::move(temp));*/
	_front = 0;
	if (size > 0) {
		_back = size - 1;
	}
	else {
		_back = 0;
	}
}
template<typename T>
Vector<T>::Vector(std::initializer_list<T> list) :_mem(list) {
	//MemData<T> temp(list);
	//_mem = (std::move(temp));
	_front = 0;
	if (_mem._size > 0) {
		_back = _mem._size - 1;
	}
	else {
		_back = 0;
	}
}
template<typename T>
Vector<T>::Vector(T* array, size_t size) :_mem(array, size) {
	//MemData<T> temp(array, size);
	//_mem = (std::move(temp));
	_front = 0;
	if (_mem._size > 0) {
		_back = _mem._size - 1;
	}
	else {
		_back = 0;
	}
}
template<typename T>
Vector<T>::Vector(const Vector<T>& other) {
	_mem = other._mem;
	_front = other._front;
	_back = other._back;
}
template<typename T>
Vector<T>::Vector(Vector<T>&& other) noexcept {
	_mem = std::move(other._mem);
	_front = other._front;
	other._front = 0;
	_back = other._back;
	other._back = 0;
}

//публичные методы
//вставок
template<typename T>
void Vector<T>::push_front(T element) noexcept {
	_mem._size++;
	if (_mem._size > 1) {
		_front = (_front + _mem._capacity - 1) % _mem._capacity;
	}
	_mem._data[_front] = element;
	if (is_full()) {
		_mem.reset_memory(_mem._size, _front);
		_front = 0;
		_back = _mem._size - 1;
	}
}

template<typename T>
void Vector<T>::push_front_many(T* elements, size_t size) noexcept {
	for (int i = size - 1; i >= 0; i--) { //идем с конца тк кладем в начало
		push_front(elements[i]);
	}
}
template<typename T>
void Vector<T>::push_back(T element) noexcept {
	_mem._size++;
	if (_mem._size > 1) {
		_back = (_back + 1) % _mem._capacity;
	}
	_mem._data[_back] = element;
	if (is_full()) {
		_mem.reset_memory(_mem._size, _front);
		_front = 0;
		_back = _mem._size - 1;
	}
}
template<typename T>
void Vector<T>::push_back_many(T* elements, size_t size) noexcept {
	for (int i = 0; i < size; i++) { //идем с начала тк кладем в конец
		push_back(elements[i]);
	}
}
template<typename T>
void Vector<T>::insert(T element, size_t i) {
	if (i > _mem._size) {
		throw std::out_of_range("ERROR: Insert index out of range!");
	}
	else {
		if (i == 0) {
			push_front(element);
		}
		else if (i == _mem._size) {
			push_back(element);
		}
		else {
			_mem._size++;
			_back = (_back + 1) % _mem._capacity;
			for (int j = _mem._size - 1; j > i; j--) {
				(*this)[j] = (*this)[j - 1];
			}
			(*this)[i] = element;
			if (is_full()) {
				_mem.reset_memory(_mem._size, _front);
				_front = 0;
				_back = _mem._size - 1;
			}
		}
	}
}
template<typename T>
void Vector<T>::insert_many(T* elements, size_t size, size_t i) {
	if (i > _mem._size) {
		throw std::out_of_range("ERROR: Insert (many) index out of range!");
	}
	else {
		for (int j = 0; j < size; j++) {
			insert(elements[j], i + j);
		}
	}
}

//удалений
template<typename T>
void Vector<T>::pop_front() {
	if (_mem._size == 0) {
		throw std::logic_error("ERROR: Empty vector! Can't pop front");
	}
	else {
		_mem._size--;
		if (_mem._size != 0) {
			_front = (_front + 1) % _mem._capacity;
		}
		else {
			_front = 0;
			_back = 0;
		}
		if (_mem._capacity - _mem._size > MEM_STEP) {
			_mem.reset_memory(_mem._size, _front);
			_front = 0;
			if (_mem._size != 0) {
				_back = _mem._size - 1;
			}
		}
	}
}
template<typename T>
void Vector<T>::pop_front_many(size_t count) {
	if (_mem._size < count) {
		throw std::logic_error("ERROR: Popping (front) too many elements!");
	}
	else {
		_mem._size -= count;
		if (_mem._size != 0) {
			_front = (_front + count) % _mem._capacity;
		}
		else {
			_front = 0;
			_back = 0;
		}
		if (_mem._capacity - _mem._size > MEM_STEP) {
			_mem.reset_memory(_mem._size, _front);
			_front = 0;
			if (_mem._size != 0) {
				_back = _mem._size - 1;
			}
		}
	}
}
template<typename T>
void Vector<T>::pop_back() {
	if (_mem._size == 0) {
		throw std::logic_error("ERROR: Empty vector! Can't pop back");
	}
	else {
		_mem._size--;
		if (_mem._size != 0) {
			_back = (_back + _mem._capacity - 1) % _mem._capacity;
		}
		else {
			_front = 0;
			_back = 0;
		}
		if (_mem._capacity - _mem._size > MEM_STEP) {
			_mem.reset_memory(_mem._size, _front);
			_front = 0;
			if (_mem._size != 0) {
				_back = _mem._size - 1;
			}
		}
	}
}
template<typename T>
void Vector<T>::pop_back_many(size_t count) {
	if (_mem._size < count) {
		throw std::logic_error("ERROR: Popping (back) too many elements!");
	}
	else {
		_mem._size -= count;
		if (_mem._size != 0) {
			_back = (_back + _mem._capacity - count) % _mem._capacity;
		}
		else {
			_front = 0;
			_back = 0;
		}
		if (_mem._capacity - _mem._size > MEM_STEP) {
			_mem.reset_memory(_mem._size, _front);
			_front = 0;
			if (_mem._size != 0) {
				_back = _mem._size - 1;
			}
		}
	}
}
template<typename T>
void Vector<T>::erase(size_t i) {
	if (i >= _mem._size) {
		throw std::out_of_range("ERROR: Erase index out of range!");
	}
	else {
		if (i == 0) {
			pop_front();
		}
		else if (i == _mem._size - 1) {
			pop_back();
		}
		else {
			_back = (_back - 1 + _mem._capacity) % _mem._capacity;
			for (int j = i; j < _mem._size - 1; j++) {
				(*this)[j] = (*this)[j + 1];
			}
			_mem._size--;
			if (_mem._size == 0) {
				_front = 0;
				_back = 0;
			}
			if (_mem._capacity - _mem._size > MEM_STEP) {
				_mem.reset_memory(_mem._size, _front);
				_front = 0;
				_back = _mem._size == 0 ? 0 : _mem._size - 1;
			}
		}
	}
}
template<typename T>
void Vector<T>::erase_many(size_t i, size_t count) {
	if (i + count > _mem._size) {
		throw std::logic_error("ERROR: Erasing too many elements!");
	}
	for (int j = 0; j < count; j++) {
		erase(i);
	}
}

//перегрузки операторов
template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) noexcept {
	if (this != &other) {
		_mem = other._mem;
		_front = other._front;
		_back = other._back;
	}
	return (*this);
}
template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
	if (this != &other) {
		_mem = std::move(other._mem);
		_front = other._front;
		other._front = 0;
		_back = other._back;
		other._back = 0;
	}
	return (*this);
}
template<typename T>
T Vector<T>::operator[](size_t i) const noexcept {
	return _mem._data[(*this).get_mem_index(i)];
}
template<typename T>
T& Vector<T>::operator[](size_t i) noexcept {
	return _mem._data[(*this).get_mem_index(i)];
}


