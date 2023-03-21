#include "map.hpp"
#include <string>
#include <vector>
#include "allocator.hpp"
#include "gtest/gtest.h"


TEST(MapTest, InsertTest) {
	using mymap = avl::MyMap<int, std::string>;
	mymap m;
	std::vector<std::string> values = { "a", "b", "c", "d" };
	for (int i = 0; i < 4; ++i) {
		auto [it, success] = m.insert({ i, values[i] });
		if (!success)
			FAIL();
	}

	// ключ 2 уже есть в m => вставки не будет
	auto [it, success] = m.insert({ 2, "e"});
	if (success)
		FAIL();

	std::vector<mymap::value_type> target = { {0, "a"}, {1, "b"}, {2, "c"}, {3, "d"} };
	std::vector<mymap::value_type> result;
	// работаспособность range-based for цикла доказывает корректность моих итераторов
	for (auto val : m)
		result.push_back(val);
	ASSERT_EQ(result, target);
}

TEST(MapTest, IteratorsTest) {
	using mymap = avl::MyMap<int, std::string>;
	mymap m;
	std::vector<std::string> values = { "a", "b", "c", "d" };
	for (int i = 0; i < 4; ++i) 
		auto [it, success] = m.insert({ i, values[i] });

	ASSERT_EQ(m.begin()->second, "a");
	ASSERT_EQ(m.rbegin()->second, "d");

	// итерирование слева направо было проверено в прошлом тесте, теперь будет справа налево
	std::vector<mymap::value_type> target = { {3, "d"}, {2, "c"}, {1, "b"}, {0, "a"} };
	std::vector<mymap::value_type> result;
	for (auto it = m.rbegin(); it != m.rend(); ++it)
		result.push_back(*it);
	ASSERT_EQ(result, target);
}


TEST(MapTest, EraseTest) {
	using mymap = avl::MyMap<int, std::string>;
	mymap m;
	std::vector<std::string> values = { "a", "b", "c", "d" };
	for (int i = 0; i < 4; ++i)
		auto [it, success] = m.insert({ i, values[i] });

	size_t success = m.erase(10);
	if (success)
		FAIL(); // ключа 10 нет 

	success = m.erase(2);
	if (!success)
		FAIL(); // ключ 2 есть

	success = m.erase(2);
	if (success)
		FAIL(); // ключа 2 уже нет

	std::vector<mymap::value_type> target = { {0, "a"}, {1, "b"}, {3, "d"} };
	std::vector<mymap::value_type> result;
	for (auto val : m)
		result.push_back(val);
	ASSERT_EQ(result, target);
}


TEST(MapTest, FindTest) {
	using mymap = avl::MyMap<int, std::string>;
	mymap m;
	std::vector<std::string> values = { "a", "b", "c", "d" };
	for (int i = 0; i < 4; ++i)
		auto [it, success] = m.insert({ i, values[i] });

	mymap::iterator it = m.find(5);
	if (it != m.end())
		FAIL(); // ключа 5 нет

	it = m.find(1);
	if (it == m.end())
		FAIL(); // ключ 1 есть

	ASSERT_EQ(it->second, "b");
}



TEST(MapTest, ContainsTest) {
	using mymap = avl::MyMap<int, std::string>;
	mymap m;
	std::vector<std::string> values = { "a", "b", "c", "d" };
	for (int i = 0; i < 4; ++i)
		auto [it, success] = m.insert({ i, values[i] });


	ASSERT_EQ(m.contains(10), false);
	ASSERT_EQ(m.contains(2), true);
}


TEST(MapTest, EmptyTest) {
	using mymap = avl::MyMap<int, std::string>;
	mymap m;

	ASSERT_EQ(m.empty(), true);

	std::vector<std::string> values = { "a", "b", "c", "d" };
	for (int i = 0; i < 4; ++i)
		auto [it, success] = m.insert({ i, values[i] });

	ASSERT_EQ(m.empty(), false);
}



TEST(MapTest, SizeTest) {
	using mymap = avl::MyMap<int, std::string>;
	mymap m;
	std::vector<std::string> values = { "a", "b", "c", "d" };
	for (int i = 0; i < 4; ++i) {
		auto [it, success] = m.insert({ i, values[i] });
		ASSERT_EQ(m.size(), i + 1);
	}

	m.erase(0);
	ASSERT_EQ(m.size(), 3);
}



TEST(MapTest, ClearTest) {
	using mymap = avl::MyMap<int, std::string>;
	mymap m;
	std::vector<std::string> values = { "a", "b", "c", "d" };
	for (int i = 0; i < 4; ++i)
		auto [it, success] = m.insert({ i, values[i] });

	m.clear();
	ASSERT_EQ(m.empty(), true);
}


TEST(MapTest, OperatorIndexTest) {
	using mymap = avl::MyMap<int, std::string>;
	mymap m;
	std::vector<std::string> values = { "a", "b", "c", "d" };
	for (int i = 0; i < 4; ++i)
		m[i] = values[i];

	std::vector<mymap::value_type> target = { {0, "a"}, {1, "b"}, {2, "c"}, {3, "d"} };
	std::vector<mymap::value_type> result;
	for (auto val : m)
		result.push_back(val);
	ASSERT_EQ(result, target);

	m[1] = "B";

	ASSERT_EQ(m[1], "B");
}



TEST(MapTest, AtTest) {
	using mymap = avl::MyMap<int, std::string>;
	mymap m;
	std::vector<std::string> values = { "a", "b", "c", "d" };
	for (int i = 0; i < 4; ++i)
		m[i] = values[i];

	ASSERT_EQ(m.at(2), "c");

	try {
		m.at(10);
	}
	catch (std::out_of_range&) {
		SUCCEED();
	}
	catch (...) {
		FAIL();
	}
}



TEST(MapTest, CompareTest) {
	using mymap = avl::MyMap<int, std::string, std::greater<int>>; // !!!
	mymap m;
	std::vector<std::string> values = { "a", "b", "c", "d" };
	for (int i = 0; i < 4; ++i)
		m[i] = values[i];

	std::vector<mymap::value_type> target = { {3, "d"}, {2, "c"}, {1, "b"}, {0, "a"} };
	std::vector<mymap::value_type> result;
	for (auto el: m)
		result.push_back(el);
	ASSERT_EQ(result, target);
}



TEST(MapTest, AllocatorTest) {
	using mymap = avl::MyMap<int, std::string, std::less<int>,
		myalloc::allocator<avl::Node<std::pair<const int, std::string>>>>; // !!!
	mymap m;
	std::vector<std::string> values = { "a", "b", "c", "d" };
	for (int i = 0; i < 4; ++i)
		m[i] = values[i];

	std::vector<mymap::value_type> target = { {0, "a"}, {1, "b"}, {2, "c"}, {3, "d"} };
	std::vector<mymap::value_type> result;
	for (auto val : m)
		result.push_back(val);
	ASSERT_EQ(result, target);
}


int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

