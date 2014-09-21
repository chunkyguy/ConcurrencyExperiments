#include <thread>
#include <iostream>


void PrintUsername(const char uname, const int count)
{
    for (int i = 0; i < count; ++i) {
        std::cout << uname << ' ';
    }
    std::cout << std::endl;
}

void MultiThreadRoomDontCare()
{
    std::thread threadX(PrintUsername, 'X', 10);
    std::thread threadY(PrintUsername, 'Y', 10);
    
    threadX.detach();
    threadY.detach();
}

void MultiThreadRoom()
{
    std::thread threadX(PrintUsername, 'X', 10);
    std::thread threadY(PrintUsername, 'Y', 10);
    
    threadX.join();
    threadY.join();
}


void SingleThreadRoom()
{
    PrintUsername('X', 10);
    PrintUsername('Y', 10);
}

int main()
{
    MultiThreadRoomDontCare();
    std::cout << "MainThread: Goodbye" << std::endl;
    return 0;
}