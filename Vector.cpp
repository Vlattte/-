#include "Vector.h"
#include <iostream>
#include <math.h>

MyVector::MyVector()
{
	_standartValue = 0.0;
	_resizeStrategy = ResizeStrategy::Multiplicative;
	_coef = 2;
	_capacity = 1;
	_size = 0;
	_data = NULL;
}

MyVector::MyVector(size_t size, ResizeStrategy resizeStrategy, float coef)
	:_size(size), _resizeStrategy(resizeStrategy), _coef(coef), _capacity(0)
{
	if (_size > 0 && _size > _capacity)
		_capacity = _size;
	else
		_capacity = 1;

	_data = new ValueType[_capacity];

	for (size_t i = 0; i < _size; i++)
	{
		_data[i] = 0.0;
	}
}

MyVector::MyVector(size_t size, ValueType value, ResizeStrategy resizeStrategy, float coef)
	:_size(size), _resizeStrategy(resizeStrategy), _coef(coef), _capacity(0)
{
	if (_size > 0 && _size > _capacity)
		_capacity = _size;
	else
		_capacity = 1;

	_data = new ValueType[_capacity];

	for (size_t i = 0; i < _size; i++)
	{
		_data[i] = value;
	}
}

MyVector::MyVector(const MyVector & copy)
{
	_size = copy._size;
	_standartValue = copy._standartValue;
	_resizeStrategy = copy._resizeStrategy;
	_coef = copy._coef;

	_data = new ValueType[_capacity];
	for (size_t i = 0; i < _size; ++i)
	{
		_data[i] = copy._data[i];
	}

}

MyVector& MyVector::operator=(const MyVector & copy)
{
	_size = copy._size;
	_standartValue = copy._standartValue;
	_resizeStrategy = copy._resizeStrategy;
	_coef = copy._coef;

	_data = new ValueType[_capacity];
	for (size_t i = 0; i < _size; ++i)
	{
		_data[i] = copy._data[i];
	}

	return *this;
}

MyVector::~MyVector()
{
	delete[] _data;
}

size_t MyVector::capacity() const
{
	return _capacity;
}

size_t MyVector::size() const
{
	return _size;
}

float MyVector::loadFactor()
{
	if (_size == 0)
	{
		return 1;
	}

	else
	{
		return float(size()) / float(capacity());
	}
}

ValueType& MyVector::operator[](const size_t i) const
{
	return _data[i];
}

void MyVector::pushBack(const ValueType & value)
{
	resize(_size + 1);
	_data[_size - 1] = value;
}

void MyVector::popBack()
{
	resize(_size - 1);
}

void MyVector::erase(const size_t i)
{
	resize(i);
}

void MyVector::erase(const size_t i, const size_t len)
{
	size_t idx = i;

	while (idx + len < _size)
	{
		_data[idx] = _data[idx + len];
		++idx;
	}

	resize(idx);
}

long long int MyVector::find(const ValueType & value, bool isBegin) const
{
	int flag = -1;

	if (isBegin == true)
	{
		for (size_t i = 0; i < _size; ++i)
		{
			if (_data[i] == value)
			{
				flag = 1;
				return i;
			}
		}
	}
	else if (isBegin == false)
	{
		for (size_t i = _size; i > 0; i--)
		{
			if (_data[i] == value)
			{
				flag = 1;
				return _size - i;
			}
		}
	}

	if (flag == -1)
		return flag;
}

void MyVector::insert(const size_t i, const ValueType & value)
{
	size_t oldSize = _size;
	ValueType giver;
	if (oldSize == 0)
	{
		resize(0);
	}

	if (i == 0) //если вставляем в начало
	{
		ValueType* buff = new ValueType[_size + 1];
		buff[0] = value;

		//переносим старые значения в буфер
		for (size_t idx = 0; idx < _size; idx++)
		{
			giver = _data[idx];
			buff[idx + 1] = giver;
		}

		//увеличиваем _data
		resize(_size + 1);

		//переносим старые значения и новый элемент в расширенную _data
		for (size_t idx = 0; idx < _size; idx++)
		{
			giver = buff[idx];
			_data[idx] = giver;
		}

		//чистим буфер
		delete[] buff;
		buff = nullptr;
	}

	//если вставляем в конец, то pushBack
	if (oldSize == i && oldSize != 0)
	{
		pushBack(value);
	}

	//если вставляем в середину
	if (i > 0 && i < oldSize)
	{
		ValueType* buff = new ValueType[_size + 1];

		//переносим все элементы от i до конца в буфер
		for (size_t idx = i; idx < _size; idx++)
		{
			giver = _data[idx];
			buff[idx + 1] = giver;
		}

		//переносим все элементы с начала до i в буфер
		for (size_t idx = 0; idx < i; idx++)
		{
			giver = _data[idx];
			buff[idx] = giver;
		}

		buff[i] = value;   //вставляем новый элемент
		resize(_size + 1); //расширяем _data

		//переносим из буфера в _data
		for (size_t idx = 0; idx < _size; idx++)
		{
			giver = buff[idx];
			_data[idx] = giver;
		}

		delete[] buff;
		buff = nullptr;
	}

}

void MyVector::insert(const size_t i, const MyVector & value)
{
	resize(_size + value._size);
	for (size_t idx = _size; idx >= i + value._size; idx--) 
	{														
		_data[idx] = _data[idx - value._size];
	}

	for (size_t idx = i, n = 0; idx < i + value._size; idx++, n++)
	{
		_data[idx] = value._data[n];
	}
}

void MyVector::reserve(const size_t capacity)
{
		ValueType* buff = new ValueType[capacity];		//массив для старых значений
		ValueType giver;							    //переменная для передачи значений

		for (size_t i = 0; i < _size; ++i)
		{
			giver = _data[i];
			buff[i] = giver;						    //переносим старые значения
		}

		delete[] _data;                                 //освобождаем старую память
		_data = nullptr;		                        //запомнили новое capacity
		_data = new ValueType[capacity];                //выделяем новую память новой длины

		for (size_t i = 0; i < _size; ++i)
		{
			giver = buff[i];
			_data[i] = giver;	                        //возвращаем старые элементы на свои места
		}
		delete[] buff;
}

void MyVector::resize(const size_t size, const ValueType value)
{
	size_t oldSize = _size;
	_size = size;

	// считаем capacity по формуле Multiplicative
	if (_resizeStrategy == ResizeStrategy::Multiplicative) 
	{
		while (_capacity < _size)
		{
			if (loadFactor() > 1)
				_capacity = _capacity * _coef;
			else if (loadFactor() < 1)
				_capacity = _capacity * loadFactor();
			else if (loadFactor() == 1)
				_capacity = _capacity;
		}
	}	
	// считаем capacity по формуле Additive
	else if (_resizeStrategy == ResizeStrategy::Additive) 
	{
		while (_capacity < _size)
		{
			if (_capacity > _size)
				_capacity = _size;
			else if (_capacity < _size)
				while (_capacity >= _size)
				{
					_capacity = _capacity + 1;
				}
		}
	}
	if (oldSize != 0)
	{
		reserve(_capacity); // резервируем память для вектора
	}
	else if (oldSize == 0)
	{
		_data = new ValueType[_capacity];
	}

	for (size_t i = oldSize; i < _capacity; ++i)
	{
		_data[i] = value; // заполняем новое место дефолтными значениями
	}
}

void MyVector::clear()
{
	delete[] _data;
	_data = NULL;
	_size = 0;
	_capacity = 0;
}

ValueType* MyVector::begin()
{
	return _data;
}

ValueType* MyVector::end()
{
	return _data + _capacity;
}

void MyVector::write()
{
	std::cout << "[";
	for (size_t i = 0; i < _size; ++i)
	{
		std::cout << _data[i] << ",";
	}
	std::cout << "end]" << std::endl;
}
