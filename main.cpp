#include <iostream>
#include "Vector.h"

using namespace std;


int main()
{

	MyVector vec1 = {0, 0, ResizeStrategy::Multiplicative, 2};

	vec1.insert(0, 4);
	vec1.write();
	vec1.insert(0, 3);
	vec1.write();
	vec1.insert(0, 2);
	vec1.write();
	vec1.insert(0, 1);
	vec1.write();
	vec1.insert(0, 0);
	vec1.write();
	vec1.insert(0, -1);
	vec1.write();
	vec1.insert(0, -2);
	vec1.write();
	vec1.insert(0, -3);
	vec1.write();
	vec1.insert(0, -4);
	vec1.write();
	vec1.insert(0, -5);
	vec1.write();


	vec1.write();
	int n;
	cin >> n;
}