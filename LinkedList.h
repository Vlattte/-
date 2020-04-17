#pragma once

using ValueType = double;

class LinkedList
{
public:
	//класс узла списка
	//по своей сути, может содержать любые данные,
	//можно реализовать и ассоциативный массив, просто добавив
	//поле с ключем в узел и, с учетом этого, поменять методы LinkedList
	//(доступ по ключу, поиск по ключу и т.д.)
	struct Node
	{
		Node(const ValueType& value, Node* next = nullptr);
		~Node();

		void insertNext(const ValueType& value);
		void removeNext();

		ValueType value;
		Node* next;
	};

public:
	////
	LinkedList(); //конструктор по умолчанию
	LinkedList(const LinkedList& copyList); //конструктор копирования
	LinkedList& operator=(const LinkedList& copyList); 

	LinkedList(LinkedList&& moveList) noexcept; // перемещение с последующим очищением старого
	LinkedList& operator=(LinkedList&& movelist) noexcept;
	~LinkedList();
	////

	// доступ к значению элемента по индексу
	ValueType& operator[](const size_t pos) const;
	//доступ к узлу по индексу
	LinkedList::Node* getNode(const size_t pos) const;

	//вставка элемента по индексу, сначала ищем, куда вставлять (O(n)), потом вставляем (O(1))
	void insert(const size_t pos, const ValueType& value);
	//вставка элемента после узла, (O(1))
	static void insertAfterNode(Node* node, const ValueType& value);
	//вставка в конец (O(n))
	void pushBack(const ValueType& value);
	//вставка в начало (O(1))
	void pushFront(const ValueType& value);

	// удаление
	void remove(const size_t pos);
	void removeNextNode(Node* node);
	void removeFront();
	void removeBack();

	//поиск, (O(n))
	long long int findIndex(const ValueType& value) const; //поиск индекса узла с этим value
	Node* findNode(const ValueType& value) const; //поиск узла с таким value

	//разворот списка
	void reverse();
	LinkedList reverse() const;
	LinkedList getReverseList() const;

	size_t size() const;

	void write() const; //метод написания списка

private:
	Node* _head;
	size_t _size;

	void forceNodeDelete(Node* node);
};

