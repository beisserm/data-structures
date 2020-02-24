#include "../include/ArrayList.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(DataStructures)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
	ArrayList<std::string> testList;
	BOOST_CHECK(testList == testList);
}

BOOST_AUTO_TEST_CASE(InitializerListConstructor)
{
	ArrayList<std::string> testList{"A", "B", "C"};

	std::string value;
	for(size_t i = 0; i < testList.size(); ++i)
	{
		value += testList.at(i);
	}

	BOOST_CHECK(value == "ABC");
}

BOOST_AUTO_TEST_CASE(UserDefinedConstructor)
{
	std::string defaults[3] = {"A", "B", "C"};
	ArrayList<std::string> testList(defaults, 3);

	for(size_t i = 0; i < 3; ++i)
	{
		defaults[i] = "";
	}

	std::string value;
	for(size_t i = 0; i < testList.size(); ++i)
	{
		value += testList.at(i);
	}

	BOOST_CHECK(value == "ABC");
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
	ArrayList<int> testList1{1, 2};
	ArrayList<int> testList2 = testList1;

	BOOST_CHECK(testList1 == testList2);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
	ArrayList<int> testList2(std::move(ArrayList<int>({1, 1, 1})));

	int val = 0;
	for(size_t i = 0; i < 3; ++i)
	{
		val += testList2[i];
	}

	BOOST_CHECK(val == 3);
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
	ArrayList<int> testList;
	testList = ArrayList<int>({2, 2, 2});

	int val = 0;
	for(size_t i = 0; i < 3; ++i)
	{
		val += testList.at(i);
	}

	BOOST_CHECK(val == 6);
}

BOOST_AUTO_TEST_CASE(Front)
{
	int temp = 2;
	ArrayList<int> testList;
	testList.push_front(1);
	testList.push_front(temp);

	int val1 = testList.front();
	int& val2 = testList.front();

	BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(BACK)
{
	int temp = 2;
	ArrayList<int> testList;
	testList.push_back(1);
	testList.push_back(temp);

	int val1 = testList.back();
	int& val2 = testList.back();

	BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(PopFront)
{
	ArrayList<int> testList;
	testList.push_front(0);
	int val = testList.pop_front();

	BOOST_CHECK(static_cast<int>(testList.size()) == val);
}

BOOST_AUTO_TEST_CASE(PopBack)
{
	ArrayList<int> testList;
	testList.push_back(0);
	int val = testList.pop_back();

	BOOST_CHECK(static_cast<int>(testList.size()) == val);
}

BOOST_AUTO_TEST_CASE(Insert)
{
	ArrayList<std::string> testList;
	testList.insert("zero", 0);
	testList.insert("one", 1);
	testList.insert("two", 2);
	testList.insert("zero", 0);

	testList.erase(0);
	testList.remove("two");
	testList.erase(1);
	testList.erase(0);

	BOOST_CHECK(testList.size() == 0);
}

BOOST_AUTO_TEST_CASE(Replace)
{
	ArrayList<int> testList {0, 1, 2};
	testList.replace(3, 0);
	testList.replace(2, 1);
	testList.replace(1, 2);

	int val = 0;
	for(size_t i = 0; i < 3; ++i)
	{
		val += testList[i];
	}

	BOOST_CHECK(val == 6);
}


BOOST_AUTO_TEST_CASE(Erase)
{
	ArrayList<int> testList {10, 11, 12};
	testList.erase(2);
	testList.erase(1);
	testList.erase(0);

	BOOST_CHECK(testList.size() == 0);
}

BOOST_AUTO_TEST_CASE(Find)
{
	ArrayList<int> testList {0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	size_t index = testList.find(1);

	BOOST_CHECK(index == 9);
}

BOOST_AUTO_TEST_CASE(Contains)
{
	ArrayList<int> testList {0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	bool have = testList.contains(1);

	BOOST_CHECK(have == true);
}

BOOST_AUTO_TEST_CASE(Equal)
{
	ArrayList<int> testList1 {0, 0};
	ArrayList<int> testList2 {0, 0};

	BOOST_CHECK(testList1 == testList2);
}

BOOST_AUTO_TEST_CASE(EqualEmpty)
{
	ArrayList<int> testList1 {};
	ArrayList<int> testList2 {};

	BOOST_CHECK(testList1 == testList2);
}

BOOST_AUTO_TEST_CASE(EqualLeftShort)
{
	ArrayList<int> testList1 {0};
	ArrayList<int> testList2 {0, 0};

	BOOST_CHECK(testList1 != testList2);
}

BOOST_AUTO_TEST_CASE(EqualRightShort)
{
	ArrayList<int> testList1 {0, 0};
	ArrayList<int> testList2 {0};

	BOOST_CHECK(testList1 != testList2);
}

BOOST_AUTO_TEST_CASE(LessThanLeftShorter)
{
	ArrayList<int> testList1 {0, 1};
	ArrayList<int> testList2 {0, 1, 2};

	BOOST_CHECK(testList1 < testList2);
}

BOOST_AUTO_TEST_CASE(GreaterThan)
{
	ArrayList<int> testList1 {0, 1};
	ArrayList<int> testList2 {0};

	BOOST_CHECK(testList1 > testList2);
}

BOOST_AUTO_TEST_SUITE_END()
