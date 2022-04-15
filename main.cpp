#include <iostream>
#include <string>
#include <deque>

#if 0
	# include <map>
	# include <stack>
	# include <vector>
	namespace ft = std;
#else
	# include "map.hpp"
	# include "stack.hpp"
	# include "vector.hpp"
#endif

#include <stdlib.h>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096
struct Buffer {
	int idx;
	char buff[BUFFER_SIZE];
};

#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template< class BiIterator >
BiIterator getNext(BiIterator it, int n)
{
	while (--n)
		++it;
	return it;
}

template< typename T >
class MutantStack : public ft::stack<T> {
public:
	MutantStack() { }
	MutantStack(const MutantStack<T>& src) { *this = src; }
	MutantStack<T>& operator=(const MutantStack<T>& rhs)
	{
		this->c = rhs.c;
		return *this;
	}
	~MutantStack() { }

	typedef typename ft::stack<T>::container_type::iterator iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
};

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	const int seed = atoi(argv[1]);
	srand(seed);

	ft::vector<std::string> vector_str;
	ft::vector<int> vector_int;
	ft::stack<int> stack_int;
	ft::vector<Buffer> vector_buffer;
	ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
	ft::map<int, int> map_int;

	for (int i = 0; i<COUNT; i++) {
		vector_buffer.push_back(Buffer());
	}

	for (int i = 0; i<COUNT; i++) {
		const int idx = rand()%COUNT;
		vector_buffer[idx].idx = 5;
	}
	ft::vector<Buffer>().swap(vector_buffer);

	try {
		for (int i = 0; i<COUNT; i++) {
			const int idx = rand()%COUNT;
			vector_buffer.at(idx);
			std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" << std::endl;
		}
	}
	catch (const std::exception& e) {
		//NORMAL ! :P
	}

	for (int i = 0; i<COUNT; ++i) {
		map_int.insert(ft::make_pair(rand(), rand()));
	}

	int sum = 0;
	for (int i = 0; i<10000; i++) {
		int access = rand();
		sum += map_int[access];
	}
	std::cout << "should be constant with the same seed or should be zero: " << sum << std::endl;

	{
		ft::map<int, int> copy = map_int;
	}
	MutantStack<char> iterable_stack;
	for (char letter = 'a'; letter<='z'; letter++)
		iterable_stack.push(letter);
	for (MutantStack<char>::iterator it = iterable_stack.begin(); it!=iterable_stack.end(); it++) {
		std::cout << *it;
	}
	std::cout << std::endl;

	std::cout << "Checking vector" << std::endl << std::endl;

	vector_int.clear();
	for (int i = 1; i<100; ++i)
		vector_int.push_back(i*(3%i)+12);
	std::cout << "vector 5th element: " << vector_int[5] << " ";
	std::cout << "vector 23th element: " << vector_int[23] << " ";
	std::cout << "vector 55th element: " << vector_int[55] << std::endl;
	ft::vector<int> second_vector_int = vector_int;
	std::cout << "vector reassigned" << std::endl;
	std::cout << "vector 5th element: " << second_vector_int[5] << " ";
	std::cout << "vector 23th element: " << second_vector_int[23] << " ";
	std::cout << "vector 55th element: " << second_vector_int[55] << std::endl;
	if (vector_int!=second_vector_int)
		throw ("Vectors should be equal");

	ft::vector<int>::iterator it = vector_int.begin();
	ft::vector<int>::const_iterator it2 = second_vector_int.begin();

	ft::vector<int>::const_iterator ite = vector_int.end();
	ft::vector<int>::iterator ite2 = second_vector_int.begin();
	while (it!=ite && it2!=ite2) {
		if (*it!=*it2)
			throw ("Vector' iterators should be equal");
		++it;
		++it2;
	}

	std::cout << "sizes v1: " << vector_int.size() << " v2: " << second_vector_int.size() << std::endl;
	std::cout << "max_sizes v1: " << vector_int.max_size() << " v2: " << second_vector_int.max_size() << std::endl;

	if (vector_int.front()!=second_vector_int.front())
		throw ("front elements not equal");

	if (vector_int.back()!=second_vector_int.back())
		throw ("front elements not equal");

	vector_int.assign(10000, 100);
	int lastVectorElement = vector_int.back();
	vector_int.pop_back();
	std::cout << "vector pop" << lastVectorElement << std::endl;
	std::cout << "vector size" << vector_int.size() << std::endl;
	vector_int.erase(vector_int.begin(), vector_int.begin()+400);
	vector_int.erase(vector_int.begin());
	std::cout << "vector size after remove some elements" << vector_int.size() << std::endl;
	if (vector_int>second_vector_int) {
		std::cout << "now first vector is bigger than second" << std::endl;
	}
	else {
		std::cout << "now second vector is bigger than first" << std::endl;
	}

	std::cout << "Checking stack" << std::endl << std::endl;

	std::cout << "stack is empty?" << stack_int.empty() << std::endl;
	stack_int.push(2342);
	std::cout << "top element" << stack_int.top() << std::endl;
	stack_int = ft::stack<int>();
	std::cout << "stack is empty?" << stack_int.empty() << std::endl;
	for (int i = 1; i<100; ++i)
		stack_int.push(i*(99%i)+1838);
	int topElement = stack_int.top();
	stack_int.pop();
	std::cout << "popped element" << topElement << std::endl;
	ft::stack<int> second_stack_int = stack_int;

	if (stack_int!=second_stack_int)
		throw ("Stacks should be equal");

	std::cout << "sizes v1: " << vector_int.size() << " v2: " << second_vector_int.size() << std::endl;
	std::cout << "max_sizes v1: " << vector_int.max_size() << " v2: " << second_vector_int.max_size() << std::endl;

	second_stack_int.pop();
	if (stack_int>second_stack_int) {
		std::cout << "now first stack is bigger than second" << std::endl;
	}
	else {
		std::cout << "now second stack is bigger than first" << std::endl;
	}

	std::cout << "Checking map" << std::endl << std::endl;
	std::cout << "Map size " << map_int.size() << std::endl;
	map_int.clear();
	std::cout << "Map size after clear" << map_int.size() << std::endl;
	for (int i = 1; i<1000; ++i)
		map_int.insert(ft::make_pair(i, 43*(-i+500)));
	std::cout << "map 5th element: " << map_int[5] << " ";
	std::cout << "map 854th element: " << map_int[854] << " ";
	std::cout << "map 542th element: " << map_int[542] << " ";

	ft::map<int, int> second_map_int = map_int;
	std::cout << "map 5th element: " << second_map_int[5] << " ";
	std::cout << "map 854th element: " << second_map_int[854] << " ";
	std::cout << "map 542th element: " << second_map_int[542] << " " << std::endl;

	ft::map<int, int>::iterator mit = map_int.begin();
	ft::map<int, int>::const_iterator mit2 = second_map_int.begin();

	ft::map<int, int>::const_iterator mite = map_int.end();
	ft::map<int, int>::iterator mite2 = second_map_int.begin();
	while (mit!=mite && mit2!=mite2) {
		if (*mit!=*mit2)
			throw ("Map's iterators should be equal");
		++mit;
		++mit2;
	}

	if (map_int!=second_map_int)
		throw ("Maps should be equal");

	map_int[434] = 333;

	if (map_int==second_map_int)
		throw ("Maps shouldn't be equal");

	map_int.erase(434);

	std::cout << "Map size before clear " << map_int.size() << std::endl;
	map_int.erase(++(++(map_int.begin())));
	std::cout << "Map size erase clear " << map_int.size() << std::endl;
	map_int.erase(getNext(map_int.begin(), 10), getNext(map_int.begin(), 30));
	std::cout << "Map size erase clear " << map_int.size() << std::endl;

	if (map_int>second_map_int) {
		std::cout << "now first vector is bigger than second" << std::endl;
		map_int.swap(second_map_int);
		if (map_int>second_map_int)
			throw ("Swap didn't work");
	}

	map_int[5555] = 123;
	mit = map_int.find(5555);
	std::cout << "try find 5555 element :" << mit->first << " " << mit->second << std::endl;

	std::cout << "count of 5555 elements :" << map_int.count(5555) << std::endl;
	std::cout << "count of 5565 elements :" << map_int.count(5565) << std::endl;

	mit = map_int.lower_bound(5000);
	ft::map<int, int>::iterator upper = map_int.upper_bound(5000);
	ft::pair<ft::map<int, int>::iterator, ft::map<int, int>::iterator> result = map_int.equal_range(5000);
	ft::map<int, int>::iterator res1 = result.first;
	ft::map<int, int>::iterator res2 = result.second;

	std::cout << "lower bound element: " << mit->first << " " << mit->second << std::endl;
	std::cout << "upper bound element: " << upper->first << " " << upper->second << std::endl;
	std::cout << "equal range lower: " << res1->first << " " << res1->second << std::endl;
	std::cout << "equal range upper: " << res2->first << " " << res2->second << std::endl;

	return (0);
}
