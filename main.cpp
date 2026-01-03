#include <iostream>
#include <string_view>

/*
    for grep 
    1. taking argument input
    2. parsing input
    3. file reading and parsing
    4. output based on parsing
*/

int main (int argc, const char* const argv[])
{
    if (argc <= 1)
    {
        std::cerr << "No arguments given.\n";
    }

    for (int i = 1; i < argc ; ++i)
    {
        std::string_view arg {argv[i]};
        
        std::cout << "arg["<< i << "] = " << arg << '\n';
    }


    return 0;
}