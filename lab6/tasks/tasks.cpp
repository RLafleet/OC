#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>

struct Person 
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t nameLength;
    char name[59];
};

void ReadFile(const std::string& fileName) 
{
    std::ifstream file(fileName, std::ios::binary);

    if (!file) 
    {
        std::cerr << "File not exists: " << fileName << std::endl;
        return;
    }

    Person person;

    while (file.read(reinterpret_cast<char*>(&person), sizeof(Person))) 
    {
        std::cout << "Birthday Date: "
            << static_cast<int>(person.month) << "/"
            << static_cast<int>(person.day) << "/"
            << person.year << std::endl;

        std::cout << "Name: ";
        for (int i = 0; i < person.nameLength; ++i) 
        {
            std::cout << person.name[i];
        }
        std::cout << std::endl;

        std::cout << "---------------------------" << std::endl;
    }

    file.close();
}

int main() 
{
    const std::string fileName = "data.txt"; 
    ReadFile(fileName);
    return 0;
}