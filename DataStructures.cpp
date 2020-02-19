//============================================================================
// Name        : DataStructures.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <limits>

#include <sstream>
#include "include/ArrayArrayList.hpp"

std::string printList(const ArrayList<int>& list)
{
	std::stringstream ss;

	for(std::size_t i = 0; i < list.size(); ++i)
	{
		ss << list.at(i) << " ";
	}

	ss << std::endl;

	return ss.str();
}

int main()
{
	ArrayList<int> myList;
	ArrayList<int> myList2;
	ArrayList<int> myList3;
	myList3.push_back(1);
	myList3.push_back(2);
	myList3.push_back(3);

	myList3.insert(3, 1);

	printList(myList3);

	printf("data=[%d] [%s]\n", myList3.at(0), printList(myList3).c_str());
	printf("data=[%d] [%s]\n", myList3.front(), printList(myList3).c_str());
	printf("data=[%d] [%s]\n", myList3.back(), printList(myList3).c_str());
	printf("data=[%d] [%s]\n", myList3.pop_front(), printList(myList3).c_str());
	printf("data=[%d] [%s]\n", myList3.pop_back(), printList(myList3).c_str());

//	ArrayList<int> myList4(ArrayList<int>());
//
	myList = myList3;
	myList.size();
	myList.max_size();
	myList.capacity();
	myList.empty();
	int i = myList[0];
	const int j = myList[1];
//
	myList.at(0);
	myList.at(1);
//
	myList.front();
	myList.back();
//
	myList.data();

	myList.push_front(-1);
	myList.push_back(1);

	myList.pop_back();
	myList.pop_front();

	myList.insert(1, 1);
	myList.replace(1, 2);
//
//	myList.erase(0);
//	myList.contains(99);
//	printf("Worked [%d]", myList.at(100));

	return 0;
}
