#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

struct Date
{
    uint16_t year;
    uint8_t day;
    uint8_t month;
};

struct Person
{
    Date birthDate;
    uint8_t nameLength;
    char name[59];
};

#endif //UTILS_H