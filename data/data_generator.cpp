#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include <iostream>
#include <string>
#include <bitset>

std::string decimal_to_binary_string(int num) 
{
    std::string binary_str = "";

    bool is_negative = num < 0;

    if (is_negative) 
        num = -num - 1;

    while (num > 0)
    {
        binary_str = (num % 2 == 0 ? "0" : "1") + binary_str;
        num /= 2;
    }

    if (is_negative) 
    {
        for (char& c : binary_str) 
            c = (c == '0') ? '1' : '0';

        while (binary_str.length() < 32)
            binary_str = "1" + binary_str;
    }
    else
    {
        while (binary_str.length() < 32)
            binary_str = "0" + binary_str;
    }

    return binary_str;
}

int main()
{
    unsigned rand_value = 1;
    srand(rand_value);

    std::fstream data1, data2;

    data1.open("data/data1.txt", std::ios::out | std::ios_base::trunc);
    data2.open("data/data2.txt", std::ios::out | std::ios_base::trunc);

    data1 << '<';
    data2 << '<';

    int size_of_data_files = 0;

    std::cin >> size_of_data_files;

    for(int i = 0; i < size_of_data_files;  ++i)
    {
        std::string random_number = decimal_to_binary_string(rand() - (RAND_MAX / 2));

        data1 << random_number << ' ';
    }

    for(int i = 0; i < size_of_data_files;  ++i)
    {
        std::string random_number = decimal_to_binary_string(rand() - (RAND_MAX / 2));

        data2 << random_number << ' ';
    }

    data1.seekp(-1, std::ios_base::cur);
    data2.seekp(-1, std::ios_base::cur);

    data1 << '>';
    data2 << '>';
}