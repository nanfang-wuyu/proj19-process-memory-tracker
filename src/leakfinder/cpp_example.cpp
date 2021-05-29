#include<iostream>

class my_class_a
{
private:
    int* leak;
    int* no_leak;
public:
    my_class_a()
    {
        leak = new int;
        no_leak = new int;
    }

    ~my_class_a()
    {
        delete no_leak;
    }
};

class my_class_b
{
private:
    my_class_a a_instance;
public:
    void foo()
    {
        int* leak = new int; 
    }

    void bar()
    {
        foo();
    }

    void foobar()
    {
       bar();
    }
};

void cpp_function()
{
    char* leak = new char; 
}

extern "C" void c_function()
{
    cpp_function();
}

int main(void)
{
    std::cout << "leakfinder C++ thread example app" << std::endl;
    std::cout << "This application is expected to leak" << std::endl;
    my_class_b b;
    b.foobar();
    b.foo();

    c_function();

    std::cout << "leakfinder C++ thread example app all done" << std::endl;
    return 0;
}

