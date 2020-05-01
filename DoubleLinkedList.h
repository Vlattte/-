#pragma once

using ValueType = double;

class DoubleLinkedList
{
public:
	//класс узла списка
	//по своей сути, может содержать любые данные,
	//можно реализовать и ассоциативный массив, просто добавив
	//поле с ключем в узел и, с учетом этого, поменять методы LinkedList
	//(доступ по ключу, поиск по ключу и т.д.)
	struct Node
	{
		Node(const ValueType& value, Node* next = nullptr, Node* previous = nullptr); //done
		~Node();																	  //done

		void insertNext(const ValueType& value);									  //done
		void insertPrevious(const ValueType& value);								  //done
		void removeNext();									  						  //done
		void removePrevious();                                                        //done

		ValueType value;									                          
		Node* next;
		Node* previous;
	};

public:
	//==========================================================//
	DoubleLinkedList(); //конструктор по умолчанию									  //done
	DoubleLinkedList(const DoubleLinkedList& copyList); //конструктор копирования     //done
	DoubleLinkedList& operator=(const DoubleLinkedList& copyList);                    //done

	DoubleLinkedList(DoubleLinkedList&& moveList) noexcept; // перемещение с последующим очищением старого  //done
	DoubleLinkedList& operator=(DoubleLinkedList&& movelist) noexcept;				  //done
	~DoubleLinkedList();															  //done
	//==========================================================//

	// доступ к значению элемента по индексу
	ValueType& operator[](const size_t pos) const;									  //done
	//доступ к узлу по индексу
	DoubleLinkedList::Node* getNode(const size_t pos) const;						  //done

	//вставка элемента по индексу, сначала ищем, куда вставлять (O(n)), потом вставляем (O(1))
	void insert(const size_t pos, const ValueType& value);							  //done
	//вставка элемента после узла, (O(1))
	static void insertAfterNode(Node* node, const ValueType& value);	              //done
	//вставка элемента перед узлом
	static void insertBeforeNode(Node* node, const ValueType& value);		          //done
	//вставка в конец (O(n))
	void pushBack(const ValueType& value);									          //done
	//вставка в начало (O(1))
	void pushFront(const ValueType& value);									          //done

	// удаление
	void remove(const size_t pos);									                  //done
	void removeNextNode(Node* node);									              //done
	void removePreviousNode(Node* node);									          //done
	void removeFront();									                              //done
	void removeBack();									                              //done

	//поиск, (O(n))
	long long int findIndex(const ValueType& value) const; //поиск индекса узла с этим value //done
	Node* findNode(const ValueType& value) const;          //поиск узла с таким value        //done

	//разворот списка
	void reverse();									                                  //done
	DoubleLinkedList reverse() const;									              //done
	DoubleLinkedList getReverseList() const;									      //done

	size_t size() const;									                          //

	void write() const; //метод написания списка									  //
	void writeTail();
private:
	Node* _head;
	Node* _tail;
	size_t _size;

	void forceNodeDelete(Node* node);									              //
};

