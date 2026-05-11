#include <chrono>
#include <iostream>
#include <thread>

void print_hello()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Hello World!" << std::endl;
}

int main()
{
    std::thread worker(print_hello);
    worker.join();
    return 0;
}
