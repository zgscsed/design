#include <iostream>
#include "singleton.h"

int main(void)
{
    // Singleton2::Ptr instance = Singleton2::getInstance();
    // Singleton2::Ptr instance2 = Singleton2::getInstance();
    Singleton3 & instance = Singleton3::getInstance();
    std::cout << "hello world"<<std::endl;

    return 0;
}