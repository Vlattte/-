#include "DoubleLinkedList.h"

#include <iostream>
#include <cassert>

//==================================================================================================//

DoubleLinkedList::Node::Node(const ValueType& value, Node* next, Node* previous)
{
	this->value = value;
	this->next = next;
	this->previous = previous;
}

DoubleLinkedList::Node::~Node()
{

}

void DoubleLinkedList::Node::insertNext(const ValueType& value)  // [2 (3) new 4]
{
	Node* newNode = new Node(value, this->next, this); //создаем новый узел со значением value
	this->next = newNode; //следующий изменился, но предыдущий остался таким же
}

void DoubleLinkedList::Node::insertPrevious(const ValueType & value) // [2 new (3) 4]
{
	Node* newNode = new Node(value, this, this->previous); //создаем новый узел со значением value
	this->previous = newNode; //предыдущий изменился, но следующий остался таким же
}

void DoubleLinkedList::Node::removeNext() // [2 (3) >4< 5] --> [2 (3) 5]
{

	Node* removeNode = this->next;
	if (removeNode == nullptr)
		return;
	Node* newNext = removeNode->next;
	delete removeNode;
	this->next = newNext;
}

void DoubleLinkedList::Node::removePrevious() // [2 >3< (4) 5] --> [2 (4) 5]
{
	Node* removeNode = this->previous;
	Node* newPrevious = removeNode->previous;
	delete removeNode;
	this->previous = newPrevious;
}

//==================================================================================================//

DoubleLinkedList::DoubleLinkedList()
	:_head(nullptr), _tail(nullptr), _size(0)
{
}

DoubleLinkedList::DoubleLinkedList(const DoubleLinkedList & copyList) 
{
	this->_size = copyList._size; //копируем размер
	
	//если список, что мы копируем пустой, то возвращем пустой список
	if (this->_size == 0)
	{
		this->_head = nullptr;
		this->_tail = nullptr;
		return;
	}

	//выделяем память и копируем голову
	this->_head = new Node(copyList._head->value);

	Node* currentNode = this->_head;
	Node* currentCopyNode = copyList._head->next;
	while (currentCopyNode)
	{
		//next для текущего узла = текущий скопированный узел
		currentNode->next = new Node(currentCopyNode->value);
		currentCopyNode = currentCopyNode->next; 
		currentNode = currentNode->next;         
	}
	this->_tail = currentNode;
}

DoubleLinkedList& DoubleLinkedList::operator=(const DoubleLinkedList& copyList)
{
	if (this == &copyList)
	{
		return *this; //если совпал, то копируем самого себя
	}

	this->_size = copyList._size;
	this->_head = copyList._head;
	this->_tail = copyList._tail;

	return *this;
}

DoubleLinkedList::DoubleLinkedList(DoubleLinkedList&& moveList) noexcept
{
	this->_size = moveList._size;
	this->_head = moveList._head;
	this->_tail = moveList._tail;

	moveList._size = 0;
	moveList._head = nullptr;
	moveList._tail = nullptr;
}

DoubleLinkedList& DoubleLinkedList::operator=(DoubleLinkedList && moveList) noexcept
{
	if (this == &moveList)
	{
		return *this;	//если это мы, то возвращаем себя же
	}

	forceNodeDelete(_head); //рекурсивно удаляем this

	this->_size = moveList._size;
	this->_head = moveList._head;
	this->_tail = moveList._tail;

	moveList._size = 0;
	moveList._head = nullptr;
	moveList._tail = nullptr;

	return *this; //возвращаем себя скопированного

}

DoubleLinkedList::~DoubleLinkedList()
{
	forceNodeDelete(_head);
}

//_______________________________________________________________________//

ValueType& DoubleLinkedList::operator[](const size_t pos) const
{
	return getNode(pos)->value;
}

DoubleLinkedList::Node* DoubleLinkedList::getNode(const size_t pos) const
{
	if (pos < 0)
	{
		assert(pos < 0); //ошибка: размер меньше нуля
	}
	else if (pos >= this->_size)
	{
		assert(pos >= this->_size); //ошибка: вышли за границу списка
	}

	if (pos == _size - 1) 
	{
		return this->_tail;
	}
	else //если не последний элемент
	{
		Node* bufNode = this->_head; // создаем буфер для хранения головного узла
		for (int i = 0; i < pos; i++)
		{
			bufNode = bufNode->next; //перебираем элементы до нужного
		}

		return bufNode;
	}
}

void DoubleLinkedList::insert(const size_t pos, const ValueType & value)
{
	if (pos < 0)
	{
		assert(pos < 0); //ошибка: размер меньше нуля
	}
	else if (pos > this->_size)
	{
		assert(pos > this->_size); //ошибка: вышли за границу списка
	}

	if (pos == 0) //если нужно вставить в начало списка, то pushFront
	{
		pushFront(value);
	}
	else if (pos == _size)
	{
		pushBack(value);
	}
	else //иначе перебираем узлы и вставляем на нужную позицию // [1 2 (3) 4 5] ---> [1 2 (3) <new> 4 5]
	{
		Node* bufNode = this->_head;
		for (size_t i = 0; i < pos - 1; ++i)
		{
			bufNode = bufNode->next;
		}
		bufNode->insertNext(value);
		++_size;
		if (pos == _size - 2)
		{
			_tail->previous = bufNode->next; //[1 2 3 tail] size(4) pos (3) ---> [1 2 3 <new> tail] size(5) pos(4)
		}
	}
}

void DoubleLinkedList::insertAfterNode(Node* node, const ValueType & value)
{
	node->insertNext(value);
}

void DoubleLinkedList::insertBeforeNode(Node * node, const ValueType & value)
{
	node->insertPrevious(value);
}

void DoubleLinkedList::pushBack(const ValueType& value)
{
	if (_size == 0)
	{
		pushFront(value);
		return;
	}
	else if (_size == 1)
	{
		_head = _tail;
		_tail = new Node(value, nullptr, _head); //[1 2]
		_size++;
	}
	else
	{
		Node* newTail = new Node(value, nullptr, _tail);
		_tail = newTail;
		Node* oldTail = _tail->previous;
		oldTail->next = _tail;
		_size++;
	}
}

void DoubleLinkedList::pushFront(const ValueType & value) // [(1) 2 3 4 5] ---> [<new> (1) 2 3 4 5]
{
	// перезаписываем головной узел (предыдущий по дефолту nullptr)
	++_size;
	if (_size == 2)
	{
		_tail = _head;
		_head = new Node(value, _tail); //[1 2]
		_tail->previous = _head;
	}
	else if (_size < 2)
	{
		_head = new Node(value, _head); //[1]
	}
	else
	{
		_head = new Node(value, _head); //[1]
		Node* buf = this->_head->next;
		buf->previous = _head; //перезаписываем предыдущий старой голове 
	}
}

void DoubleLinkedList::remove(const size_t pos)
{
	if (pos < 0)
	{
		assert(pos < 0);           //ошибка: размер меньше нуля
	}
	else if (pos > this->_size)
	{
		assert(pos > this->_size); //ошибка: вышли за границу списка
	}

	if (pos == 0)
	{
		//если удаляем голову, то перезаписываем ее
		this->_head = this->_head->next;
		_size--;
	}
	else if (pos == _size - 1)
	{
		this->_tail = this->_tail->previous;
		_tail->next = nullptr;
		_size--;
	}
	else
	{
		Node* bufNode = this->_head;
		while (bufNode->next != this->getNode(pos))
		{
			bufNode = bufNode->next;
		}
		bufNode->removeNext();
		if (pos == _size - 2)
		{
			_tail->previous = bufNode; //[1 2 3 tail] size(4) pos(3) --> [1 2 >3< tail] size(3) pos(2)
		}
		_size--;
	}
}

void DoubleLinkedList::removeNextNode(Node* node)        // [(1) >2< 3 4 5] ---> [(1) 3 4 5]
{
	size_t pos = findIndex(node->value);
	size_t posTail = findIndex(_tail->value);
	if (posTail == pos)
	{
		std::cout << "Next is nullptr, so, it's already deleted" << std::endl;
		return;
	}

	size_t position = findIndex(node->next->value);
	this->remove(position);
}

void DoubleLinkedList::removePreviousNode(Node * node)   // [>1< (2) 3 4 5] ---> [(2) 3 4 5]
{
	size_t pos = findIndex(node->value);
	if (pos == 0)
	{
		std::cout << "There is no previous element" << std::endl;
		return;
	}
	size_t position = findIndex(node->previous->value);
	this->remove(position);
}

void DoubleLinkedList::removeFront()
{
	this->remove(0);
	if (_size == 1)
	{
		_head->next = _tail;
		_head->previous = nullptr;
		_tail->previous = nullptr;
	}
}

void DoubleLinkedList::removeBack()
{
	if (_size == 2)
	{
		_tail = nullptr;
		return;
	}

	this->remove(_size - 1);
}

long long int DoubleLinkedList::findIndex(const ValueType& value) const
{
	size_t i = 0;

	while (i != _size)
	{
		if (getNode(i)->value == value)
			return i;
		++i;
	}
	std::cout << "There is no such value, so get an enormous index not from the list: ";
}

DoubleLinkedList::Node* DoubleLinkedList::findNode(const ValueType & value) const
{
	return getNode(findIndex(value));
}

void DoubleLinkedList::reverse()
{
	size_t cutSize; //уменьшенный в два раза размер
	if ((_size % 2) == 0)
	{
		cutSize = _size / 2;
	}
	else
	{
		cutSize = (_size - 1) / 2;
	}

	Node* swapper = nullptr;
	for (size_t idx = 0; idx < cutSize; ++idx)
	{
		swapper = getNode(idx);
		insert(_size - idx, swapper->value);
		write();
		writeTail();
		remove(idx);
		write();
		writeTail();

		swapper = getNode(_size - idx - 2);
		insert(idx, swapper->value);
		write();
		writeTail();
		remove(_size - idx - 2);
		write();
		writeTail();
	}
}

DoubleLinkedList DoubleLinkedList::reverse() const
{
	DoubleLinkedList bufList(*this);
	bufList.reverse();
	return bufList;
}

DoubleLinkedList DoubleLinkedList::getReverseList() const
{
	DoubleLinkedList bufList(*this);
	bufList.reverse();
	return bufList;
}

size_t DoubleLinkedList::size() const
{
	return _size;
}

void DoubleLinkedList::forceNodeDelete(Node* node)
{
	if (node == nullptr)
	{
		return; // если конец списка 
	}

	// удаляем всех впереди, пока не встретим nullptr
	Node* nextDeleteNode = node->next;
	delete node;
	forceNodeDelete(nextDeleteNode);
}

void DoubleLinkedList::write() const
{
	Node* bufNode = this->_head;
	while(bufNode)
	{
		std::cout << bufNode->value << "<>";
		bufNode = bufNode->next;
	}
	std::cout << "NULL" << std::endl;
}

void DoubleLinkedList::writeTail()
{
	std::cout << "Tail: " << _tail->value << std::endl;
	std::cout << "Tail previous: " << _tail->previous->value << std::endl;
}