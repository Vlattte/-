#include <iostream>
#include "LinkedList.h"

using namespace std;

int main()
{
	LinkedList list;
	list.insert(0, 3);
	list.insert(0, 1);
	list.insert(1, 5);
	list.insert(1, 4);
	list.insert(2, 47);
	list.write();

	//LinkedList l;
	//l = list.getReverseList();
	//list.insert(2, 444);
	//l.write();
	list.reverse();
	list.write();

	int n;
	cin >> n;
}