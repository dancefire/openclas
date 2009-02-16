#include <iostream>

int main(int argc, const char* argv[])
{
    while(!std::cin.eof())
    {
        unsigned char c;
        std::cin >> c;
        std::cout << std::hex << "\\x" << static_cast<unsigned short>(c);
    }
    std::cout << std::endl;
    return 0;
}
