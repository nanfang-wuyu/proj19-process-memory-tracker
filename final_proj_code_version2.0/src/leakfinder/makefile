all: leakfinder.so c_example c_thread_example cpp_example
	
leakfinder.so: allocation_info.hpp allocation_info.cpp leakfinder.cpp
	g++ -shared -fPIC allocation_info.cpp leakfinder.cpp -o leakfinder.so -lpthread -ldl

c_example: c_example.c
	cc -rdynamic c_example.c -o c_example 

c_thread_example: c_thread_example.c
	cc -rdynamic c_thread_example.c -lpthread -o c_thread_example 

cpp_example: cpp_example.cpp
	g++ -rdynamic cpp_example.cpp -o cpp_example 

clean:
	rm -rf leakfinder.so c_example c_thread_example cpp_example
