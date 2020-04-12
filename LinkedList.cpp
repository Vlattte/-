#include "LinkedList.h"
#include <iostream>

#include <cassert>

//_______________________________________________________________________//

LinkedList::Node::Node(const ValueType& value, Node* next)
{
	this->value = value;
	this->next = next;
}

LinkedList::Node::~Node()
{

}

void LinkedList::Node::insertNext(const ValueType & value)
{
	Node* newNode = new Node(value, this->next); //создаем новый узел со значением value
	this->next = newNode;
}

void LinkedList::Node::removeNext()
{
	Node* removeNode = this->next;
	Node* newNext = removeNode->next;
	delete removeNode;
	this->next = newNext;
}

//_______________________________________________________________________//

LinkedList::LinkedList()
	:_head(nullptr), _size(0)
{
}

LinkedList::LinkedList(const LinkedList & copyList)
{
	this->_size = copyList._size;
	if (this->_size == 0)
	{
		this->_head = nullptr;
		return;
	}

	// головной узел этого узла = (выделяем память и вставляем туда значение головного узла)
	this->_head = new Node(copyList._head->value); 

	Node* currentNode = this->_head; 
	Node* currentCopyNode = copyList._head->next; 
	while (currentCopyNode)
	{
		// next для текущего узла = текущий скопированный узел
		currentNode->next = new Node(currentCopyNode->value);
		currentCopyNode = currentCopyNode->next;
		currentNode = currentNode->next;
	}

}

LinkedList& LinkedList::operator=(const LinkedList& copyList)
{
	if (this == &copyList)
	{
		return *this; //если копируем самого себя
	}

	this->_size = copyList._size;
	this->_head = copyList._head;

	return *this;
}

LinkedList::LinkedList(LinkedList && moveList) noexcept
{
	this->_size = moveList._size;
	this->_head = moveList._head;

	moveList._size = 0;
	moveList._head = nullptr;
}

LinkedList& LinkedList::operator=(LinkedList && movelist) noexcept
{
	if (this == &movelist)
	{
		return *this;	//если это мы, то возвращаем себя же
	}

	forceNodeDelete(_head); //рекурсивно удаляем this
	
	this->_size = movelist._size;
	this->_head = movelist._head;
	
	movelist._size = 0; 
	movelist._head = nullptr; 

	return *this; //возвращаем себя скопированного

}

LinkedList::~LinkedList()
{
	forceNodeDelete(_head);
}

//_______________________________________________________________________//

ValueType & LinkedList::operator[](const size_t pos) const
{
	return getNode(pos)->value;
}

LinkedList::Node* LinkedList::getNode(const size_t pos) const
{
	if (pos < 0)
	{
		assert(pos < 0); //ошибка: размер меньше нуля
	}
	else if (pos >= this->_size)
	{
		assert(pos >= this->_size); //ошибка: вышли за границу списка
	}

	Node* bufNode = this->_head; // создаем буфер для хранения головного узла
	for (int i = 0; i < pos; i++)
	{
		bufNode = bufNode->next; //перебираем элементы до нужного
	}

	return bufNode;
}

void LinkedList::insert(const size_t pos, const ValueType & value)
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
	else //иначе перебираем узлы и вставляем на нужную позицию
	{
		Node* bufNode = this->_head;
		for (size_t i = 0; i < pos - 1; ++i)
		{
			bufNode = bufNode->next;
		}
		bufNode->insertNext(value);
		++_size;
	}
}

void LinkedList::insertAfterNode(Node* node, const ValueType & value)
{
	node->insertNext(value); 
}

void LinkedList::pushBack(const ValueType & value)
{
	if (_size == 0)
	{
		pushFront(value);
		return;
	}
	insert(_size, value); 
}

void LinkedList::pushFront(const ValueType & value)
{
	_head = new Node(value, _head); // перезаписываем головной узел
	++_size;
}

void LinkedList::remove(const size_t pos)
{
	if (pos < 0)
	{
		assert(pos < 0); //ошибка: размер меньше нуля
	}
	else if (pos > this->_size)
	{
		assert(pos > this->_size); //ошибка: вышли за границу списка
	}
	
	if (pos == 0)
	{
		this->_head = this->_head->next;
	}
	else
	{
		Node* bufNode = this->_head;
		while (bufNode->next != this->getNode(pos))
		{
			bufNode = bufNode->next;
		}
		bufNode->removeNext();
	}

	_size--;
}

void LinkedList::removeNextNode(Node* node)
{
	size_t pos = findIndex(node->value);
	this->remove(pos);
}

void LinkedList::removeFront()
{
	this->remove(0);
}

void LinkedList::removeBack()
{
	this->remove(_size - 1);
}

long long int LinkedList::findIndex(const ValueType& value) const
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

 LinkedList::Node* LinkedList::findNode(const ValueType & value) const
{
	return getNode(findIndex(value));
}

 void LinkedList::reverse() 
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
		insert(_size - idx - 1, swapper->value); 
		remove(idx);   

		swapper = getNode(_size - idx - 1);
		insert(idx, swapper->value);
		remove(_size - idx - 1);
	}
}

 LinkedList LinkedList::reverse() const
 {
	 LinkedList bufList(*this);
	 bufList.reverse();
	 return bufList;
 }

 LinkedList LinkedList::getReverseList() const
 {
	 LinkedList bufList(*this);
	 bufList.reverse();
	 return bufList;
 }

size_t LinkedList::size() const
{
	return _size;
}

void LinkedList::forceNodeDelete(Node* node)
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

void LinkedList::write() const
{
	Node* bufNode = this->_head;
	for (size_t i = 0; i < _size; ++i)
	{
		std::cout << bufNode->value << "<>";
		bufNode = bufNode->next;
	}
	std::cout << "NULL";
	std::cout << std::endl;
	delete[] bufNode;
	bufNode = NULL;
}
