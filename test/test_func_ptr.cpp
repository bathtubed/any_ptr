#include "func_ptr.hpp"
#include <stdlib.h>
#include <iostream>
#include <string>

namespace std
{
	template<class T, class... Args>
	unique_ptr<T> make_unique(Args... args)
	{
		return unique_ptr<T>(new T(args...));
	}
}

class Test
{
public:
	std::string data_;
	
public:
	Test(std::string init): data_(std::move(init)) {std::cout << "Constructing " << data_ << std::endl;}
	Test(const Test& other): data_(other.data_) {std::cout << "Copy Constructing " << data_ << std::endl;}
	Test(Test&& other): data_(std::move(other.data_)) {std::cout << "Move Constructing" << data_ << std::endl;}
	~Test() {std::cout << "Destructing " << data_ << std::endl;}
};

int main(void)
{
	using namespace edward;
	using std::cout;
	using std::endl;
	
	auto test_func = [](auto&& ptr_obj)
	{
		func_ptr<Test> init(std::move(ptr_obj));
		func_ptr<Test> move(std::move(init));
		init = std::move(move);
		func_ptr<Test> copy(init);
		init = copy;
		std::cout << "init: " << init << std::endl;
		std::cout << "copy: " << copy << std::endl;
	}	
	
	test_func(new Test("raw_heap_mem"));
	test_func(std::make_shared<Test>("shared_ptr"));
	test_func(std::make_unique<Test>("unique_ptr"));
	test_func("stack_mem");
	
	/*
	func_ptr<int> shared(std::make_shared<int>(10));
	func_ptr<int> unique(std::make_unique<int>(8));
	func_ptr<int> raw(shared.get());
	func_ptr<int> internal(3);
	
	cout << "shared = " << *shared << endl;
	cout << "unique = " << *unique << endl;
	cout << "raw = " << *raw << endl;
	cout << "Changing shared..." << endl;
	*shared = 11;
	cout << "raw = " << *raw << endl;
	cout << "internal = " << *internal << endl;
	*/
	
	return 0;
}