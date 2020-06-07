#include <iostream>
#include <cassert>
#include <exception>
#include "LinkedList.h"

using ValueType = double;
//==================================================================================================//

LinkedList::Node::Node(const ValueType& value, Node* next)
{
	this->value = value;
	this->next = next;
}

LinkedList::Node::~Node()
{
	//nothing here
}

void LinkedList::Node::insertNext(const ValueType & value)
{
	Node* newNode = new Node(value, this->next); //������� ����� ���� �� ��������� value
	this->next = newNode;
}

void LinkedList::Node::removeNext()
{
	Node* removeNode = this->next;				//[0, 1 , 2, 3]
	Node* newNext = removeNode->next;
	delete removeNode;
	this->next = newNext;
}

//==================================================================================================//

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

	// �������� ���� ����� ���� = (�������� ������ � ��������� ���� �������� ��������� ����)
	this->_head = new Node(copyList._head->value);

	Node* currentNode = this->_head;
	Node* currentCopyNode = copyList._head->next;
	while (currentCopyNode)
	{
		// next ��� �������� ���� = ������� ������������� ����
		currentNode->next = new Node(currentCopyNode->value);
		currentCopyNode = currentCopyNode->next;
		currentNode = currentNode->next;
	}

}

LinkedList& LinkedList::operator=(const LinkedList& copyList)
{
	if (this == &copyList)
	{
		return *this; //���� �������� ������ ����
	}

	this->_size = copyList._size;
	this->_head = new Node(copyList._head->value);

	Node* currentNode = this->_head;
	Node* currentCopyNode = copyList._head->next;
	while (currentCopyNode)
	{
		// next ��� �������� ���� = ������� ������������� ����
		currentNode->next = new Node(currentCopyNode->value);
		currentCopyNode = currentCopyNode->next;
		currentNode = currentNode->next;
	}
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
		return *this;	//���� ��� ��, �� ���������� ���� ��
	}

	forceNodeDelete(_head); //������� this

	this->_size = movelist._size;
	this->_head = movelist._head;

	movelist._size = 0;
	movelist._head = nullptr;

	return *this; //���������� ���� ��������������
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
		assert(pos < 0); //������: ������ ������ ����
	}
	else if (pos >= this->_size)
	{
		assert(pos >= this->_size); //������: ����� �� ������� ������
	}

	Node* bufNode = this->_head; // ������� ����� ��� �������� ��������� ����
	for (size_t i = 0; i < pos; i++)
	{
		bufNode = bufNode->next; //���������� �������� �� �������
	}

	return bufNode;
}

void LinkedList::insert(const size_t pos, const ValueType & value)
{
	if (pos < 0)
	{
		assert(pos < 0); //������: ������ ������ ����
	}
	else if (pos > this->_size)
	{
		assert(pos > this->_size); //������: ����� �� ������� ������
	}

	if (pos == 0) //���� ����� �������� � ������ ������, �� pushFront
	{
		pushFront(value);
	}
	else //����� ���������� ���� � ��������� �� ������ �������
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
	_size++;
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
	_head = new Node(value, _head); // �������������� �������� ����
	++_size;
}

void LinkedList::remove(const size_t pos)
{
	if (pos < 0)
	{
		assert(pos < 0); //������: ������ ������ ����
	}
	else if (pos > this->_size)
	{
		assert(pos > this->_size); //������: ����� �� ������� ������
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
	if (node->next == nullptr)
	{
		throw std::out_of_range("Invalid index");
	}
	node->removeNext();
	_size--;
}

void LinkedList::removeFront()
{
	if (_head == nullptr)
	{
		return;
	}
	this->remove(0);
}

void LinkedList::removeBack()
{
	this->remove(_size - 1);
}

long long int LinkedList::findIndex(const ValueType& value) const
{
	Node* buff = this->_head;
	for (long long int i = 0; i < _size; i++)
	{
		if (buff->value == value)
			return i;
		buff = buff->next;
	}
	return -1;
}

LinkedList::Node* LinkedList::findNode(const ValueType & value) const
{
	Node* buff = this->_head;
	for (long long int i = 0; i < _size; i++)
	{
		if (buff->value == value)
			return buff;
		buff = buff->next;
	}
	return nullptr;
}

void LinkedList::reverse()
{
	size_t cutSize; //����������� � ��� ���� ������
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
	LinkedList* list = new LinkedList;
	*list = *this;
	list->reverse();

	return *list;

}

size_t LinkedList::size() const
{
	return _size;
}

void LinkedList::forceNodeDelete(Node* node)
{
	if (node == nullptr)
	{
		return;
	}

	Node* nextDeleteNode = node->next;
	delete node;
	forceNodeDelete(nextDeleteNode);

	/*
	size_t pos = findIndex(node->value);
	if (node == nullptr)
	{
		return;
	}

	while (node != nullptr)
	{
		remove(pos);
		node = node->next;
		pos++;
	}
	*/
}

void LinkedList::write() const
{
	Node* bufNode = this->_head;
	while (bufNode)
	{
		std::cout << bufNode->value << "<>";
		bufNode = bufNode->next;
	}
	std::cout << "NULL" << std::endl;
}
