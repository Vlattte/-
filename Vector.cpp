#include "Vector.h"
#include <iostream>

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

	if (i == 0)
	{
		ValueType* buff = new ValueType[_size + 1];
		buff[0] = value;

		for (size_t idx = 0; idx < _size; idx++)
		{
			giver = _data[idx];
			buff[idx + 1] = giver;
		}

		resize(_size + 1);

		for (size_t idx = 0; idx < _size; idx++)
		{
			giver = buff[idx];
			_data[idx] = giver;
		}
		delete[] buff;
		buff = NULL;
	}

	if (oldSize == i && oldSize != 0)
	{
		pushBack(value);
	}

	if (i > 0 && i < oldSize)
	{
		ValueType* buff = new ValueType[_size + 1];

		for (size_t idx = i; idx < _size; idx++)
		{
			buff[idx + 1] = _data[idx];
		}

		for (size_t idx = 0; idx < i; idx++)
		{
			buff[idx] = _data[idx];
		}

		buff[i] = value;
		resize(_size + 1);

		for (size_t idx = 0; idx < _size; idx++)
		{
			_data[idx] = buff[idx];
		}

		delete[] buff;
		buff = NULL;
	}

}

void MyVector::insert(const size_t i, const MyVector & value)
{
	resize(_size + value._size);
	for (size_t idx = _size; idx >= i + value._size; idx--) //[0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0]
	{														//[7, 8, 9, 10, 11, 12]
		_data[idx] = _data[idx - value._size];
	}

	for (size_t idx = i, n = 0; idx < i + value._size; idx++, n++)
	{
		_data[idx] = value._data[n];
	}
}

void MyVector::reserve(const size_t capacity)
{
	if (capacity > _capacity)
	{
		ValueType* buff = new ValueType[_capacity];		//массив для старых значений
		ValueType giver; //переменная для передачи значений

		for (size_t i = 0; i < _size; ++i)
		{
			giver = _data[i];
			buff[i] = giver;		//переносим старые значения
		}

		delete[] _data; //освобождаем старую память
		_data = NULL;
		_capacity = capacity;	//запомнили новое capacity
		_data = new ValueType[_capacity];   //выделяем новую память новой длины
		for (size_t idx = 0; idx < _capacity; idx++)
		{
			_data[idx] = 0;
		}

		for (size_t i = 0; i < _size; ++i)
		{
			giver = buff[i];
			_data[i] = giver;	//возвращаем старые элементы на свои места
		}
		delete[] buff;
		buff = nullptr;
	}
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

MyVector MyVector::sortedSquares(const MyVector & vec, SortedStrategy strategy)
{
	MyVector outVec = vec;
	int idxOutVec = 0;
	for (size_t i = 0; i < _size; i++)
	{
		if (_data[i] == 0)
			outVec._data[idxOutVec] = _data[i];  //сохраняем все нули
	}
	
	ValueType min, minusMin;
	ValueType max, minusMax;
	//[-23, -12, -1, 5, 7, -7, 1]
	return vec;
}

void MyVector::write()
{
	size_t size = _size;
	std::cout << "[";
	for (size_t i = 0; i < _size; ++i)
	{
		std::cout << _data[i] << ",";
	}
	std::cout << "end]" << std::endl;
}
