#include "c_string.h"
#include <iostream>
#include <string.h>

#define STRING_CPP(byte) char* String##byte::Chars(){return buffer;}int String##byte::Capacity(){return byte;}String##byte::String##byte(){}String##byte::String##byte(const char *str){Set(str);}

using namespace c_string;

STRING_CPP(64)
STRING_CPP(4096)

bool String::operator==(String &other)
{
    char *c1 = Chars();
    char *c2 = other.Chars();
    int i = 0;
    while (c1[i] == c2[i] && c1[i] != 0) i++;
    return c1[i] == 0 && c2[i] == 0;
}

void String::Set(const char *str)
{
    int size = Capacity() - 1;
    char *chars = Chars();
    int i;
    for (i = 0; i < size && str[i] != 0; i++) chars[i] = str[i];
    chars[i] = 0;
}

int String::Length()
{
    return strlen(Chars());
}

void String::Print()
{
    std::cout << Chars() << std::endl;
}