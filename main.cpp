#include "c_string.h"

using namespace c_string;

int main(int argc, char* argv[])
{
    String64 str1("hello, worldj");
    String64 str2("hello, world");
    String64 str3("==");
    String64 str4("!=");
    if (str1 == str2) str3.Print();
    else str4.Print();
    return 0;
}